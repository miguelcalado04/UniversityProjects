

import turtle as t
import functools
import random
import os
import math
import pygame
import time

LARGURA_JANELA = 1024
ALTURA_JANELA = 600
DEFAULT_TURTLE_SIZE = 40
DEFAULT_TURTLE_SCALE = 3
RAIO_JOGADOR = DEFAULT_TURTLE_SIZE / DEFAULT_TURTLE_SCALE
RAIO_BOLA = DEFAULT_TURTLE_SIZE / 2
PIXEIS_MOVIMENTO = 80
LADO_MAIOR_AREA = ALTURA_JANELA / 3
LADO_MENOR_AREA = 50
RAIO_MEIO_CAMPO = LADO_MAIOR_AREA / 4
START_POS_BALIZAS = ALTURA_JANELA / 6
BOLA_START_POS = (5, 5)
PONTUACAO_JOGOS = "histórico_resultados.csv"


# Funções responsáveis pelo movimento dos jogadores no ambiente.
# O número de unidades que o jogador se pode movimentar é definida pela constante
# PIXEIS_MOVIMENTO. As funções recebem um dicionário que contém o estado
# do jogo e o jogador que se está a movimentar.

def jogador_cima(estado_jogo, jogador):
    jogador = estado_jogo[jogador]
    y = jogador.ycor()
    if (jogador.ycor() + RAIO_JOGADOR + PIXEIS_MOVIMENTO <= ALTURA_JANELA / 2):
        y = y + PIXEIS_MOVIMENTO 
    guarda_posicoes_para_var(estado_jogo)
    jogador.sety(y)


def jogador_baixo(estado_jogo, jogador):
    jogador = estado_jogo[jogador]
    y = jogador.ycor()
    if (y - RAIO_JOGADOR - PIXEIS_MOVIMENTO >= - ALTURA_JANELA / 2):
        y = y - PIXEIS_MOVIMENTO 
    guarda_posicoes_para_var(estado_jogo)
    jogador.sety(y)


def jogador_direita(estado_jogo, jogador):
    jogador = estado_jogo[jogador]
    x = jogador.xcor()
    if (RAIO_JOGADOR + x + PIXEIS_MOVIMENTO <= LARGURA_JANELA / 2):
        x = x + PIXEIS_MOVIMENTO
    guarda_posicoes_para_var(estado_jogo)
    jogador.setx(x)


def jogador_esquerda(estado_jogo, jogador):
    jogador = estado_jogo[jogador]
    x = jogador.xcor()
    if (x - RAIO_JOGADOR - PIXEIS_MOVIMENTO >= (-LARGURA_JANELA / 2)):
        x = x - PIXEIS_MOVIMENTO
    guarda_posicoes_para_var(estado_jogo)
    jogador.setx(x)


def desenha_linhas_campo():
    # Create the field boundaries
    t.penup()
    t.goto(-(LARGURA_JANELA) / 2, (ALTURA_JANELA) / 2)
    t.pendown()
    t.color("white")
    t.pensize(8)
    for i in range(2):
        t.fd(LARGURA_JANELA)
        t.right(90)
        t.fd(ALTURA_JANELA)
        t.right(90)
    criar_balizas(-LARGURA_JANELA / 2, START_POS_BALIZAS, t, -90)
    t.seth(180)
    criar_balizas(LARGURA_JANELA / 2, START_POS_BALIZAS, t, 90)

    t.penup()
    t.goto(BOLA_START_POS[0], ALTURA_JANELA / 2)
    t.pendown()
    t.setheading(-90)
    t.fd(600)

    t.penup()
    t.goto(BOLA_START_POS[0] - RAIO_MEIO_CAMPO * 2, BOLA_START_POS[1])
    t.pendown()
    t.circle(RAIO_MEIO_CAMPO * 2)

    t.hideturtle()


def criar_balizas(x, y, t, ang):
    t.penup()
    t.goto(x, y)
    t.pendown()
    for i in range(2):
        t.fd(LADO_MENOR_AREA)
        t.left(ang)
        t.fd(LADO_MAIOR_AREA)
        t.left(ang)

    ''' Função responsável por desenhar as linhas do campo, 
    nomeadamente a linha de meio campo, o círculo central, e as balizas. '''


def criar_bola():
    bola = t.Turtle()
    bola.shape("circle")
    bola.color("black")
    bola.shapesize(stretch_wid=DEFAULT_TURTLE_SCALE / 2, stretch_len=DEFAULT_TURTLE_SCALE / 2)
    bola.penup()
    bola.goto(BOLA_START_POS)

    '''
    Função responsável pela criação da bola. 
    Deverá considerar que esta tem uma forma redonda, é de cor preta, 
    começa na posição BOLA_START_POS com uma direção aleatória. 
    Deverá ter em conta que a velocidade da bola deverá ser superior à dos jogadores. 
    A função deverá devolver um dicionário contendo 4 elementos: o objeto bola, 
    a sua direção no eixo dos xx, a sua direção no eixo dos yy, 
    e um elemento inicialmente a None que corresponde à posição anterior da mesma.
    '''
    return bola


def cria_jogador(x_pos_inicial, y_pos_inicial, cor):
    jogador = t.Turtle()
    jogador.up()
    jogador.shape("circle")
    jogador.color(cor)
    jogador.shapesize(stretch_wid=DEFAULT_TURTLE_SCALE, stretch_len=DEFAULT_TURTLE_SCALE)
    jogador.goto(x_pos_inicial, y_pos_inicial)

    ''' Função responsável por criar e devolver o objeto que corresponde a um jogador (um objecto Turtle). 
    A função recebe 3 argumentos que correspondem às coordenadas da posição inicial 
    em xx e yy, e a cor do jogador. A forma dos jogadores deverá ser um círculo, 
    cujo seu tamanho deverá ser definido através da função shapesize
    do módulo \texttt{turtle}, usando os seguintes parâmetros: '''
    return jogador


def init_state():
    estado_jogo = {}
    estado_jogo['bola'] = None
    estado_jogo['jogador_vermelho'] = None
    estado_jogo['jogador_azul'] = None
    estado_jogo['var'] = {
        'bola': [],
        'jogador_vermelho': [],
        'jogador_azul': [],
    }
    estado_jogo['pontuacao_jogador_vermelho'] = 0
    estado_jogo['pontuacao_jogador_azul'] = 0
    return estado_jogo


def cria_janela():
    # create a window and declare a variable called window and call the screen()
    window = t.Screen()
    window.title("Foosball Game")
    window.bgpic("relva.png") 
    window.setup(width=LARGURA_JANELA, height=ALTURA_JANELA)
    window.tracer(0)
    return window


def cria_quadro_resultados():
    # Code for creating pen for scorecard update
    quadro = t.Turtle()
    quadro.speed(0)
    quadro.color("black")
    quadro.penup()
    quadro.hideturtle()
    quadro.goto(0, 260)
    quadro.write("Player A: 0\t\tPlayer B: 0 ", align="center", font=('Bolt', 24, "normal"))
    return quadro

def confirmar_linhas_vazias(lista):
    lis = []
    if(len(lista) > 1):
        for i in range(len(lista)):
            if(lista[i] != '\n'):
                lis.append(lista[i])
    return lis

def terminar_jogo(estado_jogo):
    '''
     Função responsável por terminar o jogo. Nesta função, deverá atualizar o ficheiro
     ''historico_resultados.csv'' com o número total de jogos até ao momento,
     e o resultado final do jogo. Caso o ficheiro não exista,
     ele deverá ser criado com o seguinte cabeçalho:
     NJogo,JogadorVermelho,JogadorAzul.
    '''
    if (os.path.isfile(PONTUACAO_JOGOS)):
        a = open(PONTUACAO_JOGOS, 'r')
        lista = a.readlines()
        a.close()
        lista = confirmar_linhas_vazias(lista)
        if (len(lista) > 1):
                a = open(PONTUACAO_JOGOS, 'w')
                a.writelines(lista)
                a.write(str(len(lista)) + "," + str(estado_jogo['pontuacao_jogador_vermelho']) + "," + str(
                    estado_jogo['pontuacao_jogador_azul']) + "\n")
        else:
                a = open(PONTUACAO_JOGOS, 'w')
                a.write("NJogo,JogadorVermelho,JogadorAzul\n")
                a.write("1" + "," + str(estado_jogo['pontuacao_jogador_vermelho']) + "," + str(
                    estado_jogo['pontuacao_jogador_azul']) + "\n")
    else:
        a = open(PONTUACAO_JOGOS, 'w')
        a.write("NJogo,JogadorVermelho,JogadorAzul\n")
        a.write("1" + "," + str(estado_jogo['pontuacao_jogador_vermelho']) + "," + str(
            estado_jogo['pontuacao_jogador_azul']) + "\n")

    a.close()

    print("Adeus")
    estado_jogo['janela'].bye()


def setup(estado_jogo, jogar):
    janela = cria_janela()
    # Assign keys to play
    janela.listen()
    if jogar:
        janela.onkeypress(functools.partial(jogador_cima, estado_jogo, 'jogador_vermelho'), 'w')
        janela.onkeypress(functools.partial(jogador_baixo, estado_jogo, 'jogador_vermelho'), 's')
        janela.onkeypress(functools.partial(jogador_esquerda, estado_jogo, 'jogador_vermelho'), 'a')
        janela.onkeypress(functools.partial(jogador_direita, estado_jogo, 'jogador_vermelho'), 'd')
        janela.onkeypress(functools.partial(jogador_cima, estado_jogo, 'jogador_azul'), 'Up')
        janela.onkeypress(functools.partial(jogador_baixo, estado_jogo, 'jogador_azul'), 'Down')
        janela.onkeypress(functools.partial(jogador_esquerda, estado_jogo, 'jogador_azul'), 'Left')
        janela.onkeypress(functools.partial(jogador_direita, estado_jogo, 'jogador_azul'), 'Right')
        janela.onkeypress(functools.partial(terminar_jogo, estado_jogo), 'Escape')
        quadro = cria_quadro_resultados()
        estado_jogo['quadro'] = quadro
    desenha_linhas_campo()
    bola = criar_bola()
    jogador_vermelho = cria_jogador(-((ALTURA_JANELA / 2) + LADO_MENOR_AREA), 0, "red")
    jogador_azul = cria_jogador(((ALTURA_JANELA / 2) + LADO_MENOR_AREA), 0, "blue")
    estado_jogo['janela'] = janela
    estado_jogo['bola'] = bola
    estado_jogo['jogador_vermelho'] = jogador_vermelho
    estado_jogo['jogador_azul'] = jogador_azul
    guarda_posicoes_para_var(estado_jogo)


def update_board(estado_jogo):
    estado_jogo['quadro'].clear()
    estado_jogo['quadro'].write("Player A: {}\t\tPlayer B: {} ".format(estado_jogo['pontuacao_jogador_vermelho'],
                                                                       estado_jogo['pontuacao_jogador_azul']),
                                align="center", font=('bolt', 24, "normal"))


def guarda_ficheiro_var(estado_jogo):
    nome_ficheiro = "replay_golo_jv_{}_ja_{}.txt".format(estado_jogo['pontuacao_jogador_vermelho'],estado_jogo['pontuacao_jogador_azul'] )
    with open(nome_ficheiro, "w") as f:
        f.write("{}\n".format(';'.join(map(lambda x: ','.join(map(str, x)), estado_jogo['var']['bola']))))
        f.write("{}\n".format(';'.join(map(lambda x: ','.join(map(str, x)), estado_jogo['var']['jogador_vermelho']))))
        f.write("{}\n".format(';'.join(map(lambda x: ','.join(map(str, x)), estado_jogo['var']['jogador_azul']))))


def movimenta_bola(estado_jogo):
    '''
    Função responsável pelo movimento da bola que deverá ser feito tendo em conta a
    posição atual da bola e a direção em xx e yy.
    '''
    bola = estado_jogo['bola']
    x_bola, y_bola = bola.xcor(), bola.ycor()

    if (x_bola == 5 and y_bola == 5):
        angul_ale = random.random() * 360
        bola.setheading(angul_ale)

    bola.fd(PIXEIS_MOVIMENTO * 0.0089)
    verifica_toque_jogador_vermelho(estado_jogo)
    verifica_toque_jogador_azul(estado_jogo)
    verifica_colisoes_ambiente(estado_jogo)


def verifica_colisoes_ambiente(estado_jogo):
    '''
    Função responsável por verificar se há colisões com os limites do ambiente,
    atualizando a direção da bola. Não se esqueça de considerar que nas laterais,
    fora da zona das balizas, a bola deverá inverter a direção onde atingiu o limite.
    '''
    bola = estado_jogo['bola']

    x_bola, y_bola = bola.xcor(), bola.ycor()

    if x_bola + RAIO_BOLA >= LARGURA_JANELA / 2 or x_bola - RAIO_BOLA <= -LARGURA_JANELA / 2 or y_bola + RAIO_BOLA >= ALTURA_JANELA / 2 or y_bola - RAIO_BOLA <= -ALTURA_JANELA / 2:
        guarda_posicoes_para_var(estado_jogo)
        linhas_parede = [(0, 300), (0, -300), (-512, 0), (512, 0)]
        for linha in linhas_parede:
            delta_x = x_bola - linha[0]
            delta_y = y_bola - linha[1]

            angulo_radianos = math.atan2(delta_y, delta_x)

        bola.seth(bola.heading() + angulo_radianos * 2)


def volta_inicio(estado_jogo):
    bola = estado_jogo['bola']
    jogador1 = estado_jogo['jogador_vermelho']
    jogador2 = estado_jogo['jogador_azul']
    jogador1.goto(-((ALTURA_JANELA / 2) + LADO_MENOR_AREA), 0)
    jogador2.goto(((ALTURA_JANELA / 2) + LADO_MENOR_AREA), 0)
    bola.goto(BOLA_START_POS)
    guarda_posicoes_para_var(estado_jogo)


def atualiza_posicoes_var(estado_jogo):
    estado_jogo['var']['bola'] = []
    estado_jogo['var']['jogador_vermelho'] = []
    estado_jogo['var']['jogador_azul'] = []
    
def comemoracao_golo(x,y,tamLetra,estado_jogo):
    janela = estado_jogo['janela']
    mensagem = t.Turtle()
    mensagem.speed(0)
    mensagem.color("black")
    mensagem.penup()
    mensagem.hideturtle()
    mensagem.goto(x, y)
    mensagem.write("GOLOOO!", align='center', font=("bold", 50, "normal"))
    pygame.mixer.init()
    pygame.mixer.music.load("SUIII.mp3")
    pygame.mixer.music.play()   
    janela.update()
    time.sleep(1)
    pygame.mixer.music.stop() 
    mensagem.clear()

def verifica_golo_jogador_vermelho(estado_jogo):
    '''
    Função responsável por verificar se um determinado jogador marcou golo.
    Para fazer esta verificação poderá fazer uso das constantes:
    LADO_MAIOR_AREA e
    START_POS_BALIZAS.
    Note que sempre que há um golo, deverá atualizar a pontuação do jogador,
    criar um ficheiro que permita fazer a análise da jogada pelo VAR,
    e reiniciar o jogo com a bola ao centro.
    O ficheiro para o VAR deverá conter todas as informações necessárias
    para repetir a jogada, usando as informações disponíveis no objeto
    estado_jogo['var']. O ficheiro deverá ter o nome

    replay_golo_jv_[TotalGolosJogadorVermelho]_ja_[TotalGolosJogadorAzul].txt

    onde [TotalGolosJogadorVermelho], [TotalGolosJogadorAzul]
    deverão ser substituídos pelo número de golos marcados pelo jogador vermelho
    e azul, respectivamente. Este ficheiro deverá conter 3 linhas, estruturadas
    da seguinte forma:
    Linha 1 - coordenadas da bola;
    Linha 2 - coordenadas do jogador vermelho;
    Linha 3 - coordenadas do jogador azul;

    Em cada linha, os valores de xx e yy das coordenadas são separados por uma
    ',', e cada coordenada é separada por um ';'.
    '''
    bola = estado_jogo['bola']
    x = bola.xcor()
    y = bola.ycor()
    if (x + RAIO_BOLA >= LARGURA_JANELA / 2 and y < START_POS_BALIZAS and y > START_POS_BALIZAS - LADO_MAIOR_AREA):
        comemoracao_golo(0,-50/2,50,estado_jogo)
        pont = estado_jogo['pontuacao_jogador_vermelho']
        pont = pont + 1
        estado_jogo['pontuacao_jogador_vermelho'] = pont
        guarda_ficheiro_var(estado_jogo)
        atualiza_posicoes_var(estado_jogo)
        update_board(estado_jogo)
        volta_inicio(estado_jogo)




def verifica_golo_jogador_azul(estado_jogo):
    '''
    Função responsável por verificar se um determinado jogador marcou golo.
    Para fazer esta verificação poderá fazer uso das constantes:
    LADO_MAIOR_AREA e
    START_POS_BALIZAS.
    Note que sempre que há um golo, deverá atualizar a pontuação do jogador,
    criar um ficheiro que permita fazer a análise da jogada pelo VAR,
    e reiniciar o jogo com a bola ao centro.
    O ficheiro para o VAR deverá conter todas as informações necessárias
    para repetir a jogada, usando as informações disponíveis no objeto
    estado_jogo['var']. O ficheiro deverá ter o nome

    replay_golo_jv_[TotalGolosJogadorVermelho]_ja_[TotalGolosJogadorAzul].txt

    onde [TotalGolosJogadorVermelho], [TotalGolosJogadorAzul]
    deverão ser substituídos pelo número de golos marcados pelo jogador vermelho
    e azul, respectivamente. Este ficheiro deverá conter 3 linhas, estruturadas
    da seguinte forma:
    Linha 1 - coordenadas da bola;
    Linha 2 - coordenadas do jogador vermelho;
    Linha 3 - coordenadas do jogador azul;
    w
    Em cada linha, os valores de xx e yy das coordenadas são separados por uma
    ',', e cada coordenada é separada por um ';'.

    '''
    bola = estado_jogo['bola']
    x = bola.xcor()
    y = bola.ycor()
    if (x - RAIO_BOLA <= -LARGURA_JANELA / 2 and y < START_POS_BALIZAS and y > START_POS_BALIZAS - LADO_MAIOR_AREA):
        comemoracao_golo(0, -50 / 2, 50, estado_jogo)
        pont = estado_jogo['pontuacao_jogador_azul']
        pont = pont + 1
        estado_jogo['pontuacao_jogador_azul'] = pont
        guarda_ficheiro_var(estado_jogo)
        atualiza_posicoes_var(estado_jogo)
        update_board(estado_jogo)
        volta_inicio(estado_jogo)


def verifica_golos(estado_jogo):
    verifica_golo_jogador_vermelho(estado_jogo)
    verifica_golo_jogador_azul(estado_jogo)


def calcular_angulo(x1, y1, x2, y2):
    return math.degrees(math.atan2(y2 - y1, x2 - x1))


def verifica_toque_jogador_azul(estado_jogo):
    '''
    Função responsável por verificar se o jogador tocou na bola.
    Sempre que um jogador toca na bola, deverá mudar a direção desta.
    '''
    bola = estado_jogo['bola']
    jogador = estado_jogo['jogador_azul']

    x_bola, y_bola = bola.xcor(), bola.ycor()
    x_jogador, y_jogador = jogador.xcor(), jogador.ycor()

    distancia = bola.distance(jogador)

    if distancia <= (RAIO_BOLA + RAIO_JOGADOR) + 12:
        angulo = calcular_angulo(x_bola, y_bola, x_jogador, y_jogador)
        novo_angulo = 180 + angulo
        bola.setheading(novo_angulo)
        bola.setx(x_bola + math.cos(math.radians(novo_angulo)))  # para nao tar sempre a colidir
        bola.sety(y_bola + math.sin(math.radians(novo_angulo)))  # para nao tar sempre a colidir
        guarda_posicoes_para_var(estado_jogo)


def verifica_toque_jogador_vermelho(estado_jogo):
    '''
    Função responsável por verificar se o jogador tocou na bola.
    Sempre que um jogador toca na bola, deverá mudar a direção desta.
    '''

    bola = estado_jogo['bola']
    jogador = estado_jogo['jogador_vermelho']

    x_bola, y_bola = bola.xcor(), bola.ycor()
    x_jogador, y_jogador = jogador.xcor(), jogador.ycor()

    distancia = bola.distance(jogador)

    if distancia <= (RAIO_BOLA + RAIO_JOGADOR) + 12:
        angulo = calcular_angulo(x_bola, y_bola, x_jogador, y_jogador)
        novo_angulo = 180 + angulo
        bola.setheading(novo_angulo)
        bola.setx(x_bola + math.cos(math.radians(novo_angulo)))  # para nao tar sempre a colidir
        bola.sety(y_bola + math.sin(math.radians(novo_angulo)))
        guarda_posicoes_para_var(estado_jogo)


def guarda_posicoes_para_var(estado_jogo):
    estado_jogo['var']['bola'].append(estado_jogo['bola'].pos())
    estado_jogo['var']['jogador_vermelho'].append(estado_jogo['jogador_vermelho'].pos())
    estado_jogo['var']['jogador_azul'].append(estado_jogo['jogador_azul'].pos())




def main():
    estado_jogo = init_state()
    setup(estado_jogo, True)
    while True:
        estado_jogo['janela'].update()
        if estado_jogo['bola'] is not None:
            movimenta_bola(estado_jogo)
        verifica_colisoes_ambiente(estado_jogo)
        verifica_golos(estado_jogo)
        if estado_jogo['jogador_vermelho'] is not None:
            verifica_toque_jogador_azul(estado_jogo)
        if estado_jogo['jogador_azul'] is not None:
            verifica_toque_jogador_vermelho(estado_jogo)


if __name__ == '__main__':
    main()