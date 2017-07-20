#include "algoritmoGenetico.h"

int contador[20] = {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0};

int mutationFrequency = 0;
int crossoverFrequency = 0;

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
        printf("%d: %.2f| ",i+1, array[i]);
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
        // printf("restricao\n\n");
        // printf("request_beginning %d\n", request_beginning);
        // printf("window_beginning %d\n", window_beginning);
        weight = 0;
		value = 0.0;
	}
	else if(window == 1){
			// printf("\nq = %.2f\n", quality1[k_line - 1]);
			value += weight * quality1[k_line - 1];
		}
	else if(window == 2){
			// printf("\nq = %.2f\n", quality2[k_line - 1]);
			value += weight * quality2[k_line - 1];
	}


	//printf("sum: %.2f\n", value);
	return value;
}

void calculateFitnessValues(double *fitnessValues,int size,
                            matrix population, int lines, int columns,
                            double *quality1_values,
                            double *quality2_values){
    int i, j, k, l, m;
    int window, k_line, request_beginning, request_ending, window_beginning, window_ending;
	int next_window, next_request_beginning, next_request_ending;
    int station, next_station;
    int request_size;
    int penaltyFlag = 0;
    double temporaryFitnessValue = 0.0;

    //Zera os valores do vetor
    for(i = 0; i < size; i++){
        fitnessValues[i] = 0;
    }

    i = 0;
    m = 0;
    //Preenche o vetor com os valores da funcao de aptidao
	for(k = 0; k < size; k++){

        //Obtem as informacoes de cada solicitacao
		for(j = 0; j < columns; j++){
		    window                = population[i][j];
		    k_line                = population[i + 1][j];
		    request_beginning     = population[i + 2][j];
            request_ending        = population[i + 3][j];
            station               = population[i + 4][j];
            request_size = request_ending - request_beginning;

            //Checa a restrição de não preemptivadade: não pode ter mais de uma
            //solicitacao ao mesmo tempo para a mesma estacao.
            //Faz a checagem da solicitacao atual com todas as outras
            for(l = j + 1; l < columns; l++){
                next_window = population[i][l];
                next_request_beginning = population[i + 2][l];
                next_request_ending = population[i + 3][l];
                next_station = population[i + 4][l];

    			//Checa se a proxima solicitacao esta na mesma janela
    			if(window == next_window){
                    // printf("mesmo janela\n");
                    if((request_ending >= next_request_beginning - 1) && (request_beginning <= next_request_ending + 1)){
                        printf("preemptivadade\n");
                        penaltyFlag = 1; //Sera penalizado
                    }
    			}

                //Checa se a proxima solicitacao esta na mesma estacao
    			else if(station == next_station){
                    //Checa restricao de redundancia
                    if((request_ending >= next_request_beginning - 1) && (request_beginning <= next_request_ending + 1)){
                        printf("redundancia\n");
                        penaltyFlag = 1; //Sera penalizado
                    }
    			}
            }

            if(penaltyFlag == 1){
                // PENALIDADE
                // printf("PENALIDE MAXIMA\n");
                fitnessValues[k] += -1.00;
                penaltyFlag = 0;
            }

            else {
                // printf("entrou no penaltyFlag\n" );
                window_beginning 	= population[i + 5][j];
                printf("window_beginning 1: %d\n", window_beginning);
                window_ending 		= population[i + 6][j];
                m++;
                if(m >= REQUESTS)
                    m = 0;
                fitnessValues[k]    += fitnessFunction(	quality1_values,
                                                        quality2_values,
                                                        window, k_line,
                                                        request_beginning,
                                                        request_ending,
                                                        window_beginning,
                                                        window_ending);
                // printf("f: %.2f\n", fitnessFunction(	quality1_values,
                //                                         quality2_values,
                //                                         window, k_line,
                //                                         request_beginning,
                //                                         request_ending,
                //                                         window_beginning,
                //                                         window_ending));
                penaltyFlag = 0;
            }
        }
		// printf("%d\n", penaltyFlag);
		i = i + LINES_PER_SINGLE_OBJECT;
    }
}





double calculateSingleFitnessValue(matrix object, int lines, int columns, double *quality1_values, double *quality2_values){
    int i, j, k, l;
    int window, k_line, request_beginning, request_ending, window_beginning, window_ending;
    int interval, next_window, next_request_beginning, next_request_ending;
    int request_size;
    double fitnessSingleValue = 0.0;


    for(k = 0; k < lines; k++){
		//Percorre a matriz do individuo
		for(j = 0; j < columns; j++){
		    window                = object[i][j];
		    k_line                = object[i + 1][j];
		    request_beginning     = object[i + 2][j];
            request_ending        = object[i + 3][j];
            request_size = request_ending - request_beginning;

            //Checa a restrição de não preemptivadade: não pode ter mais de uma
            //solicitacao ao mesmo tempo para a mesma estacao.
            //Faz a checagem da solicitacao atual com todas as outras
            for(l = j + 1; l < columns; l++){
                next_window = object[i][l];
                next_request_beginning = object[i + 2][l];
                next_request_ending = object[i + 3][l];
                // printf("Pa: %d   Pp: %d\n", request_beginning, next_request_beginning);
    			//Checa se a proxima solicitacao esta na mesma janela
    			if(window == next_window){
                    // printf("mesmo janela\n");
                    if((request_ending >= next_request_beginning - 1) && (request_beginning <= next_request_ending + 1)){
                        // printf("preemptivadade\n");
                        interval = -1; //Sera penalizado
                    }
                    else{
                        interval = 2;
                    }
    			    // else{
                    //     interval = abs(next_request_beginning - request_ending);
                    // }
    			}

    			else {
                    if((request_ending >= next_request_beginning - 1) && (request_beginning <= next_request_ending + 1)){
                        // printf("redundancia\n");
                        interval = -1; //Sera penalizado
                    }
                    else{
                        interval = 2;
                    }
    			}
                //Testa se houve um salto no agendamento entre uma solicitacao e outra

    		}
            if(interval >= 2){
                // printf("entrou no interval\n" );
                window_beginning 	= object[i + 4][j];
                window_ending 		= object[i + 5][j];
                fitnessSingleValue 	+= fitnessFunction(	quality1_values,
                                                        quality2_values,
                                                        window, k_line,
                                                        request_beginning,
                                                        request_ending,
                                                        window_beginning,
                                                        window_ending);
            }
            else {
                fitnessSingleValue += -0.00;
            }
        }
	}
    return fitnessSingleValue;

}

double getArrayValuesSum(double *v, int size){
    int i;
    double sum;
    for(i = 0; i < size; i++){
        sum += v[i];
    }
    return sum;
}

int rouletteWheelSelection(int population_size, double *roulette, objectSummary *objects){
    int index_object;
    double probability_value, sum_probability_values;
    int index_selected;

    //Seleciona um valor de probabilidade para escolher o individuo
    probability_value = (rand() % 10000) / 100.0; //Valores reais entre 0.00 e 99.99
    // printf("valor sorteado (roleta) = %lf\n", probability_value);

    index_object = 0;
    sum_probability_values = roulette[index_object];

    //Itera até encontrar o individuo sorteado
    while(sum_probability_values < probability_value){
        index_object++;
        sum_probability_values += roulette[index_object];
    }
    //Melhorar com busca binaria

    index_selected = index_object;
    contador[objects[index_selected].index/LINES_PER_SINGLE_OBJECT]++;
    return index_selected;
}

void updatePopulation(matrix population, matrix new_population,
                        int linesPerSingleObject, int columns,
                        int new_objects, objectSummary *objects) {

    int i, j, k, firstLinePosition, l;
    l = 0;
    //Itera sobre o vetor de indices para calcular a posicao de insercao na matriz de populacao
    for(i = 0; i < new_objects; i++){
        //Posicao de insercao do i-esimo novo individuo na populacao
        firstLinePosition = objects[i].index;

        //Insere o novo individuo na matriz na posicao correta
        for(j = firstLinePosition; j < firstLinePosition + linesPerSingleObject; j++){
            for(k = 0; k < columns; k++){
                population[j][k] = new_population[l][k];
            }
            l++;
        }
    }
}

// new_population: matriz auxiliar com individuos criados a cada geracao
// index_selected1: Eh o indice do vetor populationInsertionIndex
// multiplicado pelo LINES_PER_SINGLE_OBJECT
// matrix_index1 eh o indice do individuo na populacao original
void crossover(matrix population, matrix new_population,
                int matrix_index1, int matrix_index2,
                int index_selected1, int index_selected2){
    crossoverFrequency++;
    matrix single_new_object1 = alocateMatrix(LINES_PER_SINGLE_OBJECT, REQUESTS);
    matrix single_new_object2 = alocateMatrix(LINES_PER_SINGLE_OBJECT, REQUESTS);

    int i, j;
    int index1 = matrix_index1;
    int index2 = matrix_index2;

    //Copia os individuos originais para as estruturas auxiliares
    for(i = 0; i < LINES_PER_SINGLE_OBJECT; i++){
        for(j = 0; j < REQUESTS; j++){
            single_new_object1[i][j] = population[index1][j];
            single_new_object2[i][j] = population[index2][j];
        }
        index1++;
        index2++;
    }

    //Cruza as colunas 2 e 3 de single_new_object1 e single_new_object2
    crossMatrixSegment( single_new_object1, 2, 3,
                        single_new_object2, 2, 3,
                        LINES_PER_SINGLE_OBJECT, REQUESTS);

    // Insere novos objetos na populacao
    insertObjectInPopulation(new_population, single_new_object1, index_selected1, LINES_PER_SINGLE_OBJECT, REQUESTS);
    insertObjectInPopulation(new_population, single_new_object2, index_selected2, LINES_PER_SINGLE_OBJECT, REQUESTS);
}

// index: posicao de insercao na matriz auxiliar. Eh o indice do vetor
// populationInsertionIndex multiplicado pelo LINES_PER_SINGLE_OBJECT
// matrix_index eh a posicao do individuo na populacao original
void mutation(matrix population, matrix new_population, int lines, int columns, int matrix_index, int index){
    mutationFrequency++;
    int i, j, k;
    k = matrix_index;

    //Faz a copia do individuo original
    for(i = index; i < index + lines; i++){
        for(j = 0; j < REQUESTS; j++){
            new_population[i][j] = population[k][j];
        }
        k++;
    }

    //Insere os valores do novo individuo na populacao deslocando para direita
    for (i = 0; i < REQUESTS; i++){
        new_population[index + 1][i] += 1;
        new_population[index + 2][i] += 1;
        new_population[index + 3][i] += 1;
    }
}

//Insere objeto novo na matriz de populacao
// firstLinePosition: posicao da primera linha na matriz de populacao
void insertObjectInPopulation(matrix population, matrix object, int firstLinePosition, int lines, int columns){
    int i, j, k;
    k = firstLinePosition;
    for(i = 0; i < lines; i++){
        for(j = 0; j < columns; j++){
            population[k][j] = object[i][j];
        }
        k++;
    }
}

//new_objects: quantidade de novos objetos criados a cada geracao
// fitnessValues: Vetor de aptidoes
// population: matriz de populacao
// quality1_values: qualidade do sinal da janela 1
// quality2_values: qualidade do sinal da janela 2
// objects: vetor ordenado com a aptidao e a posicao inicial na populacao
//new_population: matriz de novos individuos gerados
void reproduction(int population_size, int new_objects,
                    double *fitnessValues, matrix population, matrix new_population,
                    double *quality1_values, double *quality2_values,
                    objectSummary *objects){

    int index_new_object, index_object;
    double *roulette;
    int probability_value;
    int crossover_rate;
    double sum_fitness_values;
    int index_selected1, index_selected2;
    int i, j, k;
    int window, k_line, request_beginning, request_ending, window_beginning, window_ending;


    // Vetor com o indice de insercao dos individuos na matriz de populacao
    int populationInsertionIndex[new_objects];
    // double *newObjectsFitnessValues = (double*)malloc(new_objects * sizeof(double));

    //Parametros do algoritmo genetico
    crossover_rate = 90;

    //Montagem da roleta
    roulette = (double*)malloc(population_size * sizeof(double));
    sum_fitness_values = 0.0;
    sum_fitness_values = getArrayValuesSum(fitnessValues, population_size);
    // printf("Somatorio de aptidoes: %lf\n", sum_fitness_values);

    //Calcula as probabilidades para cada individuo
    for(index_object = 0; index_object < population_size; index_object++){
        roulette[index_object] = 100.0 * fitnessValues[index_object]/sum_fitness_values;

    }

    //Contador de individuos gerados
    i = 0;

    // Indice do vetor populationInsertionIndex
    j = 0;
    while(i < new_objects){

        // Gera valores entre 1 e 100
        probability_value = rand() % 100 + 1;
        // printf("Proabilidade: %d%%\n", probability_value);

        if(probability_value <= crossover_rate){
            // printf("Método: Crossover\n");

            // Quantidade de individuos gerados no crossover
            i += 2;

            //Sorteia os objetos
            index_selected1 = rouletteWheelSelection(population_size, roulette, objects);
            index_selected2 = rouletteWheelSelection(population_size, roulette, objects);

            while(index_selected1 == index_selected2){
                index_selected2 = rouletteWheelSelection(population_size, roulette, objects);
            }

            // Trata caso em que crossover foi escolhido podendo ser gerado apenas
            // mais 1 individuo
            if(i > new_objects){

                index_selected2 = index_selected1;
                crossover(population, new_population,
                            objects[index_selected1].index, objects[index_selected2].index,
                            (j * LINES_PER_SINGLE_OBJECT), (j * LINES_PER_SINGLE_OBJECT) );

                populationInsertionIndex[j] = objects[index_selected1].index;
                populationInsertionIndex[j] = objects[index_selected2].index;
                j++;


            }
            else{
                crossover(population, new_population,
                        objects[index_selected1].index, objects[index_selected2].index,
                        (j * LINES_PER_SINGLE_OBJECT), ((j+1) * LINES_PER_SINGLE_OBJECT) );

                populationInsertionIndex[j] = objects[index_selected1].index;
                j++;
                populationInsertionIndex[j] = objects[index_selected2].index;
                j++;
                    // printf("Individuo 1 selecionado: %d\n", objects[index_selected1].index/LINES_PER_SINGLE_OBJECT);
                    // printf("Individuo 2 selecionado: %d\n", objects[index_selected2].index/LINES_PER_SINGLE_OBJECT);
            }
        }
        else{
            // printf("Método: Mutacao\n" );
            i += 1;
            index_selected1 = rouletteWheelSelection(population_size, roulette, objects);
            // printf("Individuo 1 selecionado: %d\n", objects[index_selected1].index/LINES_PER_SINGLE_OBJECT);
            mutation(population, new_population,
                    LINES_PER_SINGLE_OBJECT, REQUESTS,
                    objects[index_selected1].index,
                    (j * LINES_PER_SINGLE_OBJECT));

            populationInsertionIndex[j] = objects[index_selected1].index;
            j++;
        }
    }

    // printf("\n\n5 OBJECTS\n");
    // printIntMatrix(new_population, NEW_OBJECTS * LINES_PER_SINGLE_OBJECT, REQUESTS);

    //Insere novos individuos na populacao na posicao dos N piores
    updatePopulation(population, new_population,
                            LINES_PER_SINGLE_OBJECT, REQUESTS,
                            new_objects, objects);

    //Calcula a aptidao da nova populacao gerada
    calculateFitnessValues(fitnessValues, MAX_OBJECTS,
                            population, MAX_OBJECTS, REQUESTS,
                            quality1_values,
                            quality2_values);

    // printf("\nNOVO FITNESS\n");
    // printDoubleArray(fitnessValues, MAX_OBJECTS);

    free(roulette);
}

void imprimeContador(objectSummary *objects){
    int i;
    printf("\n\nContador\n");
    for(i = 0; i < MAX_OBJECTS; i++){
        printf("individuo %d: %d |\n", objects[i].index/LINES_PER_SINGLE_OBJECT, contador[i]);
    }
    // printIntArray(contador, MAX_OBJECTS);
}

void printStatistics(){
    printf("%d mutacoes\n", mutationFrequency);
    printf("%d crossover\n", crossoverFrequency);
}
