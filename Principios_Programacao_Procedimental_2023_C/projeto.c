#include "projeto_struct.h"
#include <stdlib.h>
#include <stdio.h>

int main() {

    pLista listaClientes= cria_listaClientes();
    pListaMarcacao listaReservas = cria_marcacao();    //listaReservas
    pListaMarcacao listaPreReservas = cria_marcacao();  //listaPreReserva

    verifica_ficheiros(listaClientes, listaReservas, listaPreReservas);  /*verifica se os ficheiros estão vazios ou não
                                                            e se não estiveram carrega para as listas os seus dados automaticamente*/

    int opcao = 0; // inicializacao do int opcao
    while (opcao != 12){ //(opcao<1 || opcao>11)
        printf("\n");
        printf("-------------------------------------------------------\n");
        printf("|                    \033[0;31mOFICINA IM\033[0m                       |\n");
        printf("|          \033[0;36m             MENU       \033[0m                   |\n");
        printf("|                                                     |\n");
        printf("|          \033[0;32m  1 - Adicionar cliente          \033[0m          |\n");
        printf("|                                                     |\n");
        printf("|          \033[0;32m  2 - Mostrar lista de clientes  \033[0m          |\n");
        printf("|                                                     |\n");
        printf("|          \033[0;32m  3 - Adicionar reserva          \033[0m          |\n");
        printf("|                                                     |\n");
        printf("|          \033[0;32m  4 - Mostrar lista de reservas  \033[0m          |\n");
        printf("|                                                     |\n");
        printf("|          \033[0;32m  5 - Mostar lista pre-reservas  \033[0m          |\n");
        printf("|                                                     |\n");
        printf("|          \033[0;32m  6 - Cancelar reserva           \033[0m          |\n");
        printf("|                                                     |\n");
        printf("|          \033[0;32m  7 - Cancelar pre-reserva       \033[0m          |\n");
        printf("|                                                     |\n");
        printf("|          \033[0;32m  8 - Ver reservas/pre-reservas do Cliente \033[0m|\n");
        printf("|                                                     |\n");
        printf("|          \033[0;32m  9 - Realizar servico           \033[0m          |\n");
        printf("|                                                     |\n");
        printf("|          \033[0;32m  10 - Gravar para ficheiro    \033[0m            |\n");
        printf("|                                                     |\n");
        printf("|          \033[0;32m  11 - Carregar do ficheiro    \033[0m            |\n");
        printf("|                                                     |\n");
        printf("|          \033[0;32m  12 - SAIR \033[0m                               |\n");
        printf("-------------------------------------------------------\n"); 
       
        scanf("%d", &opcao);
        getchar();
        

            switch (opcao) {
            case 1:
                menu_add_cliente(listaClientes); //funcoes.c
                break;

            case 2:
                mostra_lista_clientes(listaClientes); //funcoes.c
                break;

            case 3:
                menu_add_marcacao(listaReservas, listaClientes, listaPreReservas); //funcoes.c
                break;

            case 4:
                mostra_lista_marcacao(listaReservas); //funcoes.c
                break;

            case 5:
                mostra_lista_marcacao(listaPreReservas);//funcoes.c
                break;

            case 6:
                cancelar_reserva(listaReservas, listaPreReservas); //funcoes.c               
                break;

            case 7:
                cancelar_pre_reserva(listaPreReservas); //funcoes.c
                break;
            
            case 8:
                listar_marcacao_cliente(listaReservas,listaPreReservas); //funcoes.c
                break;

            case 9:
                cancelar_realizar_servico(listaReservas); //funcoes.c
                break;
            
            case 10:
                escreve_para_ficheiro_clientes(listaClientes);
                escreve_para_ficheiro_reservas(listaReservas); //funcoes.c
                escreve_para_ficheiro_prereservas(listaPreReservas);
                break;

            case 11:
                carrega_de_ficheiro_clientes(listaClientes);
                carrega_de_ficheiro_reservas(listaReservas); //funcoes.c
                carrega_de_ficheiro_prereservas(listaPreReservas);
                break;
    
            case 12:
                break;
            default:
                break;
            }
    }

}