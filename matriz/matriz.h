#include <stdlib.h>
#include <stdio.h>

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

typedef int** matrix;

matrix alocateMatrix(int lines, int columns);
void initializeMatrix(matrix m, int lines, int columns);
void printIntMatrix(matrix m, int lines, int columns);
void fillMatrixWithValues(matrix m, int lines, int columns, int *dataset);
int *read_ints(int *size);
