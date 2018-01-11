#include <stdlib.h>
#include <stdio.h>
#include "globals.h"

typedef int** matrix;

matrix alocateMatrix(int lines, int columns);
void initializeMatrix(matrix m, int lines, int columns);
void printIntMatrix(matrix m, int lines, int columns, FILE *f);
void fillMatrixWithValues(matrix m, int lines, int columns, int *dataset);
void crossMatrixSegment(matrix A, int firstAColumn, int lastAColumn,
                        matrix B, int firstBColumn, int lastBColumn,
                        int lines, int columns);
void copyMatrix(matrix origin, matrix destiny, int lines, int columns);
void freeMatrix(matrix m, int rows);
