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
		if((i+1) % LINES_PER_SINGLE_OBJECT == 0){
            printf("\n");
        }
	}
}

void copyMatrix(matrix origin, matrix destiny, int lines, int columns){

    int i, j;
    for(i = 0; i < lines; i++){
        for(j = 0; j < columns; j++){
            destiny[i][j] = origin[i][j];
        }
    }

}

// firstAColumn e lastAColumn sao indices para o trecho da matriz A que sera
// cruzado com B.
// firstBColumn e lastBColumn sao indices para o trecho da matriz B que sera
// cruzado com A.
void crossMatrixSegment(matrix A, int firstAColumn, int lastAColumn,
                        matrix B, int firstBColumn, int lastBColumn,
                        int lines, int columns){

    int iA, aux_index, jA, jB;
    //Aloca matriz auxiliar
    matrix aux_matrix = alocateMatrix(lines, columns);
    initializeMatrix(aux_matrix, lines, columns);

    // printf("matriz A (original)\n" );
    // printIntMatrix(A, lines, columns);
    // printf("matriz B (original)\n" );
    // printIntMatrix(B, lines, columns);

    //Copia a segunda metade da matriz A para a matriz auxiliar
    for(iA = 0; iA < lines; iA++){
        for(jA = firstAColumn; jA <= lastAColumn; jA++){
            aux_matrix[iA][jA] = A[iA][jA];
        }
    }

    //Insere trecho de B em A
    for(iA = 0; iA < lines; iA++){
        for(jA = firstBColumn; jA <= lastBColumn; jA++){
            A[iA][jA] = B[iA][jA];
        }
    }

    // printf("matriz aux\n" );
    // printIntMatrix(aux_matrix, lines, columns);
    //
    // printf("matriz A (cruzada)\n" );
    // printIntMatrix(A, lines, columns);

    //Insere trecho de A em B
    for(iA = 0; iA < lines; iA++){
        for(jA = firstAColumn; jA <= lastAColumn; jA++){
            B[iA][jA] = aux_matrix[iA][jA];
        }
    }
    // printf("matriz B (cruzada)\n" );
    // printIntMatrix(B, lines, columns);
}
