import foosball_alunos
import time

def le_replay(nome_ficheiro):
    replay_data = dict()
    with open(nome_ficheiro, "r") as f:
        linhas = f.readlines()
    for linha in linhas:
        linha.strip('\n')
    var_bola = linhas[0].split(';')
    var_vermelho = linhas[1].split(';')
    var_azul = linhas[2].split(';')
    posicoesb = []
    for pos in var_bola:
        x, y = map(float, pos.split(','))
        posicoesb.append((x, y))
    posicoesv = []
    for pos in var_vermelho:
        x, y = map(float, pos.split(','))
        posicoesv.append((x, y))
    posicoesa = []
    for pos in var_azul:
        x, y = map(float, pos.split(','))
        posicoesa.append((x, y))
    replay_data['bola'] = posicoesb
    replay_data['jogador_vermelho'] = posicoesv
    replay_data['jogador_azul'] = posicoesa

    return replay_data



def main():
    estado_jogo = foosball_alunos.init_state()
    foosball_alunos.setup(estado_jogo, False)
    replay = le_replay('replay_golo_jv_1_ja_0.txt')
    bola = estado_jogo['bola']
    jv = estado_jogo['jogador_vermelho']
    ja = estado_jogo['jogador_azul']
    for i in range(len(replay['bola'])):
        estado_jogo['janela'].update()
        bola.goto(replay['bola'][i][0],replay['bola'][i][1])
        jv.goto(replay['jogador_vermelho'][i][0],replay['jogador_vermelho'][i][1])
        ja.goto(replay['jogador_azul'][i][0],replay['jogador_azul'][i][1])

    estado_jogo['janela'].exitonclick()


if __name__ == '__main__':
    main()