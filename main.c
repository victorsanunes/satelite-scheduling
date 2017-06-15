#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include "matriz.h"

#define MAX_SIZE 500
//Quantidade maxima de individuos iniciais
#define MAX_OBJECTS 20 //20
//Numero de solicitacoes
#define REQUESTS 4
//Numero de linhas que cada individuo ocupara na matriz
#define LINES_PER_SINGLE_OBJECT 6
//Numero de atributos de cada individuo
#define FEATURES_NUMBER REQUESTS * LINES_PER_SINGLE_OBJECT
//Numero maximo de linhas
#define MAX_LINES MAX_OBJECTS * LINES_PER_SINGLE_OBJECT
//Discretizacao da janela
//#define WINDOW_SIZE 5

//Tamanho do horizonte de agendamento
#define HORIZON_SIZE 50

// #define INPUT_FILE "POPULACAO_INICIAL - CASO OTIMO(2).txt"
#define INPUT_FILE "POPULACAO_INICIAL - FORMATADA (cópia).txt"

void printIntArray(int *array, int size);

int *read_ints(int *size){
    //FILE *file = fopen("POPULACAO_INICIAL - FORMATADA.txt", "r");
    FILE *file = fopen(INPUT_FILE, "r");
    //Conjunto de dados a ser lido
    int *dataset = malloc(MAX_SIZE * sizeof(int));
    int i = 0;

    //Variavel com a quantidade de numeros lidos
    *size = 0;
    //Realiza leitura do arquivo file e armazena o valor na posicao i do dataset
    fscanf(file, "%d", &dataset[i]);

    //feof(filename) eh uma funcao que verifica o fim do arquivo
    while(!feof(file)){
        //printf("%d ", dataset[i]);
        i++;
        fscanf(file, "%d", &dataset[i]);

        (*size)++;
    }
    fclose(file);
    return dataset;
}

// void printIntMatrix(int matrix[MAX_LINES][REQUESTS], int lines, int columns){
//     int i, j;
// 	for(i = 0; i < lines; i++){
// 		for(j = 0; j < columns; j++){
// 			printf("%d\t", matrix[i][j]);
// 		}
// 		printf("\n");
// 		//Pula mais uma linha a cada LINES_PER_SINGLE_OBJECT linhas
// 		if((i+1) % LINES_PER_SINGLE_OBJECT == 0){
//             printf("\n");
//         }
// 	}
// }

void printIntArray(int *array, int size){
    int i;
    for (i = 0; i < size; i++){
        printf("%d ", array[i]);
        }
    printf("\n");
}

void printFloatArray(float *array, int size){
    int i;
    for (i = 0; i < size; i++){
        printf("%d: %.2f| ",i+1, array[i]);
        }
    printf("\n");
}

float truncateValue(int numerador, int denominador){
    float resultado = numerador/denominador;
    float resto = numerador % denominador;

    if(!resto)
        return resultado;
    else return (resultado + 1);
}

/*float *signalQualityValue(int windowSize){
    float *quality = malloc(windowSize * sizeof(float));
    int k;

    for(k = 1; k <= windowSize; ++k){
        if(k <= truncateValue(windowSize, 2)){
            quality[k-1] = k * 1/truncateValue(windowSize, 2);
        }
        else{
            quality[k-1] = (windowSize - k + 1) /truncateValue(windowSize, 2);
        }
    }
    return quality;
}*/

float signalQualityValue(int windowSize, int k){
    //float quality;
    //int k;
	if((k+1) <= truncateValue(windowSize, 2)){
		return ((k+1) * 1/truncateValue(windowSize, 2));
	}
	else{
		return (windowSize - k ) /truncateValue(windowSize, 2);
	}

    //return quality;
}

float fitnessFunction(	float *quality1,
						float *quality2,
						int window,
						int k_line,
						int request_beginning,
						int request_ending,
						int window_beginning,
						int window_ending){

	int weight = 1;
	float value = 0.0;

    //Checa se as restricoes estao sendo atendidas, se nao estiverem, zera os valores
	if(request_beginning < window_beginning || request_ending > window_ending || request_beginning >= request_ending){

        weight = 0;
		value = 0.0;
	}
	else if(window == 1){
			// printf("\nq = %.2f\n", quality1[k_line - 1]);
			value += weight * quality1[k_line - 1];
		}
	else if(window == 2){
			// printf("\nq = %.2f\n", quality2[k_line - 1]);
			value += weight * quality2[k_line - 1];
	}


	//printf("sum: %.2f\n", value);
	return value;
}

void calculateFitnessValues(float *fitnessValues,int size,
                            matrix population, int lines, int columns,
                            float *quality1_values,
                            float *quality2_values){
    int i, j, k, l;
    int window, k_line, request_beginning, request_ending, window_beginning, window_ending;
	int interval, next_window, next_request_beginning, next_request_ending;
    int request_size;

    //Zera os valores do vetor
    for(i = 0; i < size; i++){
        fitnessValues[i] = 0;
    }

    i = 0;
    //for para preencher o vetor com os valores calculados da funcao
	for(k = 0; k < lines; k++){
		//Percorre a matriz de populacao
		for(j = 0; j < columns; j++){
		    window                = population[i][j];
		    k_line                = population[i + 1][j];
		    request_beginning     = population[i + 2][j];
            request_ending        = population[i + 3][j];
            request_size = request_ending - request_beginning;

            //Checa a restrição de não preemptivadade: não pode ter mais de uma
            //solicitacao ao mesmo tempo para a mesma estacao.
            //Faz a checagem da solicitacao atual com todas as outras
            for(l = j + 1; l < columns; l++){
                next_window = population[i][l];
                next_request_beginning = population[i + 2][l];
                next_request_ending = population[i + 3][l];
                // printf("Pa: %d   Pp: %d\n", request_beginning, next_request_beginning);
    			//Checa se a proxima solicitacao esta na mesma janela
    			if(window == next_window){
                    // printf("mesmo janela\n");
                    if((request_ending >= next_request_beginning - 1) && (request_beginning <= next_request_ending + 1)){
                        // printf("preemptivadade\n");
                        interval = -1; //Sera penalizado
                    }
                    else{
                        interval = 2;
                    }
    			    // else{
                    //     interval = abs(next_request_beginning - request_ending);
                    // }
    			}

    			else {
                    if((request_ending >= next_request_beginning - 1) && (request_beginning <= next_request_ending + 1)){
                        // printf("redundancia\n");
                        interval = -1; //Sera penalizado
                    }
                    else{
                        interval = 2;
                    }
    			}
                //Testa se houve um salto no agendamento entre uma solicitacao e outra

    		}
            if(interval >= 2){
                // printf("entrou no interval\n" );
                window_beginning 	= population[i + 4][j];
                window_ending 		= population[i + 5][j];
                fitnessValues[k] 	+= fitnessFunction(	quality1_values,
                                                        quality2_values,
                                                        window, k_line,
                                                        request_beginning,
                                                        request_ending,
                                                        window_beginning,
                                                        window_ending);
            }
            else {
                fitnessValues[k] += -1000.00;
            }
        }

		// printf("%d\n", interval);
		i = i + LINES_PER_SINGLE_OBJECT;
	}



}

int main(){
	int size, i, j, l, value;
	int k = 0;
	int window1_size = 30;
	int window2_size = 31;

	//Le os dados
	int *dataset = read_ints(&size);

	//Calcula a qualidade do sinal
	float quality1[2][window1_size];
	// float quality1_values[window1_size];
	float *quality1_values = malloc(window1_size * sizeof(float));
	float quality2[2][window2_size];
	// float quality2_values[window2_size];
    float *quality2_values = malloc(window2_size * sizeof(float));

	printf("====SIGNAL QUALITY 1 ====\n\n");
	for(i = 0; i < window1_size; i++){
		quality1[1][i] = signalQualityValue(window1_size, i);
		quality1_values[i] = signalQualityValue(window1_size, i);
		printf("%d: %.2f| ",i+1, quality1[1][i]);
	}

	printf("\n\n====SIGNAL QUALITY 2 ====\n\n");
	for(i = 0; i < window2_size; i++){
		quality2[1][i] = signalQualityValue(window2_size, i);
		quality2_values[i] = signalQualityValue(window2_size, i);
		printf("%d: %.2f| ",i+1, quality2[1][i]);
	}

	//Popula a matrix de individuos
	matrix population = alocateMatrix(MAX_LINES, REQUESTS);
    initializeMatrix(population, MAX_LINES, REQUESTS);
    fillMatrixWithValues(population, MAX_LINES, REQUESTS, dataset);

	printf("\n\n====POPULATION====\n\n");
	printIntMatrix(population, MAX_LINES, REQUESTS);

	//Calcula funcao objetivo
    float *fitnessValues = malloc(MAX_OBJECTS * sizeof(float));
    calculateFitnessValues(fitnessValues, MAX_OBJECTS, population, MAX_OBJECTS, REQUESTS, quality1_values, quality2_values);

	printf("\n\n====FITNESS VALUES====\n\n");
	printFloatArray(fitnessValues, MAX_OBJECTS);
	return 0;
}
