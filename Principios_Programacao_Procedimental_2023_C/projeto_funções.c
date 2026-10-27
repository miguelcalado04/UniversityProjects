#include "projeto_struct.h"

/*--------- Lista De clientes --------------*/

pLista cria_listaClientes() {

    pLista aux;
    Cliente p1 = {"", "", 0};
    aux = (pLista) malloc (sizeof (noLista));
    if (aux != NULL) {
        aux->cliente = p1;
        aux->prox = NULL;
    }
    return aux;
}

int vazia_listaClientes (pLista lista) {

    if (lista->prox == NULL) return 1; //se a lista estiver vazia retorna 1 
    else return 0;
}

pLista destroi (pLista lista) {
    pLista temp_ptr;
    while (!vazia_listaClientes (lista)) {    //percorre a lista de clientes até estar vazia
        temp_ptr = lista;
        lista = lista->prox;
        free (temp_ptr);       
    }
    free(lista);
    return NULL;   //ou seja a lista foi completamente destruida
}

void procura_listaClientes (pLista lista, int chave, pLista *ant, pLista *actual){

    *ant = lista; *actual = lista->prox;
    while ((*actual) != NULL && (*actual)->cliente.nif < chave) {
        *ant = *actual;
        *actual = (*actual)->prox;
    }
    if ((*actual) != NULL && (*actual)->cliente.nif != chave)
        *actual = NULL; 
}

void elimina_listaClientes (pLista lista, int chave){

    pLista ant, actual;
    procura_listaClientes (lista, chave, &ant, &actual);   
    if (actual != NULL) {     //ou seja encontrou a chave logo procede á eliminaçao do nó
        ant->prox = actual->prox;
        free (actual);   //liberta o espaço de memoria
    }
}

void insere_cliente (pLista lista, Cliente p1){
    
    pLista no, ant, inutil;
    no = (pLista) malloc (sizeof (noLista));   //cria um novo nó 
    if (no != NULL) {  //se a alocação de meória for bem sucedida
        no->cliente = p1;    // p1 é associada ao campo cliente do nó
        procura_listaClientes(lista, p1.nif, &ant, &inutil);   //encontrar a posição onde colocar corretamente o nó
        no->prox = ant->prox;
        ant->prox = no;
    }
    printf("Cliente inserido com sucesso\n");
}

void insere_cliente_do_ficheiro (pLista lista, Cliente p1){
    
    pLista no, ant, inutil;
    no = (pLista) malloc (sizeof (noLista));
    if (no != NULL) {
        no->cliente = p1;
        procura_listaClientes(lista, p1.nif, &ant, &inutil);
        no->prox = ant->prox;
        ant->prox = no;
    }
}

pLista pesquisa_cliente (pLista lista, int chave){

    pLista ant, actual;
    procura_listaClientes (lista, chave, &ant, &actual);   //procura a chave
    return (actual); //se for enontrada devolve o seu valor

}

void imprime_listaClientes (pLista lista){

    pLista aux = lista->prox; /* Salta o header */
    while (aux) {   //enquanto o pnteiro não for NULL, dá print aos dados do nó
        printf("%s %s %d\n", aux->cliente.nome, aux->cliente.apelido, aux->cliente.nif);
        aux = aux->prox;
    }

}

void menu_add_cliente(pLista lista) {
    
    pLista aux =calloc(sizeof(noLista), 1);  //aramazena temporariamente as informações do cliente
    printf("Nome:");
    fgets(aux->cliente.nome, TAM, stdin);
    printf("Apelido:");
    fgets(aux->cliente.apelido, TAM, stdin);

    int count = 0;
    while (1) {
        printf("Digite o NIF: ");
        if (scanf("%d", &(aux->cliente.nif)) != 1) {    //se o nif contém letras
            printf("Erro: o valor digitado não é um número inteiro.\n");
            while (getchar() != '\n');  // Limpar o buffer de entrada
        
        } else {

            int temp = aux->cliente.nif;
            while (temp != 0) {
                temp /= 10;
                count++; //conta quantos digitos tem o nif
            }

            if (count != 9) {  //se não tiver 9 digitos
                printf("NIF incorreto, insira novamente.\n");
                count = 0;
            } else {
                if (existe_nif(lista, aux->cliente.nif)) {    //verifica se o nif já existe
                    printf("NIF já existe na lista de clientes.\n");
                    break;
                } else {
                    insere_cliente(lista, aux->cliente);    //se o nif não existir insere o cliente
                    break;  // NIF válido e não existe na lista, sair do loop
                }
            }
        }
    }
    getchar();
}

void mostra_lista_clientes(pLista lista) {

    imprime_listaClientes(lista);  
}



int existe_nif(pLista lista, int nif) {
    
    pLista aux = lista;   //atribui o ponteiro "lista" a um novo ponteiro "aux"
    
    while (aux != NULL) {
        if (aux->cliente.nif == nif) {
            return 1;  // O NIF já existe na lista
        }
        aux = aux->prox;  //passa para o próximo nó
    }
    
    return 0;  // O NIF não foi encontrado na lista
}


/*--------- Lista reserva e pre-reserva --------------*/

pListaMarcacao cria_marcacao() {

    pListaMarcacao listar = (pListaMarcacao)malloc(sizeof(struct noListaMarcacao));    //ponteiro para um novo nó da lista
    if (listar != NULL) {    //se a alocação de memória for bem sucedida
        listar->prox = NULL;   //prox vai ser NULL
    }

    return listar;  //devolve o ponteiro para o novo nó
}

int vazia_marcacao (pListaMarcacao listar) {

    if (listar->prox == NULL) return 1;   //verifica se o prox é NULL
    else return 0;
}

pListaMarcacao destroi_marcacao (pListaMarcacao listar) {
    
    pListaMarcacao temp_ptr;
    while (!vazia_marcacao (listar)) {      //percorre a lista
        temp_ptr = listar;
        listar = listar->prox;
        free (temp_ptr);
    }
    free(listar);
    return NULL;     //ou seja a lista foi completamente destruida
}

void procura_marcacao (pListaMarcacao listar, int chave, int chave2, int chave3, int chave4, int chave5, pListaMarcacao *ant, pListaMarcacao *actual){
                                         // p1.data.ano,p1.data.mes,p1.data.dia,p1.horas.hora,p1.horas.minuto
    
    *ant = listar; // Ponteiro para o nó anterior
    *actual = listar->prox; // Ponteiro para o nó atual
    while ((*actual != NULL && (*actual)->pessoareserva.data.ano > chave) ||
     (*actual != NULL && (*actual)->pessoareserva.data.ano == chave && (*actual)->pessoareserva.data.mes > chave2)|| 
     (*actual != NULL && (*actual)->pessoareserva.data.ano == chave && (*actual)->pessoareserva.data.mes == chave2 && (*actual)->pessoareserva.data.dia > chave3)||
     (*actual != NULL && (*actual)->pessoareserva.data.ano == chave && (*actual)->pessoareserva.data.mes == chave2 && (*actual)->pessoareserva.data.dia == chave3 && (*actual)->pessoareserva.horas.hora > chave4)||
     (*actual != NULL && (*actual)->pessoareserva.data.ano == chave && (*actual)->pessoareserva.data.mes == chave2 && (*actual)->pessoareserva.data.dia == chave3 && (*actual)->pessoareserva.horas.hora == chave4  && (*actual)->pessoareserva.horas.minuto > chave5)) {
        *ant = *actual; // Atualiza o ponteiro para o nó anterior
        *actual = (*actual)->prox; // Avança para o próximo nó
    }

    if (*actual != NULL && (*actual)->pessoareserva.data.ano != chave && (*actual)->pessoareserva.data.mes != chave2 && (*actual)->pessoareserva.data.dia != chave3 && (*actual)->pessoareserva.horas.hora != chave4 && (*actual)->pessoareserva.horas.minuto != chave5)  {
        *actual = NULL; // Reserva não encontrada, define o ponteiro como NULL
    }

}

void procura_marcacao_do_insere_marcacoes_cliente (pListaMarcacao listar, int chave, int chave2, int chave3, int chave4, int chave5, pListaMarcacao *ant, pListaMarcacao *actual){
                                        //p1.data.ano,p1.data.mes,p1.data.dia,p1.horas.hora,p1.horas.minuto

    *ant = listar; // Ponteiro para o nó anterior
    *actual = listar->prox; // Ponteiro para o nó atual
    while ((*actual != NULL && (*actual)->pessoareserva.data.ano < chave) ||
     (*actual != NULL && (*actual)->pessoareserva.data.ano == chave && (*actual)->pessoareserva.data.mes < chave2)|| 
     (*actual != NULL && (*actual)->pessoareserva.data.ano == chave && (*actual)->pessoareserva.data.mes == chave2 && (*actual)->pessoareserva.data.dia < chave3)||
     (*actual != NULL && (*actual)->pessoareserva.data.ano == chave && (*actual)->pessoareserva.data.mes == chave2 && (*actual)->pessoareserva.data.dia == chave3 && (*actual)->pessoareserva.horas.hora < chave4)||
     (*actual != NULL && (*actual)->pessoareserva.data.ano == chave && (*actual)->pessoareserva.data.mes == chave2 && (*actual)->pessoareserva.data.dia == chave3 && (*actual)->pessoareserva.horas.hora == chave4  && (*actual)->pessoareserva.horas.minuto < chave5)) {
        *ant = *actual; // Atualiza o ponteiro para o nó anterior
        *actual = (*actual)->prox; // Avança para o próximo nó
    }

    if (*actual != NULL && (*actual)->pessoareserva.data.ano != chave && (*actual)->pessoareserva.data.mes != chave2 && (*actual)->pessoareserva.data.dia != chave3 && (*actual)->pessoareserva.horas.hora != chave4 && (*actual)->pessoareserva.horas.minuto != chave5)  {
        *actual = NULL; // Reserva não encontrada, define o ponteiro como NULL
    }

}

void elimina_marcacao (pListaMarcacao listar, int chave, int chave2, int chave3, int chave4, int chave5){

    pListaMarcacao ant, actual;
    procura_marcacao (listar, chave, chave2, chave3, chave4, chave5, &ant, &actual);
    if (actual != NULL) {  //ou seja encontrou a chave logo procede á eliminaçao do nó
        ant->prox = actual->prox;
        free (actual);  //liberta o espaço de memoria
    }
} 

/*void elimina_do_insere_marcacao (pListaMarcacao listar, int chave, int chave2, int chave3, int chave4, int chave5){

    pListaMarcacao ant, actual;
    procura_marcacao_do_insere_marcacoes_cliente (listar, chave, chave2, chave3, chave4, chave5, &ant, &actual);
    if (actual != NULL) {
        ant->prox = actual->prox;
        free (actual);
    }
}*/

void insere_marcacao (pListaMarcacao listar, Marcacao p1){   

    pListaMarcacao no, ant, inutil;
    no = (pListaMarcacao) malloc (sizeof (noListaMarcacao));   //cria um novo nó
    if (no != NULL) {  // se a alocação de memoria for bem sucedida
        no->pessoareserva = p1;
        
        procura_marcacao (listar, p1.data.ano,p1.data.mes,p1.data.dia,p1.horas.hora,p1.horas.minuto, &ant, &inutil);
        no->prox = ant->prox;
        ant->prox = no;
    }
} 

void insere_marcacoes_cliente (pListaMarcacao listar, Marcacao p1){   

    pListaMarcacao no, ant, inutil;
    no = (pListaMarcacao) malloc (sizeof (noListaMarcacao));
    if (no != NULL) {
        no->pessoareserva = p1;
        
        procura_marcacao_do_insere_marcacoes_cliente (listar, p1.data.ano,p1.data.mes,p1.data.dia,p1.horas.hora,p1.horas.minuto, &ant, &inutil);
        no->prox = ant->prox;
        ant->prox = no;
    }
}

pListaMarcacao pesquisa_marcacao (pListaMarcacao listar, int chave, int chave2, int chave3, int chave4, int chave5){

    pListaMarcacao ant, actual;
    procura_marcacao (listar, chave, chave2, chave3, chave4, chave5, &ant, &actual);  
    return (actual);

}
/*pListaMarcacao pesquisa_do_insere_marcacoes_cliente (pListaMarcacao listar, int chave, int chave2, int chave3, int chave4, int chave5){

    pListaMarcacao ant, actual;
    procura_marcacao_do_insere_marcacoes_cliente (listar, chave, chave2, chave3, chave4, chave5, &ant, &actual);
    return (actual);

}*/

void imprime_marcacao (pListaMarcacao listar){

    pListaMarcacao aux = listar->prox; /* Salta o header */
    while (aux) {   //enquanto não chegar ao final da lista
        printf("%d %d %d %d:%d %d %s\n", aux->pessoareserva.data.dia, aux->pessoareserva.data.mes, aux->pessoareserva.data.ano,aux->pessoareserva.horas.hora, aux->pessoareserva.horas.minuto, aux->pessoareserva.servico, aux->pessoareserva.cliente->cliente.nome);
        aux = aux->prox;
    }
}

int existe(pListaMarcacao listar, int servico, int minuto, int hora, int dia, int mes, int ano, int hora_auxiliar, int minutos_auxiliar, int hora_auxiliar2, int minutos_auxiliar2){
    
    pListaMarcacao atual_aux = listar;
    while (atual_aux != NULL) {

            if (atual_aux->pessoareserva.horas.minuto == minutos_auxiliar &&
                atual_aux->pessoareserva.horas.hora == hora_auxiliar &&
                atual_aux->pessoareserva.data.dia == dia &&
                atual_aux->pessoareserva.data.mes == mes &&
                atual_aux->pessoareserva.data.ano == ano &&
                atual_aux->pessoareserva.servico == 1) {
                //verifica se tem reserva na meia hora anterior e se e uma reserva com manutencao  que dura 1 hora
                return 1; // Reserva encontrada
            }
            if (servico == 1){
            if (atual_aux->pessoareserva.horas.minuto == minutos_auxiliar2 &&
                atual_aux->pessoareserva.horas.hora == hora_auxiliar2 &&
                atual_aux->pessoareserva.data.dia == dia &&
                atual_aux->pessoareserva.data.mes == mes &&
                atual_aux->pessoareserva.data.ano == ano) {
                //verifica a meia-hora a seguir para saber se ja existe uma reserva
                return 1; // Reserva encontrada
            }
            }
            
             if (atual_aux->pessoareserva.horas.minuto == minuto &&
                atual_aux->pessoareserva.horas.hora == hora &&
                atual_aux->pessoareserva.data.dia == dia &&
                atual_aux->pessoareserva.data.mes == mes &&
                atual_aux->pessoareserva.data.ano == ano ) {
                //verifica a hora atual para o caso de ja existir uma reserva a hora que estamos a tentar inserir uma reserva
                return 1; // Reserva encontrada
            }
        atual_aux = atual_aux->prox;
    }
    return 0; // Reserva não encontrada
}

void cancelar_reserva(pListaMarcacao listar, pListaMarcacao listaPreReservas) {
    int dia, mes, ano, hora, minuto, servico, hora_auxiliar1, hora_auxiliar2, hora_auxiliar3, minutos_auxiliar1, minutos_auxiliar2, minutos_auxiliar3;
    int verifica_auxiliar = 0;
    int verifica = 0;
    int vazia3 = 0;
    int vazia2 = 0;
    int vazia = 0;
    pListaMarcacao ant = NULL;
    pListaMarcacao verifica_ant = NULL;
    pListaMarcacao verifica_ant2 = NULL;
    pListaMarcacao verifica_ant3 = NULL;
    pListaMarcacao verifica_ant4 = NULL;
    pListaMarcacao verifica_ant6 = NULL;
    pListaMarcacao verifica_ant7 = NULL;
    pListaMarcacao verifica_ant8 = NULL;
    pListaMarcacao atual = listar;
    pListaMarcacao atual2 = listar;
    pListaMarcacao verifica_30minutosdepois_de_1hora_actual = listar;
    pListaMarcacao verifica_30minutosantes_de_30mins_actual = listar;
    pListaMarcacao verifica_actual = listaPreReservas;
    pListaMarcacao verifica_actual2 = listaPreReservas;
    pListaMarcacao verifica_actual3 = listaPreReservas;
    pListaMarcacao verifica_actual4 = listaPreReservas;
    pListaMarcacao verifica_actual6 = listaPreReservas;
    pListaMarcacao verifica_actual7 = listaPreReservas;
    pListaMarcacao verifica_actual8 = listaPreReservas;
    pListaMarcacao comparacao_aux = listaPreReservas;
    pListaMarcacao comparacao_aux2 = listaPreReservas;
    pListaMarcacao comparacao_aux3 = listaPreReservas;
    pListaMarcacao comparacao_aux4 = listaPreReservas;
    pListaMarcacao comparacao_aux6 = listaPreReservas;
    pListaMarcacao comparacao_aux7 = listaPreReservas;
    pListaMarcacao comparacao_aux8 = listaPreReservas;
  
    printf("Insira os dados da reserva a ser cancelada:\n");
    printf("Serviço (0 para lavagem, 1 para manutenção): ");
    scanf("%d", &servico);
    printf("Insira o ano:\n");
    scanf("%d",&ano);
    printf("Insira o mes:\n");
    scanf("%d",&mes);
    printf("Insira o dia:\n");
    scanf("%d",&dia);
    printf("Hora(hh:mm):\n");
    scanf("%d:%d", &hora, &minuto);
    
   

    if (minuto == 30){ // a variavel minutos_auxiliar1 e hora_auxiliar1 toma o valor da meia hora anterior a reserva cancelada
        minutos_auxiliar1 = 0;
        hora_auxiliar1 = hora;

    }else{
        minutos_auxiliar1 = 30;
        hora_auxiliar1 = hora - 1;
    }

    if (minuto == 30){// a variavel minutos_auxiliar2 e hora_auxiliar2 toma o valor da meia hora seguinte a data de reserva cancelada
        minutos_auxiliar2 = 0;
        hora_auxiliar2 = hora + 1;

    }else{
        minutos_auxiliar2 = 30;
        hora_auxiliar2 = hora;
    }

    if (minutos_auxiliar2 == 30 ){// a variavel minutos_auxiliar1 e hora_auxiliar1 toma o valor da meia hora seguinte do fim da reserva cancelada
        minutos_auxiliar3 = 0;
        hora_auxiliar3 = hora_auxiliar2 + 1;

    }else{
        minutos_auxiliar3 = 30;
        hora_auxiliar3 = hora_auxiliar2;
    }

    // Procura a reserva na lista
    while (atual != NULL) {
        if (atual->pessoareserva.servico == servico &&
            atual->pessoareserva.data.dia == dia &&
            atual->pessoareserva.data.mes == mes &&
            atual->pessoareserva.data.ano == ano &&
            atual->pessoareserva.horas.hora == hora &&
            atual->pessoareserva.horas.minuto == minuto) {
            
            // Encontrou a reserva, remove da lista
            if (ant == NULL) {
                // O nó a ser removido é o primeiro da lista
                listar = atual->prox;
            } else {
                // O nó a ser removido está no meio ou no final da lista
                ant->prox = atual->prox;
            }
            
            free(atual);
            printf("Cancelada com sucesso!\n");
            
            if (servico == 1){
                vazia2 = 1;  //se a reserva cancelada for de uma hora a meia hora seguinte a hora da reserva que foi cancelada esta vazia
            }else{
                while(atual2 != NULL){ // se a reserva cancelada for de 30 minutos vai verificar os 30 minutos seguintes para verificar se tem 1 bloco de uma hora disponivel
                    if(atual2->pessoareserva.data.dia == dia &&
                      atual2->pessoareserva.data.mes == mes &&
                      atual2->pessoareserva.data.ano == ano &&
                      atual2->pessoareserva.horas.hora == hora_auxiliar2 &&
                       atual2->pessoareserva.horas.minuto == minutos_auxiliar2){
                        vazia2=1;
                    }
                    atual2 = atual2->prox;
                }
            }

            if(verifica == 0){//se ainda exisitirem horas disponiveis
            
                if (servico){ // se tiver-mos uma hora disponivel
                    if(verifica_auxiliar == 0){//1 bloco de 30 mins
                        

                        while (verifica_actual != NULL) {
                            //verificar se ha pre-reservas que sejam manutencao a comecar no 1 bloco de 30 minutos
                            if(verifica_actual->pessoareserva.data.dia == dia &&
                                verifica_actual->pessoareserva.data.mes == mes &&
                                verifica_actual->pessoareserva.data.ano == ano &&
                                verifica_actual->pessoareserva.horas.hora == hora &&
                                verifica_actual->pessoareserva.horas.minuto == minuto &&
                                verifica_actual->pessoareserva.servico == 1){
                                    
                                    comparacao_aux = verifica_actual->prox;

                                        while( comparacao_aux !=NULL ){ //como a lista de pre-reservas esta ordenada de modo que as mais recentes fiquem em primeiro lugar vamos verificar se a pre-reserva seguinte à pre-reserva encontrada também vai atender ás condições para poder ser inserida na lista(executa ate a pre-reserva seguinte á atual ser diferente)
                                            if(verifica_actual->pessoareserva.data.dia == comparacao_aux->pessoareserva.data.dia &&
                                                verifica_actual->pessoareserva.data.mes == comparacao_aux->pessoareserva.data.mes &&
                                                verifica_actual->pessoareserva.data.ano == comparacao_aux->pessoareserva.data.ano &&
                                                verifica_actual->pessoareserva.horas.hora == comparacao_aux->pessoareserva.horas.hora &&
                                                verifica_actual->pessoareserva.horas.minuto == comparacao_aux->pessoareserva.horas.minuto&&
                                                verifica_actual->pessoareserva.servico == comparacao_aux->pessoareserva.servico ){
                                                    verifica_actual = comparacao_aux;
                                            }
                                            comparacao_aux = comparacao_aux->prox;
                                        }

                                    // Insere a Pre-reserva na lista
                                    insere_marcacao(listar, verifica_actual->pessoareserva);
                                    printf("Inseriu uma pre-reserva na lista de reservas\n");
            
                                    if (verifica_ant == NULL) {
                                     // O nó a ser removido é o primeiro da lista
                                    listaPreReservas = verifica_actual->prox;
                                    } else {
                                        // O nó a ser removido está no meio ou no final da lista
                                        verifica_ant->prox = verifica_actual->prox;
                                    }

                                verifica = 1; //2 blocos de uma hora foram preenchidos se entrar no if

                            }
                            verifica_ant = verifica_actual;
                            verifica_actual = verifica_actual->prox;

                        }
                       
                    if(verifica == 0){ //se ainda não completou o tempo libertado pelo cancelamento da reserva
                        while(verifica_actual2 != NULL){ //verifica se ha alguma pre-reserva para inserir no primeiro bloco de 30 minutos dos 2 que estao disponiveis
                                if(verifica_actual2->pessoareserva.data.dia == dia &&
                                     verifica_actual2->pessoareserva.data.mes == mes &&
                                     verifica_actual2->pessoareserva.data.ano == ano &&
                                     verifica_actual2->pessoareserva.horas.hora == hora &&
                                    verifica_actual2->pessoareserva.horas.minuto == minuto &&
                                    verifica_actual2->pessoareserva.servico == 0){

                                         comparacao_aux2 = verifica_actual2->prox;

                                        while( comparacao_aux2 !=NULL ){ //como a lista de pre-reservas esta ordenada de modo que as mais recentes fiquem em primeiro lugar vamos verificar se a pre-reserva seguinte à pre-reserva encontrada também vai atender ás condições para poder ser inserida na lista(executa ate a pre-reserva seguinte á atual ser diferente)
                                            if(verifica_actual2->pessoareserva.data.dia == comparacao_aux2->pessoareserva.data.dia &&
                                                verifica_actual2->pessoareserva.data.mes == comparacao_aux2->pessoareserva.data.mes &&
                                                verifica_actual2->pessoareserva.data.ano == comparacao_aux2->pessoareserva.data.ano &&
                                                verifica_actual2->pessoareserva.horas.hora == comparacao_aux2->pessoareserva.horas.hora &&
                                                verifica_actual2->pessoareserva.horas.minuto == comparacao_aux2->pessoareserva.horas.minuto&&
                                                verifica_actual2->pessoareserva.servico == comparacao_aux2->pessoareserva.servico ){
                                                    verifica_actual2 = comparacao_aux2;
                                            }
                                            comparacao_aux2 = comparacao_aux2->prox;
                                        }

                                        
                                         // Insere a Pre-reserva na lista
                                         insere_marcacao(listar, verifica_actual2->pessoareserva);
                                         printf("Inseriu uma pre-reserva na lista de reservas\n");
                    
                                         if (verifica_ant2 == NULL) {
                                            // O nó a ser removido é o primeiro da lista
                                             listaPreReservas = verifica_actual2->prox;
                                        } else {
                                              // O nó a ser removido está no meio ou no final da lista
                                             verifica_ant2->prox = verifica_actual2->prox;
                                        }
                                        verifica_auxiliar = 1;
                                

                                    }
                                    verifica_ant2 = verifica_actual2;
                                    verifica_actual2 = verifica_actual2->prox;


                            
                            }
                    }
                    }

                    if (verifica_auxiliar==1){ // só executa se uma reserva de 30 minutos tiver sido inserida no primeiro bloco de 30 minutos dos 2 que estavam disponíveis
                            while (verifica_30minutosdepois_de_1hora_actual != NULL) { //verifica se existe alguma reserva na lista de reservas que esteja a ocupar os 30 minutos seguintes à hora disponivel
                            if(verifica_30minutosdepois_de_1hora_actual->pessoareserva.data.dia == dia &&
                                verifica_30minutosdepois_de_1hora_actual->pessoareserva.data.mes == mes &&
                                verifica_30minutosdepois_de_1hora_actual->pessoareserva.data.ano == ano &&
                                verifica_30minutosdepois_de_1hora_actual->pessoareserva.horas.hora == hora_auxiliar3 &&
                                verifica_30minutosdepois_de_1hora_actual->pessoareserva.horas.minuto == minutos_auxiliar3 &&
                                (verifica_30minutosdepois_de_1hora_actual->pessoareserva.servico == 0 || verifica_30minutosdepois_de_1hora_actual->pessoareserva.servico == 1)){
                                    vazia3 = 1;
                             }
                        verifica_30minutosdepois_de_1hora_actual = verifica_30minutosdepois_de_1hora_actual->prox;
                        }
                        

                        if(vazia3 == 0){ // se os 30 minutos seguintes aos 2 blocos de 30 minutos estao disponiveis
                            if (verifica == 0){//se ainda não completou o tempo libertado pelo cancelamento da reserva
                            
                            while (verifica_actual3 != NULL) { //verificar se ha uma reserva de 1 hora(manutencao) que comece no 2 bloco de 30 minutos 
                             if(verifica_actual3->pessoareserva.data.dia == dia &&
                                verifica_actual3->pessoareserva.data.mes == mes &&
                                verifica_actual3->pessoareserva.data.ano == ano &&
                                verifica_actual3->pessoareserva.horas.hora == hora_auxiliar2 &&
                                verifica_actual3->pessoareserva.horas.minuto == minutos_auxiliar2 &&
                                verifica_actual3->pessoareserva.servico == 1){

                                     comparacao_aux3 = verifica_actual3->prox;

                                        while( comparacao_aux3 !=NULL ){ //como a lista de pre-reservas esta ordenada de modo que as mais recentes fiquem em primeiro lugar vamos verificar se a pre-reserva seguinte à pre-reserva encontrada também vai atender ás condições para poder ser inserida na lista(executa ate a pre-reserva seguinte á atual ser diferente)
                                            if(verifica_actual3->pessoareserva.data.dia == comparacao_aux3->pessoareserva.data.dia &&
                                                verifica_actual3->pessoareserva.data.mes == comparacao_aux3->pessoareserva.data.mes &&
                                                verifica_actual3->pessoareserva.data.ano == comparacao_aux3->pessoareserva.data.ano &&
                                                verifica_actual3->pessoareserva.horas.hora == comparacao_aux3->pessoareserva.horas.hora &&
                                                verifica_actual3->pessoareserva.horas.minuto == comparacao_aux3->pessoareserva.horas.minuto&&
                                                verifica_actual3->pessoareserva.servico == comparacao_aux3->pessoareserva.servico ){
                                                    verifica_actual3 = comparacao_aux3;
                                            }
                                            comparacao_aux3 = comparacao_aux3->prox;
                                        }
                                // Insere a Pre-reserva na lista
                                insere_marcacao(listar, verifica_actual3->pessoareserva);
                                printf("Inseriu uma pre-reserva na lista de reservas1\n");
            
                                if (verifica_ant3 == NULL) {
                                    // O nó a ser removido é o primeiro da lista
                                    listaPreReservas = verifica_actual3->prox;
                                } else {
                                    // O nó a ser removido está no meio ou no final da lista
                                    verifica_ant3->prox = verifica_actual3->prox;
                                }
                        
                            verifica = 1;
                            }
                         
                        verifica_ant3 = verifica_actual3;
                        verifica_actual3 = verifica_actual3->prox;
                        }
                        }
                        
                        
                        }else{//so entra neste else se os 30 minutos a seguir ao bloco de 1 hora disponivel estiverem ocupados
                             if (verifica == 0){ //se ainda não completou o tempo libertado pelo cancelamento da reserva
                                 while (verifica_actual4 != NULL) { //verificar se ha uma reserva de 30 minutos(lavagem) que comece no 2 bloco de 30 minutos 
                                     if(verifica_actual4->pessoareserva.data.dia == dia &&
                                        verifica_actual4->pessoareserva.data.mes == mes &&
                                        verifica_actual4->pessoareserva.data.ano == ano &&
                                        verifica_actual4->pessoareserva.horas.hora == hora_auxiliar2 &&
                                        verifica_actual4->pessoareserva.horas.minuto == minutos_auxiliar2 &&
                                        verifica_actual4->pessoareserva.servico == 0){

                                             comparacao_aux4 = verifica_actual4->prox;

                                        while( comparacao_aux4 !=NULL ){ //como a lista de pre-reservas esta ordenada de modo que as mais recentes fiquem em primeiro lugar vamos verificar se a pre-reserva seguinte à pre-reserva encontrada também vai atender ás condições para poder ser inserida na lista(executa ate a pre-reserva seguinte á atual ser diferente)
                                            if(verifica_actual4->pessoareserva.data.dia == comparacao_aux4->pessoareserva.data.dia &&
                                                verifica_actual4->pessoareserva.data.mes == comparacao_aux4->pessoareserva.data.mes &&
                                                verifica_actual4->pessoareserva.data.ano == comparacao_aux4->pessoareserva.data.ano &&
                                                verifica_actual4->pessoareserva.horas.hora == comparacao_aux4->pessoareserva.horas.hora &&
                                                verifica_actual4->pessoareserva.horas.minuto == comparacao_aux4->pessoareserva.horas.minuto&&
                                                verifica_actual4->pessoareserva.servico == comparacao_aux4->pessoareserva.servico ){
                                                    verifica_actual4 = comparacao_aux4;
                                            }
                                            comparacao_aux4 = comparacao_aux4->prox;
                                        }
                                             // Insere a Pre-reserva na lista
                                            insere_marcacao(listar, verifica_actual4->pessoareserva);
                                            printf("Inseriu uma pre-reserva na lista de reservas2\n");
                                    
                                            if (verifica_ant4 == NULL) {
                                                // O nó a ser removido é o primeiro da lista
                                                listaPreReservas = verifica_actual4->prox;
                                            } else {
                                                 // O nó a ser removido está no meio ou no final da lista
                                                   verifica_ant4->prox = verifica_actual4->prox;
                                              }
                                                
                                        verifica = 1;
                                      }
                                                
                                  verifica_ant4 = verifica_actual4;
                                  verifica_actual4 = verifica_actual4->prox;
                                }
                            }
                        }
                        }
                      
                    }else{//se a reserva que foi cancelada foi uma lavagem (30 minutos)

                    if(vazia2 == 0){ //verifica se a hora seguinte ao bloco de 30 mins esta live
                        
                         while (verifica_actual6 != NULL) {
                            if(verifica==0){
                            /*verificar se ha pre-reservas que sejam manutencao(1 hora) a comecar no 1 bloco de 30 minutos*/
                            if(verifica_actual6->pessoareserva.data.dia == dia &&
                                verifica_actual6->pessoareserva.data.mes == mes &&
                                verifica_actual6->pessoareserva.data.ano == ano &&
                                verifica_actual6->pessoareserva.horas.hora == hora &&
                                verifica_actual6->pessoareserva.horas.minuto == minuto &&
                                verifica_actual6->pessoareserva.servico == 1){

                                     comparacao_aux6 = verifica_actual6->prox;

                                        while( comparacao_aux6 !=NULL ){ //como a lista de pre-reservas esta ordenada de modo que as mais recentes fiquem em primeiro lugar vamos verificar se a pre-reserva seguinte à pre-reserva encontrada também vai atender ás condições para poder ser inserida na lista(executa ate a pre-reserva seguinte á atual ser diferente)
                                            if(verifica_actual6->pessoareserva.data.dia == comparacao_aux6->pessoareserva.data.dia &&
                                                verifica_actual6->pessoareserva.data.mes == comparacao_aux6->pessoareserva.data.mes &&
                                                verifica_actual6->pessoareserva.data.ano == comparacao_aux6->pessoareserva.data.ano &&
                                                verifica_actual6->pessoareserva.horas.hora == comparacao_aux6->pessoareserva.horas.hora &&
                                                verifica_actual6->pessoareserva.horas.minuto == comparacao_aux6->pessoareserva.horas.minuto&&
                                                verifica_actual6->pessoareserva.servico == comparacao_aux6->pessoareserva.servico ){
                                                    verifica_actual6 = comparacao_aux6;
                                            }
                                            comparacao_aux6 = comparacao_aux6->prox;
                                        }

                                    // Insere a Pre-reserva na lista
                                    insere_marcacao(listar, verifica_actual6->pessoareserva);
                                    printf("Inseriu uma pre-reserva na lista de reservas\n");
            
                                    if (verifica_ant6 == NULL) {
                                     // O nó a ser removido é o primeiro da lista
                                    listaPreReservas = verifica_actual6->prox;
                                    } else {
                                        // O nó a ser removido está no meio ou no final da lista
                                        verifica_ant6->prox = verifica_actual6->prox;
                                    }

                                verifica = 1; //2 blocos de uma hora foram preenchidos se entrar no if

                            }
                            }
                            verifica_ant6 = verifica_actual6;
                            verifica_actual6 = verifica_actual6->prox;

                        }
                        

                    }
                    if (verifica_auxiliar==0){
                            while (verifica_30minutosantes_de_30mins_actual != NULL) { //verifica se a meia hora anterior aos 30 minutos disponiveis está ocupada
                            if(verifica_30minutosantes_de_30mins_actual->pessoareserva.data.dia == dia &&
                                verifica_30minutosantes_de_30mins_actual->pessoareserva.data.mes == mes &&
                                verifica_30minutosantes_de_30mins_actual->pessoareserva.data.ano == ano &&
                                verifica_30minutosantes_de_30mins_actual->pessoareserva.horas.hora == hora_auxiliar1 &&
                                verifica_30minutosantes_de_30mins_actual->pessoareserva.horas.minuto == minutos_auxiliar1 &&
                                (verifica_30minutosantes_de_30mins_actual->pessoareserva.servico == 0 || verifica_30minutosantes_de_30mins_actual->pessoareserva.servico == 1)){
                                    vazia = 1;
                             }
                            verifica_30minutosantes_de_30mins_actual = verifica_30minutosantes_de_30mins_actual->prox;
                         }
                        if(vazia == 0){ // se os 30 minutos anteriores aos 30 minutos disponiveis tiverem fazios
                            if (verifica == 0){ //se ainda não completou o tempo libertado pelo cancelamento da reserva
                            
                            while (verifica_actual8 != NULL) { //verifica se ha alguma pre-reserva do tipo manutencao(1 hora) que comece na hora anterior á reserva de 30 minutos que foi cancelada
                             if(verifica_actual8->pessoareserva.data.dia == dia &&
                                verifica_actual8->pessoareserva.data.mes == mes &&
                                verifica_actual8->pessoareserva.data.ano == ano &&
                                verifica_actual8->pessoareserva.horas.hora == hora_auxiliar1 &&
                                verifica_actual8->pessoareserva.horas.minuto == minutos_auxiliar1 &&
                                verifica_actual8->pessoareserva.servico == 1){

                                     comparacao_aux8 = verifica_actual8->prox;

                                        while( comparacao_aux8 !=NULL ){ //como a lista de pre-reservas esta ordenada de modo que as mais recentes fiquem em primeiro lugar vamos verificar se a pre-reserva seguinte à pre-reserva encontrada também vai atender ás condições para poder ser inserida na lista(executa ate a pre-reserva seguinte á atual ser diferente)
                                            if(verifica_actual8->pessoareserva.data.dia == comparacao_aux8->pessoareserva.data.dia &&
                                                verifica_actual8->pessoareserva.data.mes == comparacao_aux8->pessoareserva.data.mes &&
                                                verifica_actual8->pessoareserva.data.ano == comparacao_aux8->pessoareserva.data.ano &&
                                                verifica_actual8->pessoareserva.horas.hora == comparacao_aux8->pessoareserva.horas.hora &&
                                                verifica_actual8->pessoareserva.horas.minuto == comparacao_aux8->pessoareserva.horas.minuto&&
                                                verifica_actual8->pessoareserva.servico == comparacao_aux8->pessoareserva.servico ){
                                                    verifica_actual8 = comparacao_aux8;
                                            }
                                            comparacao_aux8 = comparacao_aux8->prox;
                                        }
                                // Insere a Pre-reserva na lista
                                insere_marcacao(listar, verifica_actual8->pessoareserva);
                                printf("Inseriu uma pre-reserva na lista de reservas\n");
            
                                if (verifica_ant8 == NULL) {
                                    // O nó a ser removido é o primeiro da lista
                                    listaPreReservas = verifica_actual8->prox;
                                } else {
                                    // O nó a ser removido está no meio ou no final da lista
                                    verifica_ant8->prox = verifica_actual8->prox;
                                }
                        
                            verifica = 1;
                            }
                         
                        verifica_ant8 = verifica_actual8;
                        verifica_actual8 = verifica_actual8->prox;
                        }
                        
                        }
                        
                     }
                    }
                         if(verifica == 0){ //se ainda não completou o tempo libertado pelo cancelamento da reserva

                           while (verifica_actual7 != NULL) { //verifica se ha alguma pre-reserva de 30 minutos a comecar à hora da reserva que foi cancelada
                             if(verifica_actual7->pessoareserva.data.dia == dia &&
                                verifica_actual7->pessoareserva.data.mes == mes &&
                                verifica_actual7->pessoareserva.data.ano == ano &&
                                verifica_actual7->pessoareserva.horas.hora == hora&&
                                verifica_actual7->pessoareserva.horas.minuto == minuto &&
                                verifica_actual7->pessoareserva.servico == 0){

                                     comparacao_aux7 = verifica_actual7->prox;

                                        while( comparacao_aux7 !=NULL ){ //como a lista de pre-reservas esta ordenada de modo que as mais recentes fiquem em primeiro lugar vamos verificar se a pre-reserva seguinte à pre-reserva encontrada também vai atender ás condições para poder ser inserida na lista(executa ate a pre-reserva seguinte á atual ser diferente)
                                            if(verifica_actual7->pessoareserva.data.dia == comparacao_aux7->pessoareserva.data.dia &&
                                                verifica_actual7->pessoareserva.data.mes == comparacao_aux7->pessoareserva.data.mes &&
                                                verifica_actual7->pessoareserva.data.ano == comparacao_aux7->pessoareserva.data.ano &&
                                                verifica_actual7->pessoareserva.horas.hora == comparacao_aux7->pessoareserva.horas.hora &&
                                                verifica_actual7->pessoareserva.horas.minuto == comparacao_aux7->pessoareserva.horas.minuto&&
                                                verifica_actual7->pessoareserva.servico == comparacao_aux7->pessoareserva.servico ){
                                                    verifica_actual7 = comparacao_aux7;
                                            }
                                            comparacao_aux7 = comparacao_aux7->prox;
                                        }
                                // Insere a Pre-reserva na lista
                                insere_marcacao(listar, verifica_actual7->pessoareserva);
                                printf("Inseriu uma pre-reserva na lista de reservas\n");
            
                                if (verifica_ant7 == NULL) {
                                    // O nó a ser removido é o primeiro da lista
                                    listaPreReservas = verifica_actual7->prox;
                                } else {
                                    // O nó a ser removido está no meio ou no final da lista
                                    verifica_ant7->prox = verifica_actual7->prox;
                                }
                        
                            verifica = 1;
                            }
                         
                        verifica_ant7 = verifica_actual7;
                        verifica_actual7 = verifica_actual7->prox;
                        }
                        }
                }
            }

        return;
        
        }
        ant = atual;
        atual = atual->prox;
        }
    
    
    // A pré-reserva não foi encontrada na lista
    printf("Não foi possivel encontrar a reserva.\n");
}



void cancelar_pre_reserva(pListaMarcacao lista) {
    int dia, mes, ano, hora, minuto;
    
    printf("Insira os dados da pré-reserva a ser cancelada:\n");
   // printf("Serviço (0 para lavagem, 1 para manutenção): ");
    //scanf("%d", &servico);
    printf("Insira o ano:\n");
    scanf("%d",&ano);
    printf("Insira o mes:\n");
    scanf("%d",&mes);
    printf("Insira o dia:\n");
    scanf("%d",&dia);
    printf("Hora(hh:mm):\n");
    scanf("%d:%d", &hora, &minuto);
    
    pListaMarcacao ant = NULL;
    pListaMarcacao atual = lista;
    
    // Procura a pré-reserva na lista
    while (atual != NULL) {
        if (atual->pessoareserva.data.dia == dia &&
            atual->pessoareserva.data.mes == mes &&
            atual->pessoareserva.data.ano == ano &&
            atual->pessoareserva.horas.hora == hora &&
            atual->pessoareserva.horas.minuto == minuto) {
            
            // Encontrou a pré-reserva, remove da lista
            if (ant == NULL) {
                // O nó a ser removido é o primeiro da lista
                lista = atual->prox;
            } else {
                // O nó a ser removido está no meio ou no final da lista
                ant->prox = atual->prox;
            }
            
            free(atual);
            printf("Cancelada com sucesso!\n");
            return;
        }
        
        ant = atual;
        atual = atual->prox;
    }
    
    // A pré-reserva não foi encontrada na lista
    printf("Não foi possivel encontrar a pre-reserva.\n");
}


void cancelar_realizar_servico(pListaMarcacao lista) {
    int dia, mes, ano, hora, minuto;
    
    printf("Insira os dados:\n");
    printf("Insira o ano:\n");
    scanf("%d",&ano);
    printf("Insira o mes:\n");
    scanf("%d",&mes);
    printf("Insira o dia:\n");
    scanf("%d",&dia);
    printf("Hora(hh:mm):\n");
    scanf("%d:%d", &hora, &minuto);
    
    pListaMarcacao ant = NULL;
    pListaMarcacao atual = lista;
    
    // Procura a pré-reserva na lista
    while (atual != NULL) {
        if (atual->pessoareserva.data.dia == dia &&
            atual->pessoareserva.data.mes == mes &&
            atual->pessoareserva.data.ano == ano &&
            atual->pessoareserva.horas.hora == hora &&
            atual->pessoareserva.horas.minuto == minuto) {
            
            // Encontrou a pré-reserva, remove da lista
            if (ant == NULL) {
                // O nó a ser removido é o primeiro da lista
                lista = atual->prox;
            } else {
                // O nó a ser removido está no meio ou no final da lista
                ant->prox = atual->prox;
            }
            
            free(atual);
            printf("Eliminado com sucesso!\n");
            return;
        }
        
        ant = atual;
        atual = atual->prox;
    }
    
    // A pré-reserva não foi encontrada na lista
    printf("Não foi possivel encontrar a reserva.\n");
}

void menu_add_marcacao(pListaMarcacao listar, pLista lista, pListaMarcacao listaPreReservas) {

    pListaMarcacao aux = calloc(sizeof(noListaMarcacao), 1);
    pLista ant, actual;
    int nifpessoa, minutos_auxiliar, hora_auxiliar,  minutos_auxiliar2, hora_auxiliar2;
    
    printf("Ano:");
    scanf("%d", &aux->pessoareserva.data.ano);

     while(aux->pessoareserva.data.ano < 2023){   //consideremos que as reservas so poderao ser feitas a partir do ano que estamos
        printf("Ano invalido insira novamente\n");
        printf("Ano:");
        scanf("%d", &aux->pessoareserva.data.ano);

    }
    printf("Mes:");
    scanf("%d", &aux->pessoareserva.data.mes);

        if (aux->pessoareserva.data.ano == 2023){  //verificar se o ano é 2023
            while (aux->pessoareserva.data.mes < 6){ // se for 2023 tem de ser maior que a data de hoje         
                printf("Mes invalido insira novamente\n");
                printf("Mes:");
                scanf("%d", &aux->pessoareserva.data.mes);
            }

        }else{
            while (aux->pessoareserva.data.mes < 1 || aux->pessoareserva.data.mes > 12){ // so ha 12 meses
                printf("Mes invalido insira novamente\n");
                printf("Mes:");
                scanf("%d", &aux->pessoareserva.data.mes);
            }
        }

    printf("Dia:");
    scanf("%d", &aux->pessoareserva.data.dia);

    if (aux->pessoareserva.data.mes== 2){

        if ((aux->pessoareserva.data.ano%4 == 0 && aux->pessoareserva.data.ano%100 != 0)|| aux->pessoareserva.data.ano%400 == 0 ){// se o ano for bissexto(fevreiro 29 dias)
            while (aux->pessoareserva.data.dia < 1 || aux->pessoareserva.data.dia > 29){
                printf("Dia invalido insira novamente\n");
                printf("Mes:");
                scanf("%d", &aux->pessoareserva.data.dia);
            }
        }else{ // se o ano nao for bissexto(fevereiro 28 dias)
            while (aux->pessoareserva.data.dia < 1 || aux->pessoareserva.data.dia > 28){
                printf("Dia invalido insira novamente\n");
                printf("Mes:");
                scanf("%d", &aux->pessoareserva.data.dia);
            }
        }

    }else{
        if (aux->pessoareserva.data.mes== 4 || aux->pessoareserva.data.mes== 6 || aux->pessoareserva.data.mes== 9 || aux->pessoareserva.data.mes== 11) { //meses com 30 dias
                while (aux->pessoareserva.data.dia < 1 || aux->pessoareserva.data.dia > 30){
                printf("Dia invalido insira novamente\n");
                printf("Dia:");
                scanf("%d", &aux->pessoareserva.data.dia);
            }

    }else{//meses com 31 dias
        while (aux->pessoareserva.data.dia < 1 || aux->pessoareserva.data.dia > 31){
                printf("Dia invalido insira novamente\n");
                printf("Dia:");
                scanf("%d", &aux->pessoareserva.data.dia);
            }
    }

    }
    
   printf("Insira o tipo de reserva (0-lavagem 1-manutenção): ");
    scanf("%d", &aux->pessoareserva.servico);
    while(aux->pessoareserva.servico != 0 && aux->pessoareserva.servico != 1) {   //verificar se nao digitu numeros sem ser o 0 ou 1
        printf("Serviço inválido, insira novamente\n");
        printf("Insira o tipo de reserva (0-lavagem 1-manutenção): ");
        scanf("%d", &aux->pessoareserva.servico);
    }
    printf("Horas (hh:mm):");
    scanf("%d:%d", &aux->pessoareserva.horas.hora, &aux->pessoareserva.horas.minuto);
    while(((aux->pessoareserva.horas.hora < 8 && aux->pessoareserva.horas.hora >= 18)       //horario funcional desde as 8:00 até ás 18:00
            || (aux->pessoareserva.horas.minuto != 00 && aux->pessoareserva.horas.minuto != 30)     //blocos de 30min
            || (aux->pessoareserva.horas.hora == 17 && aux->pessoareserva.horas.minuto == 30 && aux->pessoareserva.servico == 1))) { //se forem 17:30 e se manutenção(1hora) não pode marcar 

        printf("Hora inválida insira novamente\n");
        printf("Horas (hh:mm):");
        scanf("%d:%d", &aux->pessoareserva.horas.hora, &aux->pessoareserva.horas.minuto); 
    }

    printf("digite o seu NIF:");
    scanf("%d", &nifpessoa);

    if (aux->pessoareserva.horas.minuto == 30){// a variavel minutos_auxiliar e hora_auxiliar toma o valor da meia hora anterior á marcacao
        minutos_auxiliar = 0;
        hora_auxiliar = aux->pessoareserva.horas.hora;

    }else{
        minutos_auxiliar = 30;
        hora_auxiliar = aux->pessoareserva.horas.hora - 1;
    }


    if (aux->pessoareserva.horas.minuto == 30){ // a variavel minutos_auxiliar2 e hora_auxiliar2 toma o valor da meia hora seguinte á marcacao
        minutos_auxiliar2 = 0;
        hora_auxiliar2 = aux->pessoareserva.horas.hora + 1;

    }else{
        minutos_auxiliar2 = 30;
        hora_auxiliar2 = aux->pessoareserva.horas.hora;
    }

    printf("hora anterior:%d:%d\n", hora_auxiliar, minutos_auxiliar );
    printf("hora seguinte:%d:%d\n", hora_auxiliar2, minutos_auxiliar2 );


    int reserva_existe_na_lista = existe(listar,aux->pessoareserva.servico, aux->pessoareserva.horas.minuto, aux->pessoareserva.horas.hora, aux->pessoareserva.data.dia,aux->pessoareserva.data.mes, aux->pessoareserva.data.ano,  hora_auxiliar, minutos_auxiliar, hora_auxiliar2, minutos_auxiliar2);
    
    procura_listaClientes(lista, nifpessoa, &ant, &actual);
    if (actual !=NULL && actual->cliente.nif == nifpessoa) {
        aux->pessoareserva.cliente = actual;   //associar o aux ao actual
        
        if (reserva_existe_na_lista) {  //se retorna 1
            printf("A reserva ja existe na lista de reservas!\n");
                insere_marcacao(listaPreReservas, aux->pessoareserva);
                printf("Pre-reserva inserida com sucesso\n");
           

        } else {    //se retorna 0 insere reserva
            insere_marcacao(listar, aux->pessoareserva);
            printf("Reserva inserida com sucesso\n");
        }


    } else 
        printf("Cliente não encontrado!\n");

}

void mostra_lista_marcacao(pListaMarcacao listar) {

    imprime_marcacao(listar);  
}
void listar_marcacao_cliente(pListaMarcacao listar, pListaMarcacao listaPreReservas) {
    
    int nifCliente;
    printf("Insira o seu NIF:");
    scanf("%d", &nifCliente);
    
    pListaMarcacao auxReservas = cria_marcacao();
    pListaMarcacao auxPreReservas = cria_marcacao();
    
    // Percorre a lista de reservas
    pListaMarcacao reservaAtual = listar->prox;
    while (reservaAtual != NULL) {
        if (reservaAtual->pessoareserva.cliente->cliente.nif == nifCliente) {
            // Insere a reserva na lista auxiliar
            insere_marcacoes_cliente(auxReservas, reservaAtual->pessoareserva);
        }
        reservaAtual = reservaAtual->prox;
    }
    
    // Percorre a lista de pré-reservas
    pListaMarcacao preReservaAtual = listaPreReservas->prox;
    while (preReservaAtual != NULL) {
        if (preReservaAtual->pessoareserva.cliente->cliente.nif == nifCliente) {
            // Insere a pré-reserva na lista auxiliar
            insere_marcacoes_cliente(auxPreReservas, preReservaAtual->pessoareserva);
        }
        preReservaAtual = preReservaAtual->prox;
    }
    
    // Ordena a lista auxiliar por data (da mais recente para a mais antiga)
    // (Você precisa implementar essa função de ordenação ou usar uma biblioteca externa)
    
    // Imprime as reservas e pré-reservas do cliente
    printf("Reservas do cliente:\n");
    imprime_marcacao(auxReservas);
    printf("Pre-reservas do cliente:\n");
    imprime_marcacao(auxPreReservas);
    
    // Libera a memória da lista auxiliar
    destroi_marcacao(auxReservas);
    destroi_marcacao(auxPreReservas);
}


void escreve_para_ficheiro_clientes(pLista lista) {
    FILE *arquivo;
    pLista atual = lista->prox;

    arquivo = fopen("clientes.txt", "w"); // Abre o arquivo no modo de escrita, adicionando ao final

    if (arquivo == NULL) {
        printf("Erro ao abrir o arquivo.\n");
        return;
    }

    while (atual != NULL) {     
        fprintf(arquivo, "%s %s %d\n", atual->cliente.nome, atual->cliente.apelido, atual->cliente.nif);

        atual = atual->prox;
    }

    fclose(arquivo); // Fecha o arquivo
    //printf("Lista de clientes escrita com sucesso no arquivo.\n");
}

void escreve_para_ficheiro_reservas(pListaMarcacao listar) {
    FILE *arquivo;
    pListaMarcacao atual = listar->prox;/* Salta o header */

    arquivo = fopen("reservas.txt", "w"); // Abre o arquivo no modo de escrita, adicionando ao final

    if (arquivo == NULL) {
        printf("Erro ao abrir o arquivo.\n");
        return;
    }

    while (atual != NULL) {
            fprintf(arquivo, "%d %d %d %d:%d %d %s\n", atual->pessoareserva.data.dia, atual->pessoareserva.data.mes, atual->pessoareserva.data.ano, atual->pessoareserva.horas.hora, atual->pessoareserva.horas.minuto, atual->pessoareserva.servico, atual->pessoareserva.cliente->cliente.nome);

        atual = atual->prox;
    }

    fclose(arquivo); // Fecha o arquivo
    //printf("Lista de reservas escrita com sucesso no arquivo.\n");
}

void escreve_para_ficheiro_prereservas(pListaMarcacao listaPreReservas) {
    FILE *arquivo;
    pListaMarcacao atual = listaPreReservas->prox;/* Salta o header */

    arquivo = fopen("pre-reservas.txt", "w"); // Abre o arquivo no modo de escrita, adicionando ao final

    if (arquivo == NULL) {
        printf("Erro ao abrir o arquivo.\n");
        return;
    }

    while (atual != NULL) {
            fprintf(arquivo, "%d %d %d %d:%d %d %s\n", atual->pessoareserva.data.dia, atual->pessoareserva.data.mes, atual->pessoareserva.data.ano, atual->pessoareserva.horas.hora, atual->pessoareserva.horas.minuto, atual->pessoareserva.servico, atual->pessoareserva.cliente->cliente.nome);

        atual = atual->prox;
    }

    fclose(arquivo); // Fecha o arquivo
    printf("Dados gravados com sucesso.\n");
}


void carrega_de_ficheiro_clientes(pLista lista) {
    
    FILE* arquivo;
    char nome[100];
    char apelido[100];
    int nif;

    arquivo = fopen("clientes.txt", "r"); // Abre o arquivo no modo de leitura

    if (arquivo == NULL) {
        printf("Erro ao abrir o arquivo.\n");
        return;
    }

    while (fscanf(arquivo, "%s %s %d", nome, apelido, &nif) == 3) {   //ler os dados do arquivo e retorna o numero de elementos lidos com sucesso
        
        // alocando memória para uma nova estrutura Cliente

        Cliente* cliente = (Cliente*) malloc(sizeof(Cliente));
        if (cliente == NULL) {
            printf("Erro ao alocar memória para o cliente.\n");
            return;
        }
        //copiamos os dados lidos do arquivo para a estrutura do cliente
        strcpy(cliente->nome, nome);
        strcpy(cliente->apelido, apelido);
        cliente->nif = nif;
        insere_cliente_do_ficheiro(lista, *cliente);   //para passar o valor do cliente e não um ponteiro para o cliente
    }

    fclose(arquivo); // Fecha o arquivo
    printf("Dados dos clientes carregados com sucesso.\n");
}

void carrega_de_ficheiro_reservas(pListaMarcacao listar) {

    char nome[100];
    int dia, mes, ano, hora, minuto, servico;

    FILE* arquivo = fopen("reservas.txt", "r"); // Abre o arquivo no modo de leitura

    if (arquivo == NULL) {
        printf("Erro ao abrir o arquivo.\n");
        return;
    }

    while (fscanf(arquivo, "%d %d %d %d:%d %d %s\n", &dia, &mes, &ano, &hora, &minuto, &servico, nome) == 7) {
        // Alocando memória para uma nova estrutura Marcacao
        Marcacao* pessoareserva = (Marcacao*)malloc(sizeof(Marcacao));
        if (pessoareserva == NULL) {
            printf("Erro ao alocar memória para a marcação.\n");
            return;
        }

        // Alocando memória para uma nova estrutura Cliente
        pLista cliente = (pLista)malloc(sizeof(noLista));
        if (cliente == NULL) {
            printf("Erro ao alocar memória para o cliente.\n");
            free(pessoareserva);
            return;
        }

        // Copiando os dados lidos do arquivo para a estrutura de marcação
        pessoareserva->data.dia = dia;
        pessoareserva->data.mes = mes;
        pessoareserva->data.ano = ano;
        pessoareserva->horas.hora = hora;
        pessoareserva->horas.minuto = minuto;
        pessoareserva->servico = servico;
        strcpy(cliente->cliente.nome, nome);
        pessoareserva->cliente = cliente;

        insere_marcacao(listar, *pessoareserva); // Passando o valor da marcação e não um ponteiro para a marcação
    }

    fclose(arquivo); // Fecha o arquivo
    printf("Dados das reservas carregados com sucesso.\n");
}



void carrega_de_ficheiro_prereservas(pListaMarcacao listaPreReservas) {
    char nome[100];
    int dia, mes, ano, hora, minuto, servico;

    FILE* arquivo = fopen("pre-reservas.txt", "r"); // Abre o arquivo no modo de leitura

    if (arquivo == NULL) {
        printf("Erro ao abrir o arquivo.\n");
        return;
    }

    while (fscanf(arquivo, "%d %d %d %d:%d %d %s\n", &dia, &mes, &ano, &hora, &minuto, &servico, nome) == 7) {
        // Alocando memória para uma nova estrutura Marcacao
        Marcacao* pessoareserva = (Marcacao*)malloc(sizeof(Marcacao));
        if (pessoareserva == NULL) {
            printf("Erro ao alocar memória para a marcação.\n");
            return;
        }

        // Alocando memória para uma nova estrutura Cliente
        pLista cliente = (pLista)malloc(sizeof(noLista));
        if (cliente == NULL) {
            printf("Erro ao alocar memória para o cliente.\n");
            free(pessoareserva);
            return;
        }

        // Copiando os dados lidos do arquivo para a estrutura de marcação
        pessoareserva->data.dia = dia;
        pessoareserva->data.mes = mes;
        pessoareserva->data.ano = ano;
        pessoareserva->horas.hora = hora;
        pessoareserva->horas.minuto = minuto;
        pessoareserva->servico = servico;
        strcpy(cliente->cliente.nome, nome);
        pessoareserva->cliente = cliente;

        insere_marcacao(listaPreReservas, *pessoareserva); // Passando o valor da marcação e não um ponteiro para a marcação
    }

    fclose(arquivo); // Fecha o arquivo
    printf("Dados das pré-reservas carregados com sucesso.\n");
}



int verifica_ficheiros(pLista lista, pListaMarcacao listar, pListaMarcacao listaPreReservas) {

    FILE* arquivo_clientes = fopen("clientes.txt", "r"); // Abre o arquivo no modo de leitura
    FILE* arquivo_reservas = fopen("reservas.txt", "r"); // Abre o arquivo no modo de leitura
    FILE* arquivo_prereservas = fopen("pre-reservas.txt", "r"); // Abre o arquivo no modo de leitura

    /*---------Clientes---------*/

    if (arquivo_clientes == NULL) {
        printf("Erro ao abrir o arquivo.\n");
        return -1; // Retorna um valor indicando erro
    }

    fseek(arquivo_clientes, 0, SEEK_END); // Move o cursor para o final do arquivo

    long tamanho = ftell(arquivo_clientes); // Obtém a posição atual do cursor (tamanho do arquivo)

    fclose(arquivo_clientes);

        if (tamanho == 0) {
            printf("Nao ha dados de clientes para carregar\n");

        }else{
            carrega_de_ficheiro_clientes(lista);//carrega sempre as informacoes do ficheiro para as listas sempre que e iniciado
        }




    /*-------Reservas-------*/

    if (arquivo_reservas == NULL) {
        printf("Erro ao abrir o arquivo.\n");
    return -1; // Retorna um valor indicando erro
    }

    fseek(arquivo_reservas, 0, SEEK_END); // Move o cursor para o final do arquivo

    long size = ftell(arquivo_reservas); // Obtém a posição atual do cursor (tamanho do arquivo)

    fclose(arquivo_reservas);

    if (size == 0) {
            printf("Nao ha dados de reservas para carregar\n");
    } else {
            carrega_de_ficheiro_reservas(listar);    }



    /*---------Pré-reservas--------*/
    
    if (arquivo_prereservas == NULL) {
        printf("Erro ao abrir o arquivo.\n");
        return -1; // Retorna um valor indicando erro
    }

    fseek(arquivo_prereservas, 0, SEEK_END); // Move o cursor para o final do arquivo

    long size1 = ftell(arquivo_prereservas); // Obtém a posição atual do cursor (tamanho do arquivo)

    fclose(arquivo_prereservas);

    if (size1 == 0) {
            printf("Nao ha dados de pré-reservas para carregar\n");
    } else {
            carrega_de_ficheiro_prereservas(listaPreReservas);    }

    return 0;
}