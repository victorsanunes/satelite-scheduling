import globals
import fileHelper as fh
import heapsort
import random

class GeneticAlgorithm(object):
    def truncateValue(self, numerador, denominador):
        resultado = numerador/denominador
        resto = numerador % denominador

        if(resto != 1.0):
            return resultado
        else:
            return (resultado + 1)

    def setSignalQualityValue(self, index, size):
        if((index+1) <= round(size/2) ):
            return ((index+1) * 1/round(size/2))
        else:
            return (size - index ) / round(size/2)

    def calculateQualityValues(self, size):
        quality = {x: self.setSignalQualityValue(x, size) for x in range(size)}
        return quality

    def fitnessFunction(self, ind, id_req):
        weight = 1.0
        value = 0.0

        if(ind.getReqBegin(id_req) < ind.getWindowBegin(id_req) or
            ind.getReqEnd(id_req) > ind.getWindowEnd(id_req) or
            ind.getReqBegin(id_req) >= ind.getReqEnd(id_req)):
            weight = 0.0
            value = 0.0
        elif(ind.getWindow(id_req) == 1):
            value += weight * self.quality1[ind.getKline(id_req)-1]

        elif(ind.getWindow(id_req) == 2):
            value += weight * self.quality2[ind.getKline(id_req)-1]

        return value

    def calculateFitnessValues(self, ind, quality1, quality2):

        fitnessValue = 0.0
        penaltyFlag = 0
        sum = 0.0
        req = 0
        # Itera para cada uma das solicitacoes
        while req < globals.REQUESTS:
            window = ind.getWindow(req)
            kline = ind.getKline(req)
            reqBegin = ind.getReqBegin(req)
            reqEnd = ind.getReqEnd(req)
            windowBegin = ind.getWindowBegin(req)
            windowEnd = ind.getWindowEnd(req)
            station = ind.getStation(req)

            # Itera para cada uma das proximas solicitacoes
            req2 = req+1
            while(req2 < globals.REQUESTS):
                next_window = ind.getWindow(req2)
                next_kline = ind.getKline(req2)
                next_reqBegin = ind.getReqBegin(req2)
                next_reqEnd = ind.getReqEnd(req2)
                next_windowBegin = ind.getWindowBegin(req2)
                next_windowEnd = ind.getWindowEnd(req2)
                next_station = ind.getStation(req2)

                # print("Request: " + str(req) + " Next_Request: " + str(req2))
                # print("window: " + str(window) + " next_window: " + str(next_window))

                # Checa se a alocacao da solicitacao esta dentro dos limites
                # da janela
                if(reqBegin >= windowBegin and reqEnd <= windowEnd):
                    # Caso as solicitacoes estejam na mesma janela eh preciso
                    # checar preemptivadade e redundancia
                    if(next_window == window):
                        if((reqEnd >= next_reqBegin-1) and (reqBegin <= next_reqEnd+1)):
                            # print(">>>>>same window penalty")
                            penaltyFlag = 1
                            # Atribuicao para sair do laco
                            req2 = globals.REQUESTS
                    else:
                        if(station == next_station):
                            if((reqEnd >= next_reqBegin-1) and (reqBegin <= next_reqEnd+1)):
                                # print(">>>>>same station penalty")
                                penaltyFlag = 1
                                req2 = globals.REQUESTS
                else:
                    # print("Solicitacao fora dos limites")
                    penaltyFlag = 1
                    req2 = globals.REQUESTS
                    # break

                req2 += 1
            # ----------------------------------------
            if(((kline + windowBegin-1 >= windowEnd)
                or (kline + windowEnd-1 <= windowBegin)) and penaltyFlag == 0):
                penaltyFlag = 1
                req = globals.REQUESTS
                # break

            if(penaltyFlag == 1):
                sum = 0.0
                penaltyFlag = 0
                req = globals.REQUESTS
                # break

            else:
                sum += self.fitnessFunction(ind, req)
                penaltyFlag = 0

            req = req + 1
        return sum

    def printQualityArray(self):
        q1 = self.getQualityArray()[0]
        q2 = self.getQualityArray()[1]

        for k in q1:
            print(repr(k) + ": "+"{0:.2f}".format(q1[k]))
        print("\n")
        for k in q2:
            print(repr(k) + ": "+"{0:.2f}".format(q2[k]))

    def getQualityArray(self):
        return [self.quality1, self.quality2]

    def crossover(self, selected1, selected2, replace1, replace2):
        '''
        Implementacao do crossover basico

        Ele ja atualiza os novos individuos gerados na populacao,
        matando os individuos com menor fitness

        selected1: individuo que foi selecionado
        replace1: individuo para ser substituido
        '''
        cut_point = 2
        for req in range(cut_point):
            replace1.setRequest(reqID = req,
                                window=selected1.getWindow(req),
                                kline=selected1.getKline(req),
                                reqBegin=selected1.getReqBegin(req),
                                reqEnd=selected1.getReqEnd(req),
                                station=selected1.getStation(req),
                                windowBegin=selected1.getWindowBegin(req),
                                windowEnd=selected1.getWindowEnd(req))

            replace2.setRequest(reqID = req,
                                window=selected2.getWindow(req),
                                kline=selected2.getKline(req),
                                reqBegin=selected2.getReqBegin(req),
                                reqEnd=selected2.getReqEnd(req),
                                station=selected2.getStation(req),
                                windowBegin=selected2.getWindowBegin(req),
                                windowEnd=selected2.getWindowEnd(req))

        for req in range(cut_point, globals.REQUESTS):
            replace1.setRequest(reqID = req,
                                window=selected2.getWindow(req),
                                kline=selected2.getKline(req),
                                reqBegin=selected2.getReqBegin(req),
                                reqEnd=selected2.getReqEnd(req),
                                station=selected2.getStation(req),
                                windowBegin=selected2.getWindowBegin(req),
                                windowEnd=selected2.getWindowEnd(req))

            replace2.setRequest(reqID = req,
                                window=selected1.getWindow(req),
                                kline=selected1.getKline(req),
                                reqBegin=selected1.getReqBegin(req),
                                reqEnd=selected1.getReqEnd(req),
                                station=selected1.getStation(req),
                                windowBegin=selected1.getWindowBegin(req),
                                windowEnd=selected1.getWindowEnd(req))

    def mutation(self, selected1, replace1):
        req_selected = random.randint(0, globals.REQUESTS-1)
        option =  random.randint(0, 1)

        # Copia o individuo original
        for req in range(globals.REQUESTS):
            replace1.setRequest(reqID = req,
                                window=selected1.getWindow(req),
                                kline=selected1.getKline(req),
                                reqBegin=selected1.getReqBegin(req),
                                reqEnd=selected1.getReqEnd(req),
                                station=selected1.getStation(req),
                                windowBegin=selected1.getWindowBegin(req),
                                windowEnd=selected1.getWindowEnd(req))

        if(option == 1):
            replace1.setKline(req_selected, replace1.getKline(req_selected)+1)
            replace1.setReqBegin(req_selected, replace1.getReqBegin(req_selected)+1)
            replace1.setReqEnd(req_selected, replace1.getReqEnd(req_selected)+1)

        else:
            replace1.setKline(req_selected, replace1.getKline(req_selected)-1)
            replace1.setReqBegin(req_selected, replace1.getReqBegin(req_selected)-1)
            replace1.setReqEnd(req_selected, replace1.getReqEnd(req_selected)-1)

    def buildRoulette(self, fitnessValues):
        sum_fit = float(sum(fitnessValues))

        roulette = dict()
        for i in range(globals.MAX_INDIVIDUALS):
            if(sum_fit > 0):
                roulette[i] = 100 * fitnessValues[i]/sum_fit
            else:
                roulette[i] = 0.0
        return roulette

    def rouletteWheelSelection(self, roulette):
        '''
        Funcao que implementa a selecao por roleta

        Retorna o ID do individuo selecionado
        '''
        probabilityValue = random.randint(0, 10000) / 100.0
        sum_probability = roulette[0]
        selected = 0

        while sum_probability < probabilityValue \
                and selected < globals.MAX_INDIVIDUALS-1:
            selected += 1
            sum_probability += roulette[selected]
        return selected

    def __init__(self, crossoverRate):
        self.quality1 = self.calculateQualityValues(30)
        self.quality2 = self.calculateQualityValues(31)
        self.crossoverRate = crossoverRate

class Individual(object):
    def __init__(self, requests, indID):
        self.features = {x: dict() for x in range(requests)}
        self.fitness = 0.0
        self.indID = indID

    # ----------------------------- CALCULATIONS -------------------------------
    def calculateFitnessValue(self):
        ga = GeneticAlgorithm(globals.CROSSOVER_RATE)
        self.setFitnessValue(ga.calculateFitnessValues(self, ga.quality1, ga.quality2))

    # ------------------------------ PRINTERS ---------------------------------
    def printIndividual(self):
        for k in self.features:
            print("SOLICITACAO: " + str(k))
            print(self.features[k])
        print("Fitness: " + str(self.fitness))

    def printRequest(self, reqID):
        print(repr(self.features[reqID]["window"]))
        print(repr(self.features[reqID]["kline"]))
        print(repr(self.features[reqID]["reqBegin"]))
        print(repr(self.features[reqID]["reqEnd"]))
        print(repr(self.features[reqID]["station"]))
        print(repr(self.features[reqID]["windowBegin"]))
        print(repr(self.features[reqID]["windowEnd"]))

    # ------------------------------ SETTERS -----------------------------------
    def setRequest(self,reqID,
                    window, kline, reqBegin, reqEnd,
                    station, windowBegin, windowEnd):
        self.features[reqID]["window"] = window
        self.features[reqID]["kline"] = kline
        self.features[reqID]["station"] = station
        self.features[reqID]["reqBegin"] = reqBegin
        self.features[reqID]["reqEnd"] = reqEnd
        self.features[reqID]["windowBegin"] = windowBegin
        self.features[reqID]["windowEnd"] = windowEnd

    def setWindow(self, reqID, window):
        self.features[reqID]["window"] = int(window)

    def setKline(self, reqID, kline):
        self.features[reqID]["kline"] = int(kline)

    def setReqBegin(self, reqID, reqBegin):
        self.features[reqID]["reqBegin"] = int(reqBegin)

    def setReqEnd(self, reqID, reqEnd):
        self.features[reqID]["reqEnd"] = int(reqEnd)

    def setStation(self, reqID, station):
        self.features[reqID]["station"] = int(station)

    def setWindowBegin(self, reqID, windowBegin):
        self.features[reqID]["windowBegin"] = int(windowBegin)

    def setWindowEnd(self, reqID, windowEnd):
        self.features[reqID]["windowEnd"] = int(windowEnd)

    def setFitnessValue(self, value):
        self.fitness = value

    def setID(self, value):
        self.indID = value

    # ---------------------- GETTERS --------------------------------
    def getWindow(self, reqID):
        return self.features[reqID]["window"]

    def getKline(self, reqID):
        return self.features[reqID]["kline"]

    def getReqBegin(self, reqID):
        return self.features[reqID]["reqBegin"]

    def getReqEnd(self, reqID):
        return self.features[reqID]["reqEnd"]

    def getStation(self, reqID):
        return self.features[reqID]["station"]

    def getWindowBegin(self, reqID):
        return self.features[reqID]["windowBegin"]

    def getWindowEnd(self, reqID):
        return self.features[reqID]["windowEnd"]

    def getIndividualID(self):
        return self.indID

    def getFitnessValue(self):
        return self.fitness

class Population(object):
    # def fillPopulation(self, dataset):

    def __init__(self):
        self.population = {x: Individual(globals.REQUESTS, x) for x in range(globals.MAX_INDIVIDUALS)}
        self.sortedPopulationIndexes = list(self.population.keys())
        self.GA = GeneticAlgorithm(globals.CROSSOVER_RATE)
        self.fitnessArray = None
        self.roulette = None

    def savePopulationFitnessToFile(self):
        array = [0.0 for x in range(globals.MAX_INDIVIDUALS)]
        for k in self.sortedPopulationIndexes:
            array[k] = float(self.fitnessArray[k])

        with open("fitness.csv", 'a') as file_handler:
            for item in array:
                file_handler.write(str(item) + ',' )
            file_handler.write('\n')

    def printPopulation(self):
        for k in self.sortedPopulationIndexes:
            print("Individuo: " + str(k))
            self.population[k].printIndividual()
            print("--------------------------------------------")

    def printPopulationFitness(self):
        for k in self.population:

            ind = self.getIndividual(k)
            fitness = ind.getFitnessValue()
            print("Individuo[" + str(k) + "]: " + str(fitness))

    def getIndividual(self, indID):
        return self.population[indID]

    def calculatePopulationFitness(self):
        for k in self.population:
            ind = self.getIndividual(k)
            ind.calculateFitnessValue()

    def sortPopulationByFitness(self):
        # Dicionario auxiliar para armazenar o id do individuo e seu fitness
        fitnessArray = dict()
        for k in self.population:
            ind = self.getIndividual(k)
            fitnessArray[ind.getIndividualID()] = ind.getFitnessValue()

        # Vetor com os valores de fitness para cada individuo
        valueArray = list(fitnessArray.values())

        # Vetor com os ID de cada individuo
        keyArray = list(fitnessArray.keys())

        #Ordena a populacao baseado no valor de fitness
        heapsort.heapSort(valueArray, keyArray)

        self.sortedPopulationIndexes = keyArray
        self.fitnessArray = valueArray

        # for i in range(len(valueArray)):
        #     ind = self.getIndividual(i)
        #     ind.setFitnessValue(valueArray[i])
        #     ind.setID(keyArray[i])

    def runCrossover(self, id_selected1, id_selected2, id_replace1, id_replace2):
        selected1 = self.getIndividual(id_selected1)
        selected2 = self.getIndividual(id_selected2)
        replace1 = self.getIndividual(id_replace1)
        replace2 = self.getIndividual(id_replace2)
        self.GA.crossover(selected1, selected2, replace1, replace2)

    def runMutation(self, id_selected1, id_replace1):
        selected1 = self.getIndividual(id_selected1)
        replace1 = self.getIndividual(id_replace1)

        self.GA.mutation(selected1, replace1)

    def runRoulette(self):
        self.roulette = self.GA.buildRoulette(self.fitnessArray)
        # print("Roulette")
        # print(self.roulette)

    def runWheelSelection(self):
        self.runRoulette()
        selected = self.GA.rouletteWheelSelection(self.roulette)
        return selected

    def reproduction(self):
        new_ind = 0
        idx = 0
        while new_ind < globals.NEW_INDIVIDUALS:
            probabilityValue = random.randint(0, 1000) % 101

            # Escolheu crossover
            if(probabilityValue <= self.GA.crossoverRate):
                # So pode gerar mais um individuo
                if(new_ind == globals.NEW_INDIVIDUALS - 1):
                    new_ind += 1
                    selected1 = self.runWheelSelection()
                    selected2 = selected1
                    # self.runCrossover(selected1, selected1,
                    #                     self.sortedPopulationIndexes[new_ind-1],
                    #                     self.sortedPopulationIndexes[new_ind-1])
                else:
                    new_ind += 2
                    selected1 = self.runWheelSelection()
                    selected2 = self.runWheelSelection()
                    # print("Dois selecionados")
                    # self.getIndividual(selected1).printIndividual()
                    # self.getIndividual(selected2).printIndividual()
                    self.runCrossover(selected1, selected2,
                                        self.sortedPopulationIndexes[idx],
                                        self.sortedPopulationIndexes[idx+1])
                    idx += 2

            # Escolheu a mutacao
            else:
                new_ind += 1
                selected1 = self.runWheelSelection()
                self.runMutation(selected1, self.sortedPopulationIndexes[idx])
                idx += 1

    def runGenerations(self):
        # print("Calculando fitness...")
        self.calculatePopulationFitness()
        # print("Ordenando pelo fitness...")
        self.sortPopulationByFitness()
        # self.savePopulationFitnessToFile()
        for i in range(globals.MAX_GENERATION):
            self.runRoulette()
            self.reproduction()
            self.calculatePopulationFitness()
            self.sortPopulationByFitness()
            # self.savePopulationFitnessToFile()
            # print("========================================= Generation " + str(i) + " -======================================")
            # self.printPopulation()

# if __name__ == "__main__":
#     GA = GeneticAlgorithm()
#     ind = Individual(4, 0)
#     # ind.printIndividual()
#     population = Population()
#     population.setID()

    # print("%.2f" % round(GA.quality2,2))
