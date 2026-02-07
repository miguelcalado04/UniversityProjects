import time
import random

def criar_lista_crescente(x):
    lista = []
    for i in range(1, x+1):
        lista.append(i)
    return lista

def criar_lista_decrescente(x):
    lista = []
    for i in range(x, 0, -1):
        lista.append(i)
    return lista

def criar_lista_aleatoria(x):
    lista = list(range(1, x+1))
    random.shuffle(lista)
    return lista

# Algoritmo Insertion Sort---------------------------------------------------------

def insertion_sort(valores):
    for i in range(1, len(valores)):
        j = i
        while j > 0 and valores[j] < valores[j - 1]:
            valores[j], valores[j - 1] = valores[j - 1], valores[j]
            j -= 1
    return valores

# Algoritmo HEAP SORT -------------------------------------------------------------

def heapify(valores, n, a):
    maior = a
    esquerda = 2 * a + 1
    direita = 2 * a + 2

    if esquerda < n and valores[esquerda] > valores[maior]:
        maior = esquerda

    if direita < n and valores[direita] > valores[maior]:
        maior = direita

    if maior != a:
        valores[a], valores[maior] = valores[maior], valores[a]
        heapify(valores, n, maior)


def heapSort(valores):
    n = len(valores)

    #
    for x in range(n // 2 - 1, -1, -1):
        heapify(valores, n, x)

    for x in range(n - 1, 0, -1):
        valores[x], valores[0] = valores[0], valores[x]
        heapify(valores, x, 0)

# Algoritmo Quick Sort ------------------------------------------------------------

def partition(valores, menor, maior):
    piv = valores[(menor + maior) // 2]
    i = menor - 1
    j = maior + 1
    while True:
        i += 1
        while valores[i] < piv:
            i += 1
        j -= 1
        while valores[j] > piv:
            j -= 1
        if i >= j:
            return j
        valores[i], valores[j] = valores[j], valores[i]

def quicksort(valores, menor, maior):
    if menor < maior:
        pi = partition(valores, menor, maior)
        quicksort(valores, menor, pi)
        quicksort(valores, pi + 1, maior)


arr2 = [3, 6, 8, 10, 12,18,13, 9, 2, 1]
quicksort(arr2, 0, len(arr2) - 1)
print(arr2)
