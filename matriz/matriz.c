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

void main(){
    int n, *dataset;
    matrix m = alocateMatrix(MAX_LINES, REQUESTS);
    initializeMatrix(m, MAX_LINES, REQUESTS);
    dataset = read_ints(&n);
    fillMatrixWithValues(m, MAX_LINES, REQUESTS, dataset);
    printIntMatrix(m, MAX_LINES, REQUESTS);
}
