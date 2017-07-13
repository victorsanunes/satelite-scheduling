#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include "matriz.h"
#include <time.h>
#include "globals.h"
#include "algoritmoGenetico.h"


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

//Funcao que realizara a troca de variaveis
void swapObjects(objectSummary *vetor, int a, int b){
    objectSummary aux;
    aux = vetor[a];
    vetor[a] = vetor[b];
    vetor[b] = aux;
}


void sortObjectsByFitnessValues(objectSummary *vetor, int size){
    int i, j;

    for (i = 0; i < size; i++) {
        for(j = i + 1; j < size; j++){
            if(vetor[i].fitnessValue > vetor[j].fitnessValue ){
                swapObjects(vetor, i, j);
            }
        }
    }
}

int main(){
	int size, i, j, l;
	int k = 0;
	int window1_size = 30;
	int window2_size = 31;
    double sum;
    double sum_aux = 0.0;
    objectSummary *objects = malloc(MAX_OBJECTS * sizeof(objectSummary));
	//Le os dados
	int *dataset = read_ints(&size);

	//Calcula a qualidade do sinal
	double quality1[2][window1_size];
	// double quality1_values[window1_size];
	double *quality1_values = malloc(window1_size * sizeof(double));
	double quality2[2][window2_size];
	// double quality2_values[window2_size];
    double *quality2_values = malloc(window2_size * sizeof(double));

	// printf("====SIGNAL QUALITY 1 ====\n\n");
	for(i = 0; i < window1_size; i++){
		quality1[1][i] = signalQualityValue(window1_size, i);
		quality1_values[i] = signalQualityValue(window1_size, i);
		// printf("%d: %.2f| ",i+1, quality1[1][i]);
	}

	// printf("\n\n====SIGNAL QUALITY 2 ====\n\n");
	for(i = 0; i < window2_size; i++){
		quality2[1][i] = signalQualityValue(window2_size, i);
		quality2_values[i] = signalQualityValue(window2_size, i);
		// printf("%d: %.2f| ",i+1, quality2[1][i]);
	}

	//Popula a matrix de individuos
	matrix population = alocateMatrix(MAX_LINES, REQUESTS);
    initializeMatrix(population, MAX_LINES, REQUESTS);
    fillMatrixWithValues(population, MAX_LINES, REQUESTS, dataset);

	// printf("\n\n====POPULATION====\n\n");
	// printIntMatrix(population, MAX_LINES, REQUESTS);

	//Calcula funcao objetivo
    double *fitnessValues = malloc(MAX_OBJECTS * sizeof(double));
    calculateFitnessValues(fitnessValues, MAX_OBJECTS,
                            population, MAX_OBJECTS, REQUESTS,
                            quality1_values,
                            quality2_values);

	// printf("\n\n====FITNESS VALUES====\n\n");
	// printDoubleArray(fitnessValues, MAX_OBJECTS);

    //Preenche a estrutura para ordenar os valores de aptidao
    for(i = 0; i < MAX_OBJECTS; i++){
        //Calcula o indice do objeto na matriz de populacao
        objects[i].index = i * LINES_PER_SINGLE_OBJECT;
        objects[i].fitnessValue = fitnessValues[i];
    }

    sortObjectsByFitnessValues(objects, MAX_OBJECTS);

    for(i = 0; i < MAX_OBJECTS; i++){
        fitnessValues[i] = objects[i].fitnessValue;
    }

    srand(1);
    for(i = 0; i < MAX_GENERATION; i++){

        reproduction(MAX_OBJECTS, NEW_OBJECTS, fitnessValues, population, quality1_values, quality2_values, objects);
        sortObjectsByFitnessValues(objects, MAX_OBJECTS);
        for(j = 0; j < MAX_OBJECTS; j++){
            fitnessValues[j] = objects[j].fitnessValue;
        }
        // printf("\n\n====FITNESS VALUES====\n\n");
    	// printDoubleArray(fitnessValues, MAX_OBJECTS);
    }

    printf("\n\n====FITNESS VALUES====\n\n");
    for(i = 0; i < MAX_OBJECTS; i++){
        printf("%d: ", objects[i].index/LINES_PER_SINGLE_OBJECT);
        printf("%.2f |", fitnessValues[i]);
    }

    imprimeContador();
    printf("\n\n==== LAST POPULATION ====\n\n");
    printIntMatrix(population, MAX_LINES, REQUESTS);

    printStatistics();
	return 0;
}
