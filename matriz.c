#include "matriz.h"

matrix alocateMatrix(int lines, int columns){
    int i;
    matrix m;
    m = malloc(lines * sizeof(*m));
    for(i = 0; i < lines; i++){
        m[i] = malloc(columns *sizeof(*m[i]));
    }
    return m;
}

void initializeMatrix(matrix m, int lines, int columns){
    int i, j;
    for (i = 0; i < lines; i++){
        for(j = 0; j < columns; j++){
            m[i][j] = 0;
        }
    }
}

void fillMatrixWithValues(matrix m, int lines, int columns, int *dataset){
    int i, j, k;
    k = 0;
    for(i = 0; i < lines; i++){
        for(j = 0; j < columns; j++){
            m[i][j] = dataset[k];
            k++;
        }
    }
}

void printIntMatrix(matrix m, int lines, int columns){
    int i, j;
	for(i = 0; i < lines; i++){
		for(j = 0; j < columns; j++){
			printf("%d\t", m[i][j]);
		}
		printf("\n");
		//Pula mais uma linha a cada LINES_PER_SINGLE_OBJECT linhas
		// if((i+1) % LINES_PER_SINGLE_OBJECT == 0){
        //     printf("\n");
        // }
	}
}
