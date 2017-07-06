#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include "matriz.h"
#include <time.h>
#include "globals.h"

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

double calculateSingleFitnessValue(matrix object, int lines, int columns, double *quality1_values, double *quality2_values);


void reproduction(int population_size, int new_objects,
					double *fitnessValues, matrix population,
					double *quality1_values, double *quality2_values);

double getArrayValuesSum(double *v, int size);

int rouletteWheelSelection(int population_size, double *roulette);

void crossover(int population_size, int index_selected1, int index_selected2);

void mutation(int population_size, int index_selected1, matrix population);

typedef struct ObjectSummary{
    int index; //Numero do objeto. Para acessar na matrix precisa multiplicar por LINES_PER_SINGLE_OBJECT
    double fitnessValues; //Valor de aptidao
}objectSummary;

typedef struct Object{
    int k_line;
    int window;
    int request_beginning;
    int request_ending;
    int window_beginning;
    int window_ending;
}object;
