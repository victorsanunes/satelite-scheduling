def generateSingleObject(requestsNumber):
    singleObject = list()
    request = list()
    for i in range(requestsNumber):
        if(i <= 0):
            windowEnding = 30
            window = 1
        else:
            windowEnding = 31
            window = 2
        windowBeginning = 1
        request.append(window)

        k_line = input("k_linha: ")
        k_line = int(k_line)
        request.append(k_line)

        requestSize = input("Tamanho da solicitacao: ")
        requestSize = int(requestSize)
        if(requestSize % 2 == 0):
            requestBeginning = int(k_line - requestSize/2 + 1)
        else:
            requestBeginning = int(k_line + requestSize/2)
        requestEnding = int(k_line + requestSize/2)

        request.append(requestBeginning)
        request.append(requestEnding)

        if(i >= 2):
            station = 2

        else:
            station = 1
        request.append(station)

        if(i <= 0):
            windowEnding = 30
        else:
            windowEnding = 31

        windowBeginning = 1
        request.append(windowBeginning)
        request.append(windowEnding)

        singleObject.append(request)
        request = []

    return singleObject

# @param: populationSize --> number of objects
def generatePopulation(populationSize, requestsNumber):
    population = list()
    for i in range(populationSize):
        singleObject = generateSingleObject(requestsNumber)
        population.append(singleObject)
    return population

def convertPopulationIntoArray(population):
    finalPopulation = list()
    numberOfRequests = len(population[0])
    numberOfObjects = len(population)
    numberOfFeatures = len(population[0][0])

    print(repr(numberOfObjects) + "individuos")
    print(repr(numberOfRequests) + "solicitacoes")
    print(repr(numberOfFeatures) + "atributos")
    for x in range(len(population)):
        # Itera sobre os individuos
        z = 0
        for y in range(len(population[x])):
            finalPopulation.append(population[x][y][z])
            z = z + 1
    # print("FINAL: " + repr(finalPopulation))
    return finalPopulation

def saveListIntoTXTFile(array):
    filename = input("Informe o nome do arquivo txt: ")
    with open(filename, 'w') as file_handler:
        for item in array:
            file_handler.write(str(item) + '\n' )

def main():
    print("Gerador de populacao")

    # Numero de individuos, numero de solicitacoes1
    population = generatePopulation(2, 2)
    # population = list()
    # request = list()
    # singleObject = list()
    #
    # singleObject = [1, 4, 2, 1, 1, 30]
    # request.append(singleObject)
    # singleObject = [3, 2, 5, 1, 1, 31]
    # request.append(singleObject)
    # population.append(request)
    # request = []
    #
    # singleObject = [5, 3, 8, 1, 1, 30]
    # request.append(singleObject)
    # singleObject = [7, 4, 11, 1, 1, 31]
    # request.append(singleObject)
    # population.append(request)

    finalPopulation = convertPopulationIntoArray(population)
    # saveListIntoTXTFile(finalPopulation)
    print(repr(finalPopulation))
    print(repr(population[0]))
    print(repr(population[0][0]))
    print(repr(population[0][0][0]))

# ==============================================================================
main()
