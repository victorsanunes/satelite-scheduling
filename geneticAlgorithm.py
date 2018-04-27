import globals
import fileHelper as fh
import heapsort

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

    def __init__(self):
        self.quality1 = self.calculateQualityValues(30)
        self.quality2 = self.calculateQualityValues(31)

class Individual(object):
    def __init__(self, requests, indID):
        self.features = {x: dict() for x in range(requests)}
        self.fitness = 0.0
        self.indID = indID

    # ----------------------------- CALCULATIONS -------------------------------
    def calculateFitnessValue(self):
        ga = GeneticAlgorithm()
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
        # @TODO: validar a correcao dos indices apos a ordenacao

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

        for i in range(len(valueArray)):
            ind = self.getIndividual(i)
            ind.setFitnessValue(valueArray[i])
            ind.setID(keyArray[i])

# if __name__ == "__main__":
#     GA = GeneticAlgorithm()
#     ind = Individual(4, 0)
#     # ind.printIndividual()
#     population = Population()
#     population.setID()

    # print("%.2f" % round(GA.quality2,2))
