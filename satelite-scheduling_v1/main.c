#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <string.h>
#include "matriz.h"
#include <time.h>
#include "globals.h"
#include "algoritmoGenetico.h"

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

void calculateSignalQualityValues(double *array, int length){
    int i;
    for (i = 0; i < length; i++){
        array[i] = signalQualityValue(length, i);
    }
}

int main(int argc, char * argv[]){
    srand(1);
    char *filename;
	int size, i, j, l;
	int k = 0;
	int window1_size = 30;
	int window2_size = 31;
    double sum;
    double sum_aux = 0.0;
    individualSummary *individuals = malloc(MAX_INDIVIDUALS * sizeof(individualSummary));
    FILE *f = fopen("resultados/10000_GERACOES/crossover_10_teste.txt", "w");
    f_fit = fopen("values_10.txt", "w");
    f_csv = fopen("fitness_csv.csv", "w");

    // ========================= LEITURA DOS DADOS =============================
	int *dataset = read_ints(&size);

    // ====================== QUALIDADE DOS SINAIS =============================
	double quality1_values[window1_size];
    double quality2_values[window2_size];

    calculateSignalQualityValues(quality1_values, window1_size);
    calculateSignalQualityValues(quality2_values, window2_size);

    if(1){
        puts("Quality1");
        printDoubleArray(quality1_values, window1_size);

        puts("Quality2");
        printDoubleArray(quality2_values, window2_size);
    }


    // ========================== CRIA POPULACAO ===============================
    // Populacao original
    matrix population = alocateMatrix(MAX_LINES, REQUESTS);

    // Populacao auxiliar. Contera os novos individuos
    matrix new_population = alocateMatrix(NEW_INDIVIDUALS * LINES_PER_SINGLE_INDIVIDUAL, REQUESTS);

    initializeMatrix(population, MAX_LINES, REQUESTS);
    initializeMatrix(new_population, NEW_INDIVIDUALS * LINES_PER_SINGLE_INDIVIDUAL, REQUESTS);
    fillMatrixWithValues(population, MAX_LINES, REQUESTS, dataset);

    // ==================== CALCULA OS VALORES DE FITNESS ======================
    double *fitnessValues = malloc(MAX_INDIVIDUALS * sizeof(double));
    calculateFitnessValues(fitnessValues, MAX_INDIVIDUALS,
                            population, MAX_INDIVIDUALS, REQUESTS,
                            quality1_values,
                            quality2_values);

    // =========== COPIA VALORES DE APTIDAO PARA ESTRUTURA AUXILIAR ============
    copyFitnessValuesToIndividualStructure(fitnessValues, individuals, MAX_INDIVIDUALS);

    // ================= ORDENA INDIVIDUOS PELO VALOR DE APTIDAO ===============
    sortIndividualsByFitnessValues(individuals, MAX_INDIVIDUALS);

    // =========== COPIA OS VALORES ORDENADOS PARA O VETOR DE APTIDAO ==========
    copySortedFitnessValues(fitnessValues, individuals, MAX_INDIVIDUALS);

    // ============== IMPRIME VETOR DE APTIDOES NO ARQUIVO =====================
    fprintf(f, "\n================== PRIMEIROS VALORES DE APTIDOES ===================\n");
    printIndividualSummary(individuals, MAX_INDIVIDUALS, f);

    fprintf(stdout, "\n================== PRIMEIROS VALORES DE APTIDOES ===================\n");
    printIndividualSummary(individuals, MAX_INDIVIDUALS, stdout);

    // ====================== IMPRIME POPULACAO NO ARQUIVO =====================
    if(PRINTING_FLAG){
        printf("\n\n====POPULATION====\n\n");
    	printIntMatrix(population, MAX_LINES, REQUESTS, stdout);
    }

    fprintf(f, "\n=========== POPULACAO INICIAL ===========\n");
    printIntMatrix(population, MAX_LINES, REQUESTS, f);


    // time_begin = clock();
    // ============================= GERACOES ==================================
    runGenerations(MAX_GENERATION,
                population,
                new_population,
                NEW_INDIVIDUALS,
                fitnessValues,
                individuals,
                quality1_values, quality2_values);
    // time_end = clock();
    executionTime = time_end - time_begin;
    // ===================== IMPRIME VETOR DE APTIDOES =========================
    fprintf(stdout, "\n===================== NOVOS VALORES DE APTIDOES ===================\n");
    printIndividualSummary(individuals, MAX_INDIVIDUALS, stdout);

    // ====================== IMPRIME POPULACAO FINAL  =========================
    // puts("====================== IMPRIME POPULACAO FINAL  ===================");
    // printIntMatrix(population, MAX_LINES, REQUESTS, stdout);

    fprintf(f, "\n===================== NOVOS VALORES DE APTIDOES ===================\n");
    printIndividualSummary(individuals, MAX_INDIVIDUALS, f);

    fprintf(f, "====================== POPULACAO FINAL  ======================\n");
    printIntMatrix(population, MAX_LINES, REQUESTS, f);

    printStatistics(f);
    printStatistics(stdout);
    printAlgorithmInformations(f);
    printAlgorithmInformations(stdout);
    fclose(f);
    fclose(f_fit);
    fclose(f_csv);
}
