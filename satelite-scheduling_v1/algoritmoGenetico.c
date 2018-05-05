#include "algoritmoGenetico.h"

int contador[20] = {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0};
int generation = 1;

int mutationFrequency = 0;
int crossoverFrequency = 0;

void printIndividualSummary(individualSummary *individuals, int length, FILE *f){
    int i;
    for(i = 0; i < length; i++){
        fprintf(f, "individuo[%d]=%.2f\n", individuals[i].index/LINES_PER_SINGLE_INDIVIDUAL, individuals[i].fitnessValue);
    }
}

void printIntArray(int *array, int size){
    int i;
    for (i = 0; i < size; i++){
        printf("%d ", array[i]);
        }
    printf("\n");
}

void printDoubleArray(double *array, int size){
    int i;
    for (i = 0; i < size; i++){
        printf("%d: %.4f| \n",i+1, array[i]);
        }
    printf("\n");
}

double truncateValue(int numerador, int denominador){
    double resultado = numerador/denominador;
    double resto = numerador % denominador;

    if(!resto)
        return resultado;
    else return (resultado + 1);
}

double signalQualityValue(int windowSize, int k){
    //double quality;
    //int k;
	if((k+1) <= truncateValue(windowSize, 2)){
		return ((k+1) * 1/truncateValue(windowSize, 2));
	}
	else{
		return (windowSize - k ) /truncateValue(windowSize, 2);
	}

    //return quality;
}

double fitnessFunction(	double *quality1,
						double *quality2,
						int window,
						int k_line,
						int request_beginning,
						int request_ending,
						int window_beginning,
						int window_ending){

	int weight = 1;
	double value = 0.0;

    //Checa se as restricoes estao sendo atendidas, se nao estiverem, zera os valores
	if(request_beginning < window_beginning || request_ending > window_ending || request_beginning >= request_ending){
        weight = 0;
		value = 0.0;
	}
	else if(window == 1){
			value += weight * quality1[k_line - 1];
		}
	else if(window == 2){
			value += weight * quality2[k_line - 1];
	}
	return value;
}

void calculateFitnessValues(double *fitnessValues,int size,
                            matrix population, int lines, int columns,
                            double *quality1_values,
                            double *quality2_values){
    int i, j, k, l;
    int window, k_line;
    int request_beginning, request_ending;
    int window_beginning, window_ending;
	int next_window, next_request_beginning, next_request_ending;
    int station, next_station;
    int satelite, next_satelite;
    int request_size;
    int penaltyFlag = 0;

    //Zera os valores do vetor
    for(i = 0; i < size; i++){
        fitnessValues[i] = 0;
    }

    i = 0;
    //Preenche o vetor com os valores da funcao de aptidao
	for(k = 0; k < size; k++){

        //Obtem as informacoes de cada solicitacao
		for(j = 0; j < columns; j++){
		    window                = population[i][j];
		    k_line                = population[i + 1][j];
		    request_beginning     = population[i + 2][j];
            request_ending        = population[i + 3][j];
            station               = population[i + 4][j];
            window_beginning      = population[i + 5][j];
            window_ending         = population[i + 6][j];
            // satelite              = population[i + 5][j];
            request_size = request_ending - request_beginning;

            //Checa a restrição de não preemptivadade: não pode ter mais de uma
            //solicitacao ao mesmo tempo para a mesma estacao.
            //Faz a checagem da solicitacao atual com todas as outras
            for(l = j + 1; l < columns; l++){
                next_window = population[i][l];
                next_request_beginning = population[i + 2][l];
                next_request_ending = population[i + 3][l];
                next_station = population[i + 4][l];
                // next_satelite = population[i + 5][l];
                // printf("Proxima comeco: %d\n", next_request_beginning);
                // printf("Estacao atual: %d\n", station);
                // printf("Proxima estacao: %d\n", next_station);
                // getchar();
                if(request_beginning >= window_beginning
                        && request_ending <= window_ending){
        			//Checa se a proxima solicitacao esta na mesma janela ou se a
                    // estacao eh a mesma
        			if(window == next_window){
                        // puts("MESMA JANELA");
                        // printf("mesmo janela\n");
                        // printf("request_beginning: %d\nrequest_ending: %d", request_beginning, request_ending);
                        // printf("\nnext_request_beginning: %d\nnext_request_ending: %d\n\n", next_request_beginning, next_request_ending);
                        // getchar();
                        if((request_ending >= next_request_beginning - 1) &&
                                (request_beginning <= next_request_ending + 1)){
                            // printf("PUNICAO\n");
                            penaltyFlag = 1; //Sera penalizado

                            // Sai do for
                            l = columns + 1;
                        }
        			}
                    else{
                        if(station == next_station){
                            // puts("MESMA ESTACAO");
                            if((request_ending >= next_request_beginning - 1) &&
                                    (request_beginning <= next_request_ending + 1)){
                                // printf("PUNICAO\n");
                                penaltyFlag = 1; //Sera penalizado

                                // Sai do for
                                l = columns + 1;
                            }
                        }
                    }
                }

                else{
                    penaltyFlag = 1;
                    l = columns + 1;
                }

            }

            if((k_line + window_beginning -1 >= window_ending ||
                    k_line + window_ending -1 <= window_beginning) && penaltyFlag != 1){
            // puts("POPULACAO INCOERENTE");
                penaltyFlag = 1;
                l = columns + 1;
            }

            if(penaltyFlag == 1){
                fitnessValues[k] = 0.00;
                penaltyFlag = 0;
                j = columns + 1;
                break;
            }

            else {
                // window_beginning 	= population[i + 5][j];
                // window_ending 		= population[i + 6][j];
                fitnessValues[k]    += fitnessFunction(	quality1_values,
                                                        quality2_values,
                                                        window, k_line,
                                                        request_beginning,
                                                        request_ending,
                                                        window_beginning,
                                                        window_ending);
                penaltyFlag = 0;
            }
        }

		i = i + LINES_PER_SINGLE_INDIVIDUAL;
    }
}

double getArrayValuesSum(double *v, int size){
    int i;
    double sum = 0.0;
    for(i = 0; i < size; i++){
        sum += v[i];
    }
    return sum;
}

void fillRoulette(double *roulette, double *fitnessValues, int length){
    int i;
    double sumFitnessValues = 0.0;
    sumFitnessValues = getArrayValuesSum(fitnessValues, length);
    for(i = 0; i < length; i++){
        roulette[i] = 100.0 * fitnessValues[i]/sumFitnessValues;
    }
}

int rouletteWheelSelection(double *roulette, int length, individualSummary *individuals){
    int indexIndividual, indexSelected;
    double probabilityValue, sumProbabilityValues;

    probabilityValue = (rand() % 10000) / 100.0;
    indexIndividual = 0;
    sumProbabilityValues = roulette[indexIndividual];
    indexSelected = 0;
    if(PRINTING_FLAG){
        puts("SOMA DAS PROBABILIDADES");
        printf("%.2f\n", sumProbabilityValues);
    }


    while(sumProbabilityValues < probabilityValue){
        indexSelected++;
        sumProbabilityValues += roulette[indexSelected];
        if(PRINTING_FLAG)
            printf("%.2f\n", sumProbabilityValues);
    }

    indexIndividual = indexSelected;
    // if(PRINTING_FLAG)
        // printf("\n\nResultado da roleta: %d\n", indexSelected);
    return indexSelected;

}

void insertIndividualsInPopulation(matrix population, matrix individual,
                                int firstLinePosition, int lines, int columns){
    int i, j, k;
    k = firstLinePosition;
    for(i = 0; i < lines; i++){
        for(j = 0; j < columns; j++){
            population[k][j] = individual[i][j];
        }
        k++;
    }
}

// selected1_matrixInsertionPosition eh a posicao de insercao do individuo
// na matriz da populacao original
void crossover(matrix originalPopulation, matrix newPopulation,
                int matrixIndex_1, int matrixIndex_2,
                int selected1_matrixInsertionPosition,
                int selected2_matrixInsertionPosition){

    crossoverFrequency++;
    matrix newIndividual_1 = alocateMatrix(LINES_PER_SINGLE_INDIVIDUAL, REQUESTS);
    matrix newIndividual_2 = alocateMatrix(LINES_PER_SINGLE_INDIVIDUAL, REQUESTS);

    matrix F1 = alocateMatrix(LINES_PER_SINGLE_INDIVIDUAL, REQUESTS);
    matrix P1 = alocateMatrix(LINES_PER_SINGLE_INDIVIDUAL, REQUESTS);
    matrix F2 = alocateMatrix(LINES_PER_SINGLE_INDIVIDUAL, REQUESTS);
    matrix P2 = alocateMatrix(LINES_PER_SINGLE_INDIVIDUAL, REQUESTS);

    int matrixIndex_1_aux = matrixIndex_1;
    int matrixIndex_2_aux = matrixIndex_2;

    // ====== COPIA OS INDIVIDUOS ORIGINAIS PARA AS MATRIZES AUXILIARES ========
    int i, j;
    for(i = 0; i < LINES_PER_SINGLE_INDIVIDUAL; i++){
        for(j = 0; j < REQUESTS; j++){
            P1[i][j] = originalPopulation[matrixIndex_1_aux][j];
            P2[i][j] = originalPopulation[matrixIndex_2_aux][j];
        }
        matrixIndex_1_aux++;
        matrixIndex_2_aux++;
    }

    // if(PRINTING_FLAG){
        // puts("ANTES DO CROSSOVER");
        // puts("INDIVIDUO 1");
        // printIntMatrix(P1, LINES_PER_SINGLE_INDIVIDUAL, REQUESTS, stdout);
        //
        // puts("INDIVIDUO 2");
        // printIntMatrix(P2, LINES_PER_SINGLE_INDIVIDUAL, REQUESTS, stdout);
    // }


    int request1, request2;
    request1 = (int)rand() % REQUESTS;
    // // request2 = (int)rand() % REQUESTS + 1;
    // // request1 = 2;
    // // request2 = 3;
    //
    // // ===== CRUZA AS COLUNAS 2 E 3 DE NEWINDIVIDUAL_1 E NEWINDIVIDUAL_2 =======
    // // crossMatrixSegment( newIndividual_1, 0, 1,
    // //                     newIndividual_2, 1, 3,
    // //                     LINES_PER_SINGLE_INDIVIDUAL, REQUESTS);
    int c, l, p;
    p = request1;
    for(c = 0; c <= p; c++){
    	for(l = 0; l < 7; l++){
    		F1[l][c] = P1[l][c];
    		F2[l][c] = P2[l][c];
    	}
    }


    for(c = (p+1); c < 4; c++){
    	for(l = 0; l < 7; l++){
    		F1[l][c] = P2[l][c];
    		F2[l][c] = P1[l][c];
    	}
    }


    // if(PRINTING_FLAG){
        // puts("DEPOIS DO CROSSOVER");
        // puts("INDIVIDUO 1");
        // printIntMatrix(F1, LINES_PER_SINGLE_INDIVIDUAL, REQUESTS, stdout);
        //
        // puts("INDIVIDUO 2");
        // printIntMatrix(F2, LINES_PER_SINGLE_INDIVIDUAL, REQUESTS, stdout);
        // getchar();
    // // }
    // printf("selected1_matrixInsertionPosition: %d\nselected2_matrixInsertionPosition: %d", selected1_matrixInsertionPosition, selected2_matrixInsertionPosition);
    // // =========== INSERE OS NOVOS INDIVIDUOS NA POPULACAO ORIGINAL ============
    insertIndividualsInPopulation(originalPopulation,
                                    F1,
                                    selected1_matrixInsertionPosition,
                                    LINES_PER_SINGLE_INDIVIDUAL,
                                    REQUESTS);

    insertIndividualsInPopulation(originalPopulation,
                                    F2,
                                    selected2_matrixInsertionPosition,
                                    LINES_PER_SINGLE_INDIVIDUAL,
                                    REQUESTS);
}

void mutation(matrix originalPopulation,
            int individualOriginalPosition,
            int lines, int columns,
            int insertionIndex){
    mutationFrequency++;
    matrix newIndividual_1 = alocateMatrix(LINES_PER_SINGLE_INDIVIDUAL, REQUESTS);
    int i, j, k, request, shift, option;
    int request1, request2;
    int aux1, aux2, aux3;
    k = individualOriginalPosition;

    // ===================== COPIA O INDIVIDUO ORIGINAL  =======================
    for(i = 0; i < lines; i++){
        for(j = 0; j < columns; j++){
            newIndividual_1[i][j] = originalPopulation[k][j];
        }
        k++;
    }

    // printf("VELHO INDIVIDUO\n");
    // printIntMatrix(newIndividual_1, LINES_PER_SINGLE_INDIVIDUAL, REQUESTS, stdout);

    // ==================== SORTEIA QUAL OPERACAO REALIZAR =====================
    // OPTION = 0: REALIZA O SHITF
    // OPTION = 1: TROCA O PONTO DE MEDIO ENTRE DUAS SOLICITACOES
    option = (int)rand() % 2;
    int deslocation;
    // if(option == 0){
        // =================== SORTEIA QUAL SOLICITACAO MEXER ======================
        request = (int)rand() % REQUESTS;

        // ====================== SORTEIA QUAL O SHIFT =============================
        shift = (int)rand() % 2;

        if(shift == 0)
            deslocation = -1;
        else
            deslocation = 1;

        // ========================= APLICA O SHIFT ==========================
        newIndividual_1[1][request] += deslocation;
        newIndividual_1[2][request] += deslocation;
        newIndividual_1[3][request] += deslocation;
    // }

    // else if(option == 1){
    //     // ================= SORTEIA QUAIS SOLICITACOES TROCAR =================
        // request1 = (int)rand() % REQUESTS;
        // request2 = (int)rand() % REQUESTS;
        //
        // // printf("request1: %d\nrequest2: %d", request1, request2);
        //
        // aux1 = newIndividual_1[1][request1];
        // aux2 = newIndividual_1[2][request1];
        // aux3 = newIndividual_1[3][request1];
        //
        // newIndividual_1[1][request1] = newIndividual_1[1][request2];
        // newIndividual_1[2][request1] = newIndividual_1[2][request2];
        // newIndividual_1[3][request1] = newIndividual_1[3][request2];
        //
        // newIndividual_1[1][request2] = aux1;
        // newIndividual_1[2][request2] = aux2;
        // newIndividual_1[3][request2] = aux3;
    // }

    // printf("NOVO INDIVIDUO (OPCAO: %d)\n", option);
    // printIntMatrix(newIndividual_1, LINES_PER_SINGLE_INDIVIDUAL, REQUESTS, stdout);
    // getchar();
    // ============= INSERE O NOVO INDIVIDUO NA POPULACAO ORIGINAL =============
    insertIndividualsInPopulation(originalPopulation,
                                    newIndividual_1,
                                    insertionIndex,
                                    LINES_PER_SINGLE_INDIVIDUAL,
                                    REQUESTS);

}

// Realiza o crossover ou mutacao e atualiza a populacao
void reproduction(int maxGeneration,
                    matrix originalPopulation,
                    matrix newPopulation,
                    int newIndividuals,
                    double *fitnessValues,
                    individualSummary *individualsArray,
                    double *quality1_values, double *quality2_values){

    // ======================== MONTA A ROLETA  ==========================
    // Roleta tem o tamanho da populacao
    double roulette[MAX_INDIVIDUALS];
    fillRoulette(roulette, fitnessValues, MAX_INDIVIDUALS);

    // ===================== GERA NOVOS INDIVUDUOS =================
    int newIndividualsCounter, selected1, selected2, matrixIndex;
    double probabilityValue;
    double crossoverRate = CROSSOVER_RATE;
    matrixIndex = 0;

    for(newIndividualsCounter = 0; newIndividualsCounter < newIndividuals; ){
        probabilityValue = rand() % 100 + 1;

        // ======================== CROSSOVER ==================================
        if(probabilityValue <= crossoverRate){
            if(newIndividualsCounter == newIndividuals-1){
                // puts("B");
                newIndividualsCounter += 1;
                selected1 = rouletteWheelSelection(roulette, MAX_INDIVIDUALS, individualsArray);
                selected2 = selected1;
            }
            else{
                newIndividualsCounter += 2;
                selected1 = rouletteWheelSelection(roulette, MAX_INDIVIDUALS, individualsArray);
                selected2 = rouletteWheelSelection(roulette, MAX_INDIVIDUALS, individualsArray);
                // while(selected1 == selected2){
                //     selected2 = rouletteWheelSelection(roulette, MAX_INDIVIDUALS, individualsArray);
                // }
            }
            if(PRINTING_FLAG){
                puts("SELECIONADOS");
                printf("%d e %d\n", selected1, selected2);
            }

            // ================ APLICA O OPERADOR DE CROSSOVER  ================
            crossover(originalPopulation, newPopulation,
                            individualsArray[selected1].index,
                            individualsArray[selected2].index,
                            individualsArray[matrixIndex].index,
                            individualsArray[matrixIndex+1].index);
            matrixIndex += 2;


        }
        // ========================= MUTACAO ===================================
        else{
            newIndividualsCounter += 1;
            selected1 = rouletteWheelSelection(roulette, MAX_INDIVIDUALS, individualsArray);
            // printf("Selecionado: %d\n", individualsArray[selected1].index);
            mutation(originalPopulation,
                    individualsArray[selected1].index,
                    LINES_PER_SINGLE_INDIVIDUAL, REQUESTS,
                    individualsArray[matrixIndex].index);
            matrixIndex++;
        }
    }
}

//Funcao que realizara a troca de variaveis
void swapIndividuals(individualSummary *vetor, int a, int b){
    individualSummary aux;
    aux = vetor[a];
    vetor[a] = vetor[b];
    vetor[b] = aux;
}

void sortIndividualsByFitnessValues(individualSummary *vetor, int size){
    int i, j;
    // puts("Entrou na ordenacao");
    for (i = 0; i < size; i++) {
        for(j = i + 1; j < size; j++){
            if(vetor[i].fitnessValue > vetor[j].fitnessValue ){
                swapIndividuals(vetor, i, j);
            }
        }
    }
    // puts("fim da ordenacao");
}

void copyFitnessValuesToIndividualStructure(double *fitnessArray, individualSummary *individualArray, int length){
    int i;
    for(i = 0; i < length; i++){
        individualArray[i].index = i * LINES_PER_SINGLE_INDIVIDUAL;
        individualArray[i].fitnessValue = fitnessArray[i];
    }
}

void copySortedFitnessValues(double *fitnessArray, individualSummary *individualArray, int length){
    int i;
    for(i = 0; i < length; i++){
        fitnessArray[i] = individualArray[i].fitnessValue;
    }
}

void runGenerations(int maxGeneration,
                    matrix originalPopulation,
                    matrix newPopulation,
                    int newIndividuals,
                    double *fitnessValues,
                    individualSummary *individualsArray,
                    double *quality1_values, double *quality2_values){

    firstBestSolution = -1;
    int generation;
    for(generation = 0; generation < MAX_GENERATION; generation++){

        // ================= CHAMA A REPRODUCAO ================
        time_begin += clock();
        reproduction(maxGeneration,
                    originalPopulation,
                    newPopulation,
                    newIndividuals,
                    fitnessValues,
                    individualsArray,
                    quality1_values, quality2_values);
        time_end += clock();
        // printf("NOVA POPULACAO\n");
        // printIntMatrix(originalPopulation, MAX_LINES, REQUESTS, stdout);

        // ============= CALCULA OS NOVOS VALORES DE APTIDAO ===================
        calculateFitnessValues(fitnessValues, MAX_INDIVIDUALS,
                                originalPopulation, MAX_INDIVIDUALS, REQUESTS,
                                quality1_values,
                                quality2_values);

        // =========== COPIA VALORES DE APTIDAO PARA ESTRUTURA AUXILIAR ============
        copyFitnessValuesToIndividualStructure(fitnessValues, individualsArray, MAX_INDIVIDUALS);

        // ================= ORDENA INDIVIDUOS PELO VALOR DE APTIDAO ===============
        sortIndividualsByFitnessValues(individualsArray, MAX_INDIVIDUALS);

        if(firstBestSolution == -1 &&
            individualsArray[MAX_INDIVIDUALS-1].fitnessValue >= (0.9 * 2.94) ){
            firstBestSolution = generation;
            printf("Individuo=%d\n", individualsArray[MAX_INDIVIDUALS-1].index/LINES_PER_SINGLE_INDIVIDUAL);
        }
        // =========== COPIA OS VALORES ORDENADOS PARA O VETOR DE APTIDAO ==========
        copySortedFitnessValues(fitnessValues, individualsArray, MAX_INDIVIDUALS);
        fprintf(f_fit, "%.2f\n", individualsArray[MAX_INDIVIDUALS-1].fitnessValue);
        //saveFitnessValues(f_fit, individualsArray, MAX_INDIVIDUALS);
        //saveFitnessValuesToCSV(f_csv, individualsArray, MAX_INDIVIDUALS);

    }
}

void saveFitnessValues(FILE *f, individualSummary *ind, int length){
    int i;
    //fprintf(f, "{\"fitness\":{");
    fprintf(f, "{");

    for(i = 0; i < length; i++){
        if(i < length-1){
            fprintf(f, "%i: %.2f, ",ind[i].index/LINES_PER_SINGLE_INDIVIDUAL,
                                    ind[i].fitnessValue);
        }
        else{
            fprintf(f, "%i: %.2f",ind[i].index/LINES_PER_SINGLE_INDIVIDUAL,
                                    ind[i].fitnessValue);
        }
    }
    fprintf(f, "}\n");
}

void saveFitnessValuesToCSV(FILE *f, individualSummary *ind, int length){
    int i, j;
    double array[length];
    //fprintf(f, "{\"fitness\":{");

    // ======================== IMPRIME O HEADER ==============================
    // for(i = 0; i < length; i++){
    //     fprintf(f, "%d", i);
    //     if(i < length-1){
    //         fprintf(f, ",");
    //     }
    // }
    // fprintf(f, "\n");

    // ================= COPIA OS VALORES NAS ORDENS CERTAS ====================
    for(i = 0; i < length; i++){
        array[ind[i].index/LINES_PER_SINGLE_INDIVIDUAL] = ind[i].fitnessValue;
    }
    for(i = 0; i < length; i++){
        fprintf(f, "%.2f", array[i]);
        if(i < length-1){
            fprintf(f, ",");
        }
    }
    fprintf(f, "\n");
}

void printStatistics(FILE *f){
    fprintf(f, "\n======== STATISTICS ==========\n");
    fprintf(f, "CROSSOVER: %d\n", crossoverFrequency);
    fprintf(f, "MUTACAO: %d\n", mutationFrequency);
    fprintf(f, "Primeiro otimo: %d\n", firstBestSolution);
    fprintf(f, "Tempo: %.2f\n", executionTime/CLOCKS_PER_SEC);
}

void printAlgorithmInformations(FILE *f){
    fprintf(f, "\n======== DADOS DO ALGORITMO ==========\n");
    fprintf(f, "CROSSOVER_RATE: %.2f\n", CROSSOVER_RATE);
    fprintf(f, "GERACOES: %d\n", MAX_GENERATION);
    fprintf(f, "NOVOS INDIVIDUOS POR GERACAO: %d\n", NEW_INDIVIDUALS);
}
