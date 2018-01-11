#include <stdlib.h>
#include <stdio.h>
#include "globals.h"
#include "algoritmoGenetico.h"
#include "utils.h"



/*
individual *createPopulationArray(int length, int requestNumber, int *dataset){
    int i, j, k, index;

    // Vetor da populacao
    individual *array = malloc(length * sizeof(individual));
    for (i = 0; i < length; i++){

        //printf("I=%d\n", i);
        array[i].req = malloc(requestNumber * sizeof(request));
        for(j = 0; j < requestNumber; j++){
            index = j + (i * FEATURES_NUMBER);
            //printf("J=%d\n", j);
            for(k = 0; k < LINES_PER_SINGLE_INDIVIDUAL; k++){
                //printf("dataset[index]=%d\nindex=%d\n\n",dataset[index], index);

                array[i].req[j].features[k] = dataset[index];
                index += REQUESTS;
                //printf("lido=%d\n", dataset[index]);
            }
            //printf("=========\n\n");
            array[i].req[j].index = i;
            array[i].req[j].window = i;
            array[i].req[j].k_line = 42 + j;
            array[i].req[j].request_beginning = -1;
            array[i].req[j].request_ending = -1;
            array[i].req[j].station = -1;
            array[i].req[j].window_beginning = -1;
            array[i].req[j].window_ending = -1;

        }
        //array[i].index = i;
    }
    return array;
}


void printRequestArray(request *req, int requestNumber){
    int i, j;

    for(i = 0; i < LINES_PER_SINGLE_INDIVIDUAL; i++){
        for(j = 0; j < requestNumber; j++){
            printf("%d\t", req[j].features[i]);
        }
        printf("\n");
    }
}

void printPopulation(individual *population, int size){
    int i;
    for(i = 0; i < size; i++){
        printRequestArray(population[i].req, REQUESTS);
        printf("===========\n");
    }
}*/

void main(){
    // ============================= FLAGS =====================================
    int imprimeQualidades = 0;
    int imprimePopulacaoInicial = 0;

    int size, window1_length, window2_length;

    // ========================= LEITURA DOS DADOS =============================
    int *dataset = read_ints(&size);
    individual *population = createPopulationArray(MAX_INDIVIDUALS, REQUESTS, dataset);
    if(imprimePopulacaoInicial)
        printPopulation(population, MAX_INDIVIDUALS);

    // ====================== QUALIDADE DOS SINAIS =============================
    window1_length = 30;
    window2_length = 31;
    double quality1_values[window1_length];
    double quality2_values[window2_length];
    calculateSignalQualityValues(quality1_values, window1_length);
    calculateSignalQualityValues(quality2_values, window2_length);

    if(imprimeQualidades) {
        puts("Quality1");
        printDoubleArray(quality1_values, window1_length);

        puts("Quality2");
        printDoubleArray(quality2_values, window2_length);
    }

    // ==================== CALCULA OS VALORES DE FITNESS ======================
    double *fitnessValues = malloc(MAX_INDIVIDUALS * sizeof(double));
    calculateFitnessValues(fitnessValues, MAX_INDIVIDUALS,
                            population, MAX_INDIVIDUALS, REQUESTS,
                            quality1_values,
                            quality2_values);


}
