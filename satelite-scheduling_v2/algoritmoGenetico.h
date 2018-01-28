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

// uma solicitacao eh um vetor
typedef struct request{
    int features[LINES_PER_SINGLE_INDIVIDUAL];
}request;

// Um individuo eh uma estrutura com solicitacoes
typedef struct individual{
    request *req;
    int index;
    double fitness;
}individual;

individual *createPopulationArray(int length, int requestNumber, int *dataset);
void printRequestArray(request *req, int requestNumber);
void printPopulation(individual *population, int size);
double signalQualityValue(int windowSize, int k);
void calculateSignalQualityValues(double *array, int length);
void calculateFitnessValues(double *fitnessValues,int size,
                            individual *population, int lines, int columns,
                            double *quality1_values,
                            double *quality2_values);

double fitnessFunction(	double *quality1,
						double *quality2,
						int window,
						int k_line,
						int request_beginning,
						int request_ending,
						int window_beginning,
						int window_ending);

int getWindow(individual *ind, int individualID, int requestID);
int getWindowBeginning(individual *ind, int individualID, int requestID);
int getWindowEnding(individual *ind, int individualID, int requestID);
int getKLine(individual *ind, int individualID, int requestID);
int getRequestBeginning(individual *ind, int individualID, int requestID);
int getRequestEndingw(individual *ind, int individualID, int requestID);
int getStation(individual *ind, int individualID, int requestID);
void sortPopulation(individual *population, int size);
void swapIndividuals(individual *population, int a, int b);
void runGenerations(int maxGeneration,
                    individual *originalPopulation,
                    individual *newPopulation,
                    double *quality1_values, double *quality2_values);
//======================================================================
