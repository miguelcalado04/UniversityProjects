#ifndef STRUCTS_H
#define STRUCTS_H
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define TAM 50


typedef struct data { // Cria uma STRUCT para armazenar os dados de uma data

    int dia;
    int mes;
    int ano;
    
}Data;

typedef struct hora { // Cria uma STRUCT para armazenar os dados de uma hora

    int hora;
    int minuto;
}Hora;

typedef struct cliente { // Cria uma STRUCT para armazenar os dados de um Cliente
    
    char nome[TAM];
    char apelido[TAM]; 
    int nif;
    
}Cliente;

typedef struct noLista {

    Cliente cliente;
    struct noLista * prox;

}noLista;

typedef noLista *pLista;



typedef struct marcacao { // Cria uma STRUCT para armazenar os dados de uma marcação

    int servico; //(0 para lavagem 1 para manutenção)
    Hora horas;
    Data data;
    pLista cliente;
}Marcacao;

typedef struct noListaMarcacao {

    Marcacao pessoareserva;
    struct noListaMarcacao * prox;

}noListaMarcacao;

typedef noListaMarcacao *pListaMarcacao;




pLista cria_listaClientes();
int vazia_listaClientes (pLista lista);
pLista destroi_listaClientes (pLista lista);
void procura_listaClientes (pLista lista, int chave, pLista *ant, pLista *actual);  
void elimina_listaClientes (pLista lista, int chave);
void insere_cliente (pLista lista, Cliente p1); 
void insere_cliente_do_ficheiro (pLista lista, Cliente p1);
pLista pesquisa_clientes (pLista lista, int chave);
void imprime_listaClientes (pLista lista);
int existe_nif(pLista lista, int nif);
void menu_add_cliente(pLista lista);
void mostra_lista_clientes(pLista lista);

pListaMarcacao cria_marcacao();
int vazia_marcacao (pListaMarcacao listar);
pListaMarcacao destroi_marcacao (pListaMarcacao listar);
void procura_marcacao (pListaMarcacao listar, int chave, int chave2, int chave3, int chave4, int chave5, pListaMarcacao *ant, pListaMarcacao *actual);
void procura_marcacao_do_insere_marcacoes_cliente (pListaMarcacao listar, int chave, int chave2, int chave3, int chave4, int chave5, pListaMarcacao *ant, pListaMarcacao *actual);
void elimina_marcacao (pListaMarcacao listar, int chave, int chave2, int chave3, int chave4, int chave5);
void elimina_do_insere_marcacao (pListaMarcacao listar, int chave, int chave2, int chave3, int chave4, int chave5);
void insere_marcacao (pListaMarcacao listar, Marcacao p1);
void insere_marcacoes_cliente (pListaMarcacao listar, Marcacao p1);
pListaMarcacao pesquisa_marcacao (pListaMarcacao listar, int chave, int chave2, int chave3, int chave4, int chave5);
void imprime_marcacao (pListaMarcacao listar);
int existe(pListaMarcacao listar, int servico, int minuto, int hora, int dia, int mes, int ano, int hora_auxiliar, int minutos_auxiliar, int hora_auxiliar2, int minutos_auxiliar2);
void cancelar_reserva(pListaMarcacao listar, pListaMarcacao listaPreReservas);
void cancelar_pre_reserva(pListaMarcacao lista);
void cancelar_realizar_servico(pListaMarcacao lista);
void menu_add_marcacao(pListaMarcacao listar, pLista lista, pListaMarcacao listaPreReservas);
void mostra_lista_marcacao(pListaMarcacao listar);
void listar_marcacao_cliente(pListaMarcacao listar, pListaMarcacao listaPreReservas);

void escreve_para_ficheiro_clientes(pLista lista);
void escreve_para_ficheiro_reservas(pListaMarcacao listar);
void escreve_para_ficheiro_prereservas(pListaMarcacao listaPreReservas);
void carrega_de_ficheiro_clientes(pLista lista);
void carrega_de_ficheiro_reservas(pListaMarcacao listar);
void carrega_de_ficheiro_prereservas(pListaMarcacao listaPreReservas);
int verifica_ficheiros(pLista lista, pListaMarcacao listar, pListaMarcacao listaPreReservas);



#endif
