import foosball_alunos

def le_replay(nome_ficheiro):
    replay_data = dict()

    with open(nome_ficheiro) as f:
        linhas = f.readlines()

    linhas = [linha.strip('\n') for linha in linhas]

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
    for i in range(len(replay['bola'])-1):
        estado_jogo['janela'].update()
        estado_jogo['bola'].setx(replay['bola'][i][0])
        estado_jogo['bola'].sety(replay['bola'][i][1])
        estado_jogo['jogador_vermelho'].setx(replay['jogador_vermelho'][i][0])
        estado_jogo['jogador_vermelho'].sety(replay['jogador_vermelho'][i][1])
        estado_jogo['jogador_azul'].setx(replay['jogador_azul'][i][0])
        estado_jogo['jogador_azul'].sety(replay['jogador_azul'][i][1])

    estado_jogo['janela'].exitonclick()


if __name__ == '__main__':
    main()