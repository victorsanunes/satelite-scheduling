#include "utils.h"
#include "globals.h"

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

double truncateValue(int numerador, int denominador){
    double resultado = numerador/denominador;
    double resto = numerador % denominador;

    if(!resto)
        return resultado;
    else return (resultado + 1);
}

void printDoubleArray(double *array, int size){
    int i;
    for (i = 0; i < size; i++){
        printf("%d: %.2f| \n",i+1, array[i]);
        }
    printf("\n");
}

double *createEmptyDoubleArray(int length){
    int i;
    double *array = malloc(length * sizeof(double));
    for(i = 0; i < length; i++){
        array[i] = 0.0;
    }

    return array;
}

int *createEmptyIntArray(int length){
    int i;
    int *array = malloc(length * sizeof(int));
    for(i = 0; i < length; i++){
        array[i] = 0.0;
    }

    return array;
}
