#include "algoritmoGenetico.h"

int contador[20] = {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0};

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

void printDoubleArray(double *array, int size){
    int i;
    for (i = 0; i < size; i++){
        printf("%d: %.2f| ",i+1, array[i]);
        }
    printf("\n");
}

double truncateValue(int numerador, int denominador){
    double resultado = numerador/denominador;
    double resto = numerador % denominador;

    if(!resto)
        return resultado;
    else return (resultado + 1);
}

/*double *signalQualityValue(int windowSize){
    double *quality = malloc(windowSize * sizeof(double));
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

double signalQualityValue(int windowSize, int k){
    //double quality;
    //int k;
	if((k+1) <= truncateValue(windowSize, 2)){
		return ((k+1) * 1/truncateValue(windowSize, 2));
	}
	else{
		return (windowSize - k ) /truncateValue(windowSize, 2);
	}

    //return quality;
}

double fitnessFunction(	double *quality1,
						double *quality2,
						int window,
						int k_line,
						int request_beginning,
						int request_ending,
						int window_beginning,
						int window_ending){

	int weight = 1;
	double value = 0.0;

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

void calculateFitnessValues(double *fitnessValues,int size,
                            matrix population, int lines, int columns,
                            double *quality1_values,
                            double *quality2_values){
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
                fitnessValues[k] += -0.00;
            }
        }

		// printf("%d\n", interval);
		i = i + LINES_PER_SINGLE_OBJECT;
	}



}

double calculateSingleFitnessValue(matrix object, int lines, int columns, double *quality1_values, double *quality2_values){
    // @TODO: Corrigir calculo da funcao para unico individuo
    // Alternativa e calcular a funcao para os 5 individuos diretamente
    int i, j, k, l;
    int window, k_line, request_beginning, request_ending, window_beginning, window_ending;
    int interval, next_window, next_request_beginning, next_request_ending;
    int request_size;
    double fitnessSingleValue = 0.0;


    for(k = 0; k < lines; k++){
		//Percorre a matriz do individuo
		for(j = 0; j < columns; j++){
		    window                = object[i][j];
		    k_line                = object[i + 1][j];
		    request_beginning     = object[i + 2][j];
            request_ending        = object[i + 3][j];
            request_size = request_ending - request_beginning;

            //Checa a restrição de não preemptivadade: não pode ter mais de uma
            //solicitacao ao mesmo tempo para a mesma estacao.
            //Faz a checagem da solicitacao atual com todas as outras
            for(l = j + 1; l < columns; l++){
                next_window = object[i][l];
                next_request_beginning = object[i + 2][l];
                next_request_ending = object[i + 3][l];
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
                window_beginning 	= object[i + 4][j];
                window_ending 		= object[i + 5][j];
                fitnessSingleValue 	+= fitnessFunction(	quality1_values,
                                                        quality2_values,
                                                        window, k_line,
                                                        request_beginning,
                                                        request_ending,
                                                        window_beginning,
                                                        window_ending);
            }
            else {
                fitnessSingleValue += -0.00;
            }
        }
	}
    return fitnessSingleValue;

}

double getArrayValuesSum(double *v, int size){
    int i;
    double sum;
    for(i = 0; i < size; i++){
        sum += v[i];
    }
    return sum;
}

int rouletteWheelSelection(int population_size, double *roulette){
    int index_object;
    double probability_value, sum_probability_values;
    int index_selected;

    //Seleciona um valor de probabilidade para escolher o individuo
    probability_value = (rand() % 10000) / 100.0; //Valores reais entre 0.00 e 99.99
    printf("valor sorteado = %lf\n", probability_value);

    index_object = 0;
    sum_probability_values = roulette[index_object];

    //Itera até encontrar o individuo sorteado
    while(sum_probability_values < probability_value){
        index_object++;
        sum_probability_values += roulette[index_object];
    }
    //Melhorar com busca binaria

    index_selected = index_object;
    contador[index_selected]++;
    return index_selected;
}

void crossover(int population_size, int index_selected1, int index_selected2){}

void mutation(int population_size, int index_selected1, matrix population){

    int matrix_index;
    matrix_index = index_selected1 * LINES_PER_SINGLE_OBJECT;
    int i, j;

    //Insere os valores do novo individuo na populacao
    for (i = 0; i < REQUESTS; i++){
        population[matrix_index + 1][i] += 1;
        population[matrix_index + 2][i] += 1;
        population[matrix_index + 3][i] += 1;
    }
}

void reproduction(int population_size, int new_objects, double *fitnessValues, matrix population, double *quality1_values, double *quality2_values){
    printf("\n\n\tReproducao\n");
    // srand(1);
    // srand(time(NULL));
    int index_new_object, index_object;
    int index_father1, index_father2;
    double *roulette;
    int probability_value;
    int crossover_rate;
    double sum_fitness_values;
    int index_selected1, index_selected2;
    int object_counter = 0;
    int i, j, k;
    int window, k_line, request_beginning, request_ending, window_beginning, window_ending;

    //Linha inicial dos objetos na matrix de populacao
    int object1_line, object2_line;

    matrix new_population_objects = alocateMatrix(new_objects * LINES_PER_SINGLE_OBJECT, REQUESTS);
    matrix single_new_object1 = alocateMatrix(LINES_PER_SINGLE_OBJECT, REQUESTS);
    initializeMatrix(new_population_objects, new_objects * LINES_PER_SINGLE_OBJECT, REQUESTS);
    initializeMatrix(single_new_object1, LINES_PER_SINGLE_OBJECT, REQUESTS);

    //Parametros do algoritmo genetico
    crossover_rate = 90;

    //Montagem da roleta
    roulette = (double*)malloc(population_size * sizeof(double));
    sum_fitness_values = 0.0;
    sum_fitness_values = getArrayValuesSum(fitnessValues, population_size);
    printf("Somatorio de aptidoes: %lf\n", sum_fitness_values);

    //Calcula as probabilidades para cada individuo
    for(index_object = 0; index_object < population_size; index_object++){
        roulette[index_object] = 100.0 * fitnessValues[index_object]/sum_fitness_values;
    }


    i = 0;
    while(i <= new_objects){

        // Gera valores ntre 1 e 100
        // srand(time(NULL));
        probability_value = rand() % 100 + 1;
        printf("Proabilidade: %d%%\n", probability_value);

        if(probability_value > crossover_rate){
            printf("Método: Crossover\n");
            i += 2;
            printf("%d individuos a serem criados....\n", i);

            //Sorteia os objetos
            index_selected1 = rouletteWheelSelection(population_size, roulette);
            index_selected2 = rouletteWheelSelection(population_size, roulette);
            while(index_selected1 == index_selected2){
                printf("index_selected1 == index_selected2. Selecionando novo... ");
                index_selected2 = rouletteWheelSelection(population_size, roulette);
            }
            if(i > new_objects){
                printf("Numero maximo de objetos\n");
                index_selected2 = index_selected1;
            }
            crossover(population_size, index_selected1, index_selected2);
            printf("Selecionado 1 = %d\n", index_selected1);
            printf("Selecionado 2 = %d\n", index_selected2);

        }
        else{
            printf("Método: Mutacao\n" );
            i += 1;
            index_selected1 = rouletteWheelSelection(population_size, roulette);
            mutation(population_size, index_selected1, population);
            printf("Selecionado 1 = %d\n", index_selected1);


            //Linha do individuo na matriz de populacao
            object1_line = index_selected1 * LINES_PER_SINGLE_OBJECT;
            printf("Linha do individuo na matriz de populacao: %d\n", object1_line);

            //Preenche novo individuo com seus valores
            for(j = 0; j < LINES_PER_SINGLE_OBJECT; j++){
                for(k = 0; k < REQUESTS; k++){
                    // printf("%d\t", population[object1_line][k]);
                    single_new_object1[j][k] = population[object1_line][k];
                    //printf("%d\t", single_new_object1[j][k]);
                }
                //printf("\n");
                object1_line += 1;
            }

            @TODO: Calcular fitness apos a geracao dos 5 individuos
            //Calcula o valor da funcao objetivo para um individuo especifico
            // fitnessValues[index_selected1] = calculateSingleFitnessValue(single_new_object1,
            //                                     LINES_PER_SINGLE_OBJECT, REQUESTS,
            //                                     quality1_values, quality2_values);
            // printf("fitness: %lf\n", fitnessValues[index_selected1]);

        }
    //i++;
    }
    printIntArray(contador, 20);
    free(roulette);
}
