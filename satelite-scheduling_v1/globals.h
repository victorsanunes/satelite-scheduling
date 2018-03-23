#define MAX_SIZE 1000

//Quantidade maxima de individuos iniciais
#define MAX_INDIVIDUALS 20
//Numero de solicitacoes
#define REQUESTS 4

//Numero de linhas que cada individuo ocupara na matriz
#define LINES_PER_SINGLE_INDIVIDUAL 7

//Numero de atributos de cada individuo
#define FEATURES_NUMBER REQUESTS * LINES_PER_SINGLE_INDIVIDUAL

//Numero maximo de linhas
#define MAX_LINES MAX_INDIVIDUALS * LINES_PER_SINGLE_INDIVIDUAL

//Discretizacao da janela
//#define WINDOW_SIZE 5

//Tamanho do horizonte de agendamento
#define HORIZON_SIZE 50

// Numero de geracoes maximas
#define MAX_GENERATION 10000
// Numero de individuos gerados a cada geracao
#define NEW_INDIVIDUALS 5

#define CROSSOVER_RATE 10.00

// #define INPUT_FILE "POPULACAO_INICIAL - CASO OTIMO.txt"
// #define INPUT_FILE "POPULACAO_INICIAL.txt"
// #define INPUT_FILE "TESTE_DA_FUNCAO_OBJETIVO.txt"
// #define INPUT_FILE "SUPER_POPULACAO.txt"
#define INPUT_FILE "POPULACAO.txt"

#define PRINTING_FLAG 0

FILE *f_fit;
FILE *f_csv;

int firstBestSolution;
clock_t time_begin, time_end;
double executionTime;
