#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include "matriz.h"
#include <time.h>
#include "globals.h"

//Estrutura utilizada para ordenação do vetor
typedef struct ObjectSummary{
    int index; //Numero do objeto. Para acessar na matrix precisa multiplicar por LINES_PER_SINGLE_OBJECT
    double fitnessValue; //Valor de aptidao
}objectSummary;

typedef struct Object{
    int k_line;
    int window;
    int request_beginning;
    int request_ending;
    int window_beginning;
    int window_ending;
}object;

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
double calculateSingleFitnessValue(matrix object, int lines, int columns,
                            double *quality1_values, double *quality2_values);
void reproduction(int population_size, int new_objects,
					double *fitnessValues, matrix population, matrix new_population,
					double *quality1_values, double *quality2_values,
					objectSummary *objects);
double getArrayValuesSum(double *v, int size);
int rouletteWheelSelection(int population_size, double *roulette, objectSummary *objects);
void crossover(matrix population, matrix new_population,
                int matrix_index1, int matrix_index2,
                int index_selected1, int index_selected2);
void mutation(matrix population, matrix new_population, int lines, int columns, int matrix_index, int index);
void updatePopulation(matrix population, matrix new_population,
                        int linesPerSingleObject, int columns,
                        int new_objects, objectSummary *objects);
void insertObjectInPopulation(matrix population, matrix object,
                                int firstLinePosition, int lines, int columns);
void imprimeContador();
void printStatistics();
