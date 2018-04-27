import random

def printIndexedArray(keyArray, valueArray):
    print("Indexed array")
    for i in keyArray:
        print(str(valueArray[i]) + " ", end="")
    print("\n")

def getIndexedElement(keyArray, valueArray, i):
    return valueArray[keyArray[i]]

def redoHeap(valueArray, keyArray, i, size):
    son = int(2*i + 1)
    # print("PAI: " + str(getIndexedElement(keyArray, valueArray, i)))
    # Checa se o filho da direita existe e se eh maior do que o pai
    if ((son + 1) <= (size - 1)) \
            and ( (getIndexedElement(keyArray, valueArray, i) < getIndexedElement(keyArray, valueArray, son+1)) \
            or (getIndexedElement(keyArray, valueArray, i) < getIndexedElement(keyArray, valueArray, son)) ):

        # Descobre se o filho da esquerda eh o maior
        if( (getIndexedElement(keyArray, valueArray, son) >= getIndexedElement(keyArray, valueArray, son+1)) \
                                    & (getIndexedElement(keyArray, valueArray, son) >= getIndexedElement(keyArray, valueArray, i)) ):
            pos = son
            keyArray[i], keyArray[son] = keyArray[son], keyArray[i]
            redoHeap(valueArray, keyArray, pos, size)

        # Descobre se o filho da direita eh o maior
        elif ((getIndexedElement(keyArray, valueArray, son+1) >= getIndexedElement(keyArray, valueArray, son)) \
                & (getIndexedElement(keyArray, valueArray, son+1) >= getIndexedElement(keyArray, valueArray, i))):

            pos = son + 1
            keyArray[i], keyArray[son+1] = keyArray[son+1], keyArray[i]
            redoHeap(valueArray, keyArray, pos, size)

    # Checa se o filho da esquerda existe e se ele eh maior do que pai
    elif (son <= (size - 1)) and (getIndexedElement(keyArray, valueArray, i) < getIndexedElement(keyArray, valueArray, son)):
        pos = son
        redoHeap(valueArray, keyArray, pos, size)
        keyArray[i], keyArray[son] = keyArray[son], keyArray[i]

def buildHeap(valueArray, keyArray, size):
    left = int((size-1)/2)
    while (left >= 0):
        redoHeap(valueArray, keyArray, left, size)
        left = left - 1

def heapSort(valueArray, keyArray):
    # indice do ultimo elemento
    i = len(valueArray) - 1

    buildHeap(valueArray, keyArray, len(valueArray))
    while(i > 0):
        keyArray[0], keyArray[i] = keyArray[i], keyArray[0]
        redoHeap(valueArray, keyArray, 0, i)
        i -= 1

# def generateArray(n):
#     array = list()
#     for i in range(n):
#         array.append(random.randint(0, 1000))
#     return array

# def main():
#     random.seed(1)
#     # array = generateArray(10)
#     keyArray = [0, 1, 2, 3, 4, 5, 6]
#     valueArray = [3.0, 2.95, 2.0, 1.8, 1.75, 1.5, 0.75]
#     print("Original keyArray: " + repr(keyArray))
#     print("Original valueArray: " + repr(valueArray))
#     # # redoHeap(valueArray, keyArray, int((len(keyArray)-2)/2), len(keyArray))
#     # printIndexedArray(keyArray, valueArray)
#     heapSort(valueArray, keyArray)
#     printIndexedArray(keyArray, valueArray)
#     # # array = heapSort(array)
#     print("Sorted keyArray: " + repr(keyArray))
#     print("Sorted valueArray: " + repr(valueArray))
#
# main()
