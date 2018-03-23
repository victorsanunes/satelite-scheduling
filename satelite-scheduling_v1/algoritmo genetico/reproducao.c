#include <stdlib.h>
#include <stdio.h>

int populacao[40][4]; // Composta por 20 individuos. Cada individuo tem 2 linhas (indice da janela e ponto medio k_linha) e 4 colunas (solicitacoes).
int novos_individuos[10][4]; // Composta por 5 individuos.
int contador[20] = {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0};


int Selecao_Roleta(int tamanho_populacao, double *roleta)
{
    int indice_individuo; //comecando em zero.
    double somatorio_probabilidades;
    double valor_probabilidade;
    int indice_selecionado; // comecando em zero.

    valor_probabilidade = (rand()%10000)/100.0; //Valores reais entre 0.00 e 99.99.
    printf("valor sorteado = %lf\n",valor_probabilidade);

    indice_individuo = 0;
    somatorio_probabilidades = roleta[indice_individuo];
    while(somatorio_probabilidades < valor_probabilidade)
    {
        indice_individuo++;
        somatorio_probabilidades = somatorio_probabilidades + roleta[indice_individuo];
    }
    indice_selecionado = indice_individuo;
    // obs.: pode ser melhorado com busca binaria.

    contador[indice_selecionado] = contador[indice_selecionado] + 1;

    return indice_selecionado;
}


void Operador_Crossover(int tamanho_populacao,int indice_selecionado_1,int indice_selecionado_2)
{

}


void Operador_Mutacao(int tamanho_populacao,int indice_selecionado_1)
{

}


void Reproducao(int tamanho_populacao, int numero_novos_individuos, double *aptidoes)
{
    int indice_novo_individuo, indice_individuo;
    int indice_pai_1, indice_pai_2;
    double *roleta;
    int valor_probabilidade;
    int taxa_crossover;
    double somatorio_aptidoes;
    int indice_selecionado_1, indice_selecionado_2;

    // Especificacao de parametros dp algoritmo genetico
    taxa_crossover = 90;

    /////////////////////////////////////////////////////////////
    // Montagem da roleta
    /////////////////////////////////////////////////////////////
    roleta = (double*)malloc(tamanho_populacao*sizeof(double));
    somatorio_aptidoes = 0.0;
    for(indice_individuo=0;indice_individuo<tamanho_populacao;indice_individuo++)
    {
        somatorio_aptidoes = somatorio_aptidoes + aptidoes[indice_individuo];
    }

    printf("somatorio_aptidoes = %lf\n",somatorio_aptidoes);

    for(indice_individuo=0;indice_individuo<tamanho_populacao;indice_individuo++)
    {
        roleta[indice_individuo] = 100.0*aptidoes[indice_individuo]/somatorio_aptidoes;
    }
    ///////////////////////////////////////////////////////////////

    for(indice_novo_individuo=1;indice_novo_individuo<=numero_novos_individuos;indice_novo_individuo++)
    {
        printf("indice_novo_individuo = %d\n",indice_novo_individuo);

        // Selecao do tipo de operador de reproducao
        valor_probabilidade = rand()%100 + 1; // Valores inteiros entre 1 e 100.

        if(valor_probabilidade <= taxa_crossover)
        {
            // Operacao de crossover selecionada
            printf("crossover\n");

            // Selecao de individuos para a reproducao
            indice_selecionado_1 = Selecao_Roleta(tamanho_populacao, roleta);
            indice_selecionado_2 = Selecao_Roleta(tamanho_populacao, roleta);
            while(indice_selecionado_2 == indice_selecionado_1)
            {
                indice_selecionado_2 = Selecao_Roleta(tamanho_populacao, roleta);
            }

            Operador_Crossover(tamanho_populacao,indice_selecionado_1,indice_selecionado_2);

            printf("indice_selecionado_1 = %d\n",indice_selecionado_1);
            printf("indice_selecionado_2 = %d\n",indice_selecionado_2);
        }
        else
        {
            // Operacao de mutacao selecionada
            printf("mutacao\n");

            // Selecao de individuo para a reproducao
            indice_selecionado_1 = Selecao_Roleta(tamanho_populacao, roleta);

            Operador_Mutacao(tamanho_populacao,indice_selecionado_1);

            printf("indice_selecionado_1 = %d\n",indice_selecionado_1);
        }

        printf("--------------------------------------------\n");

    }

    free(roleta);
}

int main()
{
    int i;
    double aptidoes[20] = {0.5 , 7.2 , 45 , 2 , 3 , 1 , 10 , 20 , 0.8 , 3.3 , 8.1 , 5 , 7 , 6 , 2.5 , 1.5 , 1.2 , 9 , 2.2 , 4.2};
                          //1     2     3   4   5   6    7    8    9     10    11   12  13  14   15    16    17  18   19    20
    //srand(time(NULL)); // sem semente -> gera uma nova sequencia aleatoria, sempre que eh executado.

    srand(1); // semente 1 -> gera uma mesma sequencia aleatoria para a semente 1, sempre que eh executado.

    for(i=0;i<1000;i++)
    {
        Reproducao(20,5,aptidoes);
        //getchar();
    }

    printf("contagem dos selecionados = ");
    for(i=0;i<20;i++)
    {
        printf("%d ",contador[i]);
    }
    printf("\n");


    getchar();

    return 0;
}

