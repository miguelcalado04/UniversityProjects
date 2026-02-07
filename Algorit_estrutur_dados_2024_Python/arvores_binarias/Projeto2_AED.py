import time
#Arvore binaria---------------------------------------------------------  

def criar_no_arvore_binaria(chave):
    return {"valor": chave, "esquerda": None, "direita": None}
    
def criar_arvore_binaria():
        return None
    
def insere_esquerda_arvore_binaria(raiz, chave):
        if raiz is None:
            return criar_no_arvore_binaria(chave)
        else:
            atual = raiz
            while atual["esquerda"] is not None:
                atual = atual["esquerda"]
            atual["esquerda"] = criar_no_arvore_binaria(chave)
        return raiz
    
def imprime_arvore_binaria(raiz):
        if raiz is None:
            print("Árvore vazia")
            return
            
        stack = [(raiz, 0)]
        while stack:
            no, nivel = stack.pop()
            if no:
                print(' ' * 5 * nivel + '↳', no["valor"])
                stack.append((no["direita"], nivel + 1))
                stack.append((no["esquerda"], nivel + 1))
    
inicio = time.time()
raiz = criar_arvore_binaria()
valores = [90,30,70,200,40,10,80,5,105,19,39,40,1,3,500,33,220]
for valor in valores:
        raiz = insere_esquerda_arvore_binaria(raiz, valor)
    
fim = time.time()
tempo = fim - inicio
print("\nÁrvore binária (tempo:{})\n".format(tempo))
imprime_arvore_binaria(raiz)
    
    
    #----------------------------------------------------------------------
    #Arvore binaria de pesquisa
    
    
def criar_no_arvore(chave):
        return {"valor": chave, "esquerda": None, "direita": None}
    
def insere_arvore(arvore, chave):
        
        if arvore is None:
            return criar_no_arvore(chave)
        
        else:
            no_atual = arvore
            
            while no_atual:
                
                if chave < no_atual["valor"]:
                    
                    if no_atual["esquerda"] is None:
                        no_atual["esquerda"] = criar_no_arvore(chave)
                        break
                    
                    else:
                        no_atual = no_atual["esquerda"]
                        
                elif chave > no_atual["valor"]:
                    
                    if no_atual["direita"] is None:
                        no_atual["direita"] = criar_no_arvore(chave)
                        break
                    
                    else:
                        no_atual = no_atual["direita"]
                else:
                    break
        return arvore
    
def imprime_arvore(arvore):
        
        if not arvore:
            print("Árvore vazia")
            return
    
        stack = []
        nivel = 0
    
        while stack or arvore:
            if arvore:
                stack.append((arvore, nivel))
                arvore = arvore["direita"]
                nivel += 1
            else:
                arvore, nivel = stack.pop()
                print(' ' * 5 * nivel + '-> ', arvore["valor"])
                arvore = arvore["esquerda"]
                nivel += 1
    
    
inicio = time.time()
arvore = None
valores = [90,30,70,200,40,10,80,5,105,19,39,40,1,3,500,33,220]
for valor in valores:
        arvore = insere_arvore(arvore, valor)
    
fim = time.time()
tempo = fim - inicio
print("Árvore binária de pesquisa (tempo:{}):\n".format(tempo))    
imprime_arvore(arvore)
    
        
#----------------------------------------------------------------------
# Arvore AVL
def criar_no_avl(chave):
       return {"valor": chave, "esquerda": None, "direita": None, "altura_no": 1}
   
def insere_avl_contagem_rotacoes(raiz, chave):
       if not raiz:
           return criar_no_avl(chave), 0, 0
   
       rotacoes_simples = 0
       rotacoes_duplas = 0
   
       if chave < raiz["valor"]:
           raiz["esquerda"], rotacoes_esquerda_simples, rotacoes_esquerda_duplas = insere_avl_contagem_rotacoes(raiz["esquerda"], chave)
           rotacoes_simples += rotacoes_esquerda_simples
           rotacoes_duplas += rotacoes_esquerda_duplas
       else:
           raiz["direita"], rotacoes_direita_simples, rotacoes_direita_duplas = insere_avl_contagem_rotacoes(raiz["direita"], chave)
           rotacoes_simples += rotacoes_direita_simples
           rotacoes_duplas += rotacoes_direita_duplas
   
       raiz["altura_no"] = 1 + max(get_altura_no(raiz["esquerda"]), get_altura_no(raiz["direita"]))
       balance = get_balance(raiz)
   
       # Rotações
       if balance > 1 and chave < raiz["esquerda"]["valor"]:
           return rotate_direita(raiz), rotacoes_simples + 1, rotacoes_duplas
       if balance < -1 and chave > raiz["direita"]["valor"]:
           return rotate_esquerda(raiz), rotacoes_simples + 1, rotacoes_duplas
       if balance > 1 and chave > raiz["esquerda"]["valor"]:
           raiz["esquerda"] = rotate_esquerda(raiz["esquerda"])
           return rotate_direita(raiz), rotacoes_simples + 2, rotacoes_duplas + 1
       if balance < -1 and chave < raiz["direita"]["valor"]:
           raiz["direita"] = rotate_direita(raiz["direita"])
           return rotate_esquerda(raiz), rotacoes_simples + 2, rotacoes_duplas + 1
   
       return raiz, rotacoes_simples, rotacoes_duplas
   
def rotate_direita(z):
       y = z["esquerda"]
       T3 = y["direita"]
       y["direita"] = z
       z["esquerda"] = T3
   
       z["altura_no"] = 1 + max(get_altura_no(z["esquerda"]), get_altura_no(z["direita"]))
       y["altura_no"] = 1 + max(get_altura_no(y["esquerda"]), get_altura_no(y["direita"]))
       
       return y
   
def rotate_esquerda(z):
       y = z["direita"]
       T2 = y["esquerda"]
   
       y["esquerda"] = z
       z["direita"] = T2
   
       z["altura_no"] = 1 + max(get_altura_no(z["esquerda"]), get_altura_no(z["direita"]))
       y["altura_no"] = 1 + max(get_altura_no(y["esquerda"]), get_altura_no(y["direita"]))
   
       return y
   
def get_altura_no(raiz):
       if not raiz:
           return 0
       return raiz["altura_no"]
   
def get_balance(raiz):
       if not raiz:
           return 0
       return get_altura_no(raiz["esquerda"]) - get_altura_no(raiz["direita"])
   
def imprime_avl(raiz):
       imprime_avl2(raiz, 0)
   
def imprime_avl2(raiz, nivel):
       if raiz:
           imprime_avl2(raiz["direita"], nivel + 1)
           print(' ' * 4 * nivel + '-> ', raiz["valor"])
           imprime_avl2(raiz["esquerda"], nivel + 1)
   
   
inicioAVL = time.time()
raiz = None
rotacoes_simples_totais = 0
rotacoes_duplas_totais = 0
valores = [90,30,70,200,40,10,80,5,105,19,39,40,1,3,500,33,220]
for valor in valores:
       raiz, rotacoes_simples, rotacoes_duplas = insere_avl_contagem_rotacoes(raiz, valor)
       rotacoes_simples_totais += rotacoes_simples
       rotacoes_duplas_totais += rotacoes_duplas
fimAVL = time.time()
tempoAVL = fimAVL - inicioAVL
print("AVL (tempo:{}):\n".format(tempoAVL))
print("Rotações simples :", rotacoes_simples_totais)
print("Rotações duplas:", rotacoes_duplas_totais)
imprime_avl(raiz)

#----------------------------------------------------------------------
# Árvore VP

rotacoes_simples = 0
rotacoes_duplas = 0
    
def criar_no(valor):
        return {"valor": valor, "vermelho": False, "pai": None, "esquerda": None, "direita": None}
    
def inserir(raiz, valor):
        global rotacoes_simples, rotacoes_duplas  
        novo_no = criar_no(valor)
        novo_no["vermelho"] = True
        pai = None
        atual = raiz
    
        while atual:
            pai = atual
            if novo_no["valor"] < atual["valor"]:
                atual = atual["esquerda"]
            elif novo_no["valor"] > atual["valor"]:
                atual = atual["direita"]
            else:
                return raiz
    
        novo_no["pai"] = pai
        if not pai:
            raiz = novo_no
        elif novo_no["valor"] < pai["valor"]:
            pai["esquerda"] = novo_no
        else:
            pai["direita"] = novo_no
    
        raiz = verificar(raiz, novo_no)
        return raiz
    
def verificar(raiz, novo_no):
        global rotacoes_simples, rotacoes_duplas 
        count = 0
        while novo_no != raiz and novo_no["pai"]["vermelho"]:  
            if novo_no["pai"] == novo_no["pai"]["pai"]["direita"]: 
                tio = novo_no["pai"]["pai"]["esquerda"]  
                if tio and tio["vermelho"]:  
                    tio["vermelho"] = False
                    novo_no["pai"]["vermelho"] = False
                    novo_no["pai"]["pai"]["vermelho"] = True
                    novo_no = novo_no["pai"]["pai"]
                    
                else:  
                    if novo_no == novo_no["pai"]["esquerda"]:  
                        novo_no = novo_no["pai"]
                        raiz = roda_direita(raiz, novo_no)
                        rotacoes_simples += 1
                        count += 1
                    novo_no["pai"]["vermelho"] = False
                    novo_no["pai"]["pai"]["vermelho"] = True
                    raiz = roda_esquerda(raiz, novo_no["pai"]["pai"])
                    if count > 1:
                        rotacoes_duplas += 1  
                    count = 0
                    rotacoes_simples += 1
            else:  
                tio = novo_no["pai"]["pai"]["direita"]  
                if tio and tio["vermelho"]:  
                    tio["vermelho"] = False
                    novo_no["pai"]["vermelho"] = False
                    novo_no["pai"]["pai"]["vermelho"] = True
                    novo_no = novo_no["pai"]["pai"]
                else:  
                    if novo_no == novo_no["pai"]["direita"]:  
                        novo_no = novo_no["pai"]
                        raiz = roda_esquerda(raiz, novo_no)
                        rotacoes_simples += 1
                        count += 1
                    novo_no["pai"]["vermelho"] = False
                    novo_no["pai"]["pai"]["vermelho"] = True
                    raiz = roda_direita(raiz, novo_no["pai"]["pai"])
                    if count > 1:
                        rotacoes_duplas += 1  
                    count = 0
                    rotacoes_simples += 1
                    
        raiz["vermelho"] = False
        return raiz
    
    
def roda_esquerda(raiz, x):
        y = x["direita"]  
        x["direita"] = y["esquerda"]
        if y["esquerda"] is not None:
            y["esquerda"]["pai"] = x
    
        y["pai"] = x["pai"]
        if x["pai"] is None:
            raiz = y
        elif x is x["pai"]["esquerda"]:
            x["pai"]["esquerda"] = y
        else:
            x["pai"]["direita"] = y
        y["esquerda"] = x
        x["pai"] = y
        return raiz
    
def roda_direita(raiz, x):
        y = x["esquerda"]
        x["esquerda"] = y["direita"]
        if y["direita"] is not None:
            y["direita"]["pai"] = x
    
        y["pai"] = x["pai"]
        if x["pai"] is None:
            raiz = y
        elif x is x["pai"]["direita"]:
            x["pai"]["direita"] = y
        else:
            x["pai"]["esquerda"] = y
        y["direita"] = x
        x["pai"] = y
        return raiz
    
def imprimir_arvore(raiz, nivel=0):
        if raiz:
            if (raiz["vermelho"] == False):
                cor_raiz = "🄿"
            else:
                cor_raiz = "🅅"
                
            imprimir_arvore(raiz["direita"], nivel + 1)
            print(' ' * 5 * nivel + '->' + f'{raiz["valor"]} {cor_raiz}')
            imprimir_arvore(raiz["esquerda"], nivel + 1)
    
inicioAVP = time.time()
raiz = None
valores = [90,30,70,200,40,10,80,5,105,19,39,40,1,3,500,33,220]
for valor in valores:
        raiz = inserir(raiz, valor)
    
fimAVP = time.time()
tempoAVP = fimAVP - inicioAVP
print("\nÁrvore VP (tempo:{}):".format(tempoAVP))
print("Número de rotações simples:", rotacoes_simples)
print("Número de rotações duplas:", rotacoes_duplas)
imprimir_arvore(raiz)
