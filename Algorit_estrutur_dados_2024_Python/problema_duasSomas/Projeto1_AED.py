import random
import time

def cria_tabela(n_elementos):
    tab = []
    for i in range(n_elementos):
        x = random.randint(0,n_elementos)
        tab.append(x)
    valor_k = random.randint(1,n_elementos) 
    return tab, valor_k

#O(n^2)
def algoritmo1(tabela,k):
    for i in range(len(tabela)):
        for y in range(i+1, len(tabela)):
            if(tabela[i]!= tabela[y]):
                if(tabela[i]+tabela[y]==k):
                    return True
                
#O(nlogn)

def algoritmo2(tabela, k):
    tabela.sort()
    i = 0
    j = len(tabela) - 1
    
    while i < j:
        soma = tabela[i] + tabela[j]
        if soma == k:
            return True
        elif soma < k:
            i += 1
        else:
            j -= 1
    
    return False
            
            
#complexidade do set --> O(1)
#O(n)

def algoritmo3(tabela, k):
    vistos = set()
    for valor in tabela:
        complemento = k - valor
        if complemento in vistos:
            return True
        vistos.add(valor)
    return False
    

tabelas= []
numeros = []
for i in range(30):
    tabela,k = cria_tabela(10000)
    tabelas.append(tabela)
    numeros.append(k)
t1 = 0
t2 = 0
t3 = 0
for i in range(30):
    tabela = tabelas[i]
    k = numeros[i]
    inicio = time.time()
    algoritmo1(tabela,k)
    fim = time.time()
    tempo = fim - inicio
    t1 = t1 + tempo
    
    inicio = time.time()
    algoritmo2(tabela,k)
    fim = time.time()
    tempo = fim - inicio
    t2 = t2 + tempo
    
    inicio = time.time()
    algoritmo3(tabela,k)
    fim = time.time()
    tempo = fim - inicio
    t3 = t3 + tempo
    
    
print("D1:")
print((t1/30)*1000)
print("D2:")
print((t2/30)*1000)
print("D3:")
print((t3/30)*1000)