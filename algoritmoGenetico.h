#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include "matriz.h"
#include <time.h>
#include "globals.h"

//Estrutura utilizada para ordenação do vetor
typedef struct individualSummary{
    int index; // Primeira linha do objeto na matriz de populacao
    double fitnessValue; //Valor de aptidao
}individualSummary;

void printIndividualSummary(individualSummary *individuals, int length, FILE *f);

void printIntArray(int *array, int size);

void printDoubleArray(double *array, int size);

double truncateValue(int numerador, int denominador);

double signalQualityValue(int windowSize, int k);

double fitnessFunction(	double *quality1,
						double *quality2,
						int window,
						int k_line,
						int request_beginning,
						int request_ending,
						int window_beginning,
						int window_ending);

void calculateFitnessValues(double *fitnessValues,int size,
                            matrix population, int lines, int columns,
                            double *quality1_values,
                            double *quality2_values);

void runGenerations(int maxGeneration,
                    matrix originalPopulation,
                    matrix newPopulation,
                    int newIndividuals,
                    double *fitnessValues,
                    individualSummary *individualsArray,
                    double *quality1_values, double *quality2_values);

double getArrayValuesSum(double *v, int size);

int rouletteWheelSelection(double *roulette, int length, individualSummary *individuals);

void fillRoulette(double *roulette, double *fitnessValues, int length);

void insertIndividualsInPopulation(matrix population, matrix individual,
                                int firstLinePosition, int lines, int columns);

void reproduction(int maxGeneration,
                    matrix originalPopulation,
                    matrix newPopulation,
                    int newIndividuals,
                    double *fitnessValues,
                    individualSummary *individualsArray,
                    double *quality1_values, double *quality2_values);

void crossover(matrix originalPopulation, matrix newPopulation,
                int matrixIndex_1, int matrixIndex_2,
                int selected1_matrixInsertionPosition,
                int selected2_matrixInsertionPosition);

void printStatistics(FILE *f);

void printAlgorithmInformations(FILE *f);

void swapIndividuals(individualSummary *vetor, int a, int b);

void sortIndividualsByFitnessValues(individualSummary *vetor, int size);

void copyFitnessValuesToIndividualStructure(double *fitnessArray, individualSummary *individualArray, int length);

void copySortedFitnessValues(double *fitnessArray, individualSummary *individualArray, int length);

void mutation(matrix originalPopulation,
            int individualOriginalPosition,
            int lines, int columns,
            int insertionIndex);
/*double calculateSingleFitnessValue(matrix individuals, int lines, int columns,
                            double *quality1_values, double *quality2_values);


void crossover(matrix population, matrix new_population,
                int matrix_index1, int matrix_index2,
                int index_selected1, int index_selected2);
void mutation(matrix population, matrix new_population, int lines, int columns, int matrix_index, int index);
void updatePopulation(matrix population, matrix new_population,
                        int linesPerSingleindividuals, int columns,
                        int new_individualss, individualSummary *individualss);
void insertindividualsInPopulation(matrix population, matrix individuals,
                                int firstLinePosition, int lines, int columns);
void imprimeContador(individualSummary *individualss);
void printStatistics();
*/
