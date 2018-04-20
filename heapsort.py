import random

def redoHeap(valueArray, keyArray, i, size):
    son = int(2*i + 1)
    # print("son: " + repr(array[son]))

    # Checa se o filho da direita existe e se eh maior do que o pai
    if ((son + 1) <= (size - 1)) and ( (valueArray[i] < valueArray[son + 1]) or (valueArray[i] < valueArray[son]) ):

        # Descobre qual dos filhos eh o maior
        if( (valueArray[son] >= valueArray[son+1]) & (valueArray[son] >= valueArray[i]) ):
            pos = son
            valueArray[i], valueArray[son] = valueArray[son], valueArray[i]
            keyArray[i], keyArray[son] = keyArray[son], keyArray[i]
            redoHeap(valueArray, keyArray, pos, size)

        elif ((valueArray[son+1] >= valueArray[son]) & (valueArray[son+1] >= valueArray[i])):
            pos = son + 1
            valueArray[i], valueArray[son+1] = valueArray[son+1], valueArray[i]
            keyArray[i], keyArray[son+1] = keyArray[son+1], keyArray[i]
            redoHeap(valueArray, keyArray, pos, size)
        # print("Redoheap2(): " + repr(array))
    # Checa se o filho da esquerda existe e se ele eh maior do que pai
elif (son <= (size - 1)) and (valueArray[i] < valueArray[son]):
        pos = son
        redoHeap(valueArray, keyArray, pos, size)
        valueArray[i], valueArray[son] = valueArray[son], valueArray[i]
        keyArray[i], keyArray[son] = keyArray[son], keyArray[i]

        # print("Redoheap1(): " + repr(array))

def buildHeap(valueArray, keyArray, size):
    left = int((size-1)/2)
    while (left >= 0):
        redoHeap(valueArray, keyArray, left, size)
        left = left - 1

def heapSort(valueArray, keyArray):
    i = len(valueArray) - 1
    buildHeap(valueArray, keyArray, len(valueArray))
    while(i > 0):
        array[0], array[i] = array[i], array[0]
        redoHeap(array, 0, i)
        i -= 1

# def generateArray(n):
#     array = list()
#     for i in range(n):
#         array.append(random.randint(0, 1000))
#     return array
#
# def main():
#     random.seed(1)
#     array = generateArray(10)
#     print("Original array: " + repr(array))
#
#     heapSort(array)
#     # array = heapSort(array)
#     print("Sorted array: " + repr(array))
