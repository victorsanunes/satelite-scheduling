import fileHelper as fh
import globals
import geneticAlgorithm as GA

def loadDataset():
    dataset = fh.readFile(globals.INPUT_FILE)
    ga = GA.GeneticAlgorithm()
    # individuo = GA.Individual(4, 0)
    # ind.printIndividual()
    population = GA.Population()
    ind_count = 0
    jump = 0

    # Itera sobre cada um dos individuos da populacao
    for ind in range(globals.MAX_INDIVIDUALS):
        # Salto para corrigir a posicao da linha no dataset
        jump = ind*globals.LINES_PER_SINGLE_INDIVIDUAL

        # Para cada uma das caracteristicas
        for i in range(globals.LINES_PER_SINGLE_INDIVIDUAL):
            if i == globals.WINDOW:
                for j in range(len(dataset[0])):
                    population.getIndividual(ind).setWindow(j, dataset[i+jump][j])

            elif i == globals.KLINE:
                for j in range(len(dataset[i])):
                    population.getIndividual(ind).setKline(j, dataset[i+jump][j])

            elif i == globals.REQUEST_BEGINNING:
                for j in range(len(dataset[i])):
                    population.getIndividual(ind).setReqBegin(j, dataset[i+jump][j])

            elif i == globals.REQUEST_ENDING:
                for j in range(len(dataset[i])):
                    population.getIndividual(ind).setReqEnd(j, dataset[i+jump][j])

            elif i == globals.STATION:
                for j in range(len(dataset[i])):
                    population.getIndividual(ind).setStation(j, dataset[i+jump][j])

            elif i == globals.WINDOW_BEGINNING:
                for j in range(len(dataset[i])):
                    population.getIndividual(ind).setWindowBegin(j, dataset[i+jump][j])

            elif i == globals.WINDOW_ENDING:
                for j in range(len(dataset[i])):
                    population.getIndividual(ind).setWindowEnd(j, dataset[i+jump][j])

    return population

def main():
    population = loadDataset()
    # population.printPopulation()

    population.calculatePopulationFitness()
    population.printPopulationFitness()
    population.sortPopulationByFitness()
    print("\n\nAfter sorting")
    population.printPopulationFitness()
    # population.printPopulation()

    # ind = population.getIndividual(10)
    # ind.setFitnessValue()
    # ind.printIndividual()


main()
