MAX_SIZE = 1000

WINDOW = 0
KLINE = 1
REQUEST_BEGINNING = 2
REQUEST_ENDING = 3
STATION = 4
WINDOW_BEGINNING = 5
WINDOW_ENDING = 6

# Quantidade maxima de individuos iniciais
MAX_INDIVIDUALS = 20

# Numero de solicitacoes
REQUESTS = 4

# Numero de linhas que cada individuo ocupara na matriz
LINES_PER_SINGLE_INDIVIDUAL = 7

# Numero de atributos de cada individuo
FEATURES_NUMBER = REQUESTS * LINES_PER_SINGLE_INDIVIDUAL

# Numero maximo de linhas
MAX_LINES = MAX_INDIVIDUALS * LINES_PER_SINGLE_INDIVIDUAL

# Discretizacao da janela
WINDOW_SIZE = 5

# Tamanho do horizonte de agendamento
HORIZON_SIZE = 50

 # Numero de geracoes maximas
MAX_GENERATION = 10000

# Numero de individuos gerados a cada geracao
NEW_INDIVIDUALS = 5

CROSSOVER_RATE = 5.0

# INPUT_FILE "POPULACAO_INICIAL - CASO OTIMO.txt"
# INPUT_FILE "POPULACAO_INICIAL.txt"
# INPUT_FILE "TESTE_DA_FUNCAO_OBJETIVO.txt"
# INPUT_FILE "SUPER_POPULACAO.txt"

INPUT_FILE = "POPULACAO4.txt"
# INPUT_FILE = "teste_crossover.txt"

PRINTING_FLAG = 0
