#define MAX_SIZE 1000

//Quantidade maxima de individuos iniciais
#define MAX_OBJECTS 20
//Numero de solicitacoes
#define REQUESTS 4

//Numero de linhas que cada individuo ocupara na matriz
#define LINES_PER_SINGLE_OBJECT 7

//Numero de atributos de cada individuo
#define FEATURES_NUMBER REQUESTS * LINES_PER_SINGLE_OBJECT

//Numero maximo de linhas
#define MAX_LINES MAX_OBJECTS * LINES_PER_SINGLE_OBJECT

//Discretizacao da janela
//#define WINDOW_SIZE 5

//Tamanho do horizonte de agendamento
#define HORIZON_SIZE 50

// Numero de geracoes maximas
#define MAX_GENERATION 49

// Numero de individuos gerados a cada geracao
#define NEW_OBJECTS 1

// #define INPUT_FILE "POPULACAO_INICIAL - CASO OTIMO.txt"
#define INPUT_FILE "POPULACAO_INICIAL.txt"
// #define INPUT_FILE "TESTE_DA_FUNCAO_OBJETIVO.txt"
