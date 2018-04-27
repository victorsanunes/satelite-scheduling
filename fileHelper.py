import globals
import csv

def readFile(filename):

    file = open(filename, "r")
    # dataset = {x: list() for x in range(globals.MAX_INDIVIDUALS)}
    dataset = list()
    with open(filename) as tsv:
        for line in csv.reader(tsv, dialect="excel-tab"):
            if len(line) > 0:
                dataset.append(line)

    return dataset
