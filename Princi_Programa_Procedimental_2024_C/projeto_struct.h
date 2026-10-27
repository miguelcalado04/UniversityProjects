#ifndef STRUCTS_H
#define STRUCTS_H
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define TAM 50

typedef struct data {
    int dia;
    int mes;
    int ano;
} Data;

typedef struct registo_dados {
    Data data;
    float ta_min;
    float ta_max;
    float peso;
    int altura;
    
} RegistoDados;

typedef struct noRegisto {
    RegistoDados registo;
    struct noRegisto *prox;
} NoRegisto;

typedef struct lista_dados {
    NoRegisto *inicio; // Ponteiro para o primeiro 
    NoRegisto *fim;    // Ponteiro para o último 
    struct lista_dados *prox; // Ponteiro para a próxima lista de dados
} ListaDados;



typedef struct doente {
    char nome[TAM];
    Data nascimento;
    int id;
    int cc;
    int telefone;
    char email[TAM];
    ListaDados *lista_dados;

}Doente;

typedef struct noLista {

    Doente doente;
    struct noLista * prox;

}noLista;

typedef noLista *pLista;


pLista cria_listaDoentes();
void insere_doentes_do_ficheiro(pLista lista, char *nome_ficheiro, char *ficheiro_registos);
void guarda_no_ficheiro(pLista lista);
int id_maxi(pLista lista);//
pLista procurar_pessoa_por_id(pLista lista, int id);//
void insere_doente (pLista lista, Doente d1);
pLista pesquisa_doente (pLista lista, int chave);
void imprime_listaDoentes (pLista lista);//
void menu_add_doente(pLista lista);//
void remover_doente(pLista lista) ;//
void swap(noLista **a, noLista **b);
void listar_doentes_ta(pLista lista);
void ordenaABC(pLista* lista);
void ordena_listaDoentes(pLista lista);
void imprimir_dados_doente(pLista lista);//
void registar_dados_doente(pLista lista);//


#endif
