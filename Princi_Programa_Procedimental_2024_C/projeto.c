#include "projeto_struct.h"
#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>

int main() {



    pLista listaDoentes= cria_listaDoentes();
    insere_doentes_do_ficheiro(listaDoentes, "doentes.txt", "registos.txt");

   
    int opcao = 0; // inicializacao do int opcao
    while (opcao != 7){ 
        printf("\n");
        printf("--------------------------------------------------------------------------\n");
        printf("|                        \033[0;31mHospital AM\033[0m                                     |\n");
        printf("|          \033[0;36m                MENU       \033[0m                                   |\n");
        printf("|                                                                        |\n"); 
        printf("|          \033[0;32m  1 - Adicionar Doente           \033[0m                             |\n");
        printf("|                                                                        |\n");
        printf("|          \033[0;32m  2 - Remover Doente             \033[0m                             |\n");
        printf("|                                                                        |\n");
        printf("|          \033[0;32m  3 - Mostar lista de Doentes por ordem alfabetica  \033[0m          |\n");
        printf("|                                                                        |\n");
        printf("|          \033[0;32m  4 - Mostrar Doentes com tensões máximas maior que   \033[0m        |\n");
        printf("|                                                                        |\n");
        printf("|          \033[0;32m  5 - Registar tensão, peso e altura de um Doente  \033[0m           |\n");
        printf("|                                                                        |\n");
        printf("|          \033[0;32m  6 - Mostar informações Doente  \033[0m                             |\n"); 
        printf("|                                                                        |\n");
        printf("|          \033[0;32m  7 - SAIR \033[0m                                                   |\n");
        printf("--------------------------------------------------------------------------\n"); 
       
        
        scanf("%d", &opcao);
        getchar();
        

            switch (opcao) {
            case 1:
                menu_add_doente(listaDoentes); 
                break;

            case 2:
                remover_doente(listaDoentes);
                break;

            case 3:
                imprime_listaDoentes(listaDoentes);
                break;

            case 4:
                listar_doentes_ta(listaDoentes);
                break;

            case 5:
                registar_dados_doente(listaDoentes);
                break;

            case 6:
                imprimir_dados_doente(listaDoentes);             
                break;

            case 7:
                guarda_no_ficheiro(listaDoentes);
                printf("Programa a encerrar!\n");
                for(int i = 3; i > 0; i--){
                    sleep(1);
                    printf("%d\n", i);
                    
                }
                sleep(1);
                printf("Programa encerrado!\n");
                break;
            default:
                printf("Opção inválida\n");
                break;
            }
    }

}