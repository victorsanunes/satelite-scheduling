import globals
import fileHelper as fh

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

    def fitnessFunction(self, features, id_req):
        weight = 1.0
        value = 0.0

        if(features.getReqBegin(id_req) < features.getWindowBegin(id_req) or
            features.getReqEnd(id_req) > features.getWindowEnd(id_req) or
            features.getReqBegin(id_req) >= features.getReqEnd(id_req)):
            weight = 0.0
            value = 0.0
        elif(features.getWindow(id_req) == 1):
            value += weight * self.quality1[features.getKline(id_req)-1]

        elif(features.getWindow(id_req) == 2):
            value += weight * self.quality2[features.getKline(id_req)-1]

        return value

    def calculateFitnessValues(self, features, quality1, quality2):
        fitnessValue = 0.0
        penaltyFlag = 0
        sum = 0.0

        # Itera para cada uma das solicitacoes
        for req in features:
            window = features.getWindow(req)
            kline = features.getKline(req)
            reqBegin = features.getReqBegin(req)
            reqEnd = features.getReqEnd(req)
            windowBegin = features.getWindowBegin(req)
            windowEnd = features.getWindowEnding(req)

            # Itera para cada uma das proximas solicitacoes
            req2 = req+1
            while(req2 < globals.REQUESTS):
                next_window = features.getWindow(req2)
                next_kline = features.getKline(req2)
                next_reqBegin = features.getReqBegin(req2)
                next_reqEnd = features.getReqEnd(req2)
                next_windowBegin = features.getWindowBegin(req2)
                next_windowEnd = features.getWindowEnding(req2)

                # Checa se a alocacao da solicitacao esta dentro dos limites
                # da janela
                if(reqBegin >= windowBegin and reqEnd <= windowEnd):
                    # Caso as solicitacoes estejam na mesma janela eh preciso
                    # checar preemptivadade e redundancia
                    if(next_window == window):
                        if((reqEnd >= next_reqEnd-1) and (reqBegin <= next_reqEnd+1)):
                            penaltyFlag = 1
                            req2 = globals.REQUESTS
                    else:
                        if(station == next_station):
                            if((reqEnd >= next_reqEnd-1) and (reqBegin <= next_reqEnd+1)):
                                penaltyFlag = 1
                                req2 = globals.REQUESTS
                else:
                    penaltyFlag = 1
                    req2 = globals.REQUESTS

                if(((kline + windowBegin-1 >= windowEnd)
                    or (kline + windowEnd-1 <= windowBegin)) and penaltyFlag == 0):

                    penaltyFlag = 1
                    req2 = globals.REQUESTS

                if(penaltyFlag == 1):
                    fitnessValue = 0.0
                    penaltyFlag = 0

                else:
                    sum += fitnessFunction(self.features, req)
                    penaltyFlag = 0
                    req2 += 1

        print("SOMA: " + str(sum))

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
    def __init__(self, requests):
        self.features = {x: dict() for x in range(requests)}
        self.fitness = 0.0

    def setRequest(self,id,
                    window, kline, reqBegin, reqEnd,
                    station, windowBegin, windowEnd):
        self.features[id]["window"] = window
        self.features[id]["kline"] = kline
        self.features[id]["station"] = station
        self.features[id]["reqBegin"] = reqBegin
        self.features[id]["reqEnd"] = reqEnd
        self.features[id]["windowBegin"] = windowBegin
        self.features[id]["windowEnd"] = windowEnd

    def printRequest(self, id):
        print(repr(self.features[id]["window"]))
        print(repr(self.features[id]["kline"]))
        print(repr(self.features[id]["reqBegin"]))
        print(repr(self.features[id]["reqEnd"]))
        print(repr(self.features[id]["station"]))
        print(repr(self.features[id]["windowBegin"]))
        print(repr(self.features[id]["windowEnd"]))

    def setWindow(self, id, window):
        self.features["window"] = window

    def setKline(self, id, kline):
        self.features["kline"] = kline

    def setReqBegin(self, id, reqBegin):
        self.features[id]["reqBegin"] = reqBegin

    def setReqEnd(self, id, reqEnd):
        self.features[id]["reqEnd"] = reqEnd

    def setStation(self, id, station):
        self.features[id]["station"] = station

    def setWindowBegin(self, id, windowBegin):
        self.features[id]["windowBegin"] = windowBegin

    def setWindowEnd(self, id, windowEnd):
        self.features[id]["windowEnd"] = windowEnd

    def getWindow(self, id):
        return self.features["window"]

    def getKline(self, id):
        return self.features["kline"]

    def getReqBegin(self, id):
        return self.features[id]["reqBegin"]

    def getReqEnd(self, id):
        return self.features[id]["reqEnd"]

    def getStation(self, id):
        return self.features[id]["station"]

    def getWindowBegin(self, id):
        return self.features[id]["windowBegin"]

    def getWindowEnd(self, id):
        return self.features[id]["windowEnd"]

    def printIndividual(self):
        for k in self.features:
            print(self.features[k])
class Population(object):
    # def fillPopulation(self, dataset):

    def __init__(self):
        self.population = {x: Individual(globals.REQUESTS) for x in range(globals.MAX_INDIVIDUALS)}


    def printPopulation(self):
        for k in self.population:
            population[k].printIndividual()


if __name__ == "__main__":
    GA = GeneticAlgorithm()
    ind = Individual(4)
    ind.printIndividual()
    # population = Population()
    

    # print("%.2f" % round(GA.quality2,2))
