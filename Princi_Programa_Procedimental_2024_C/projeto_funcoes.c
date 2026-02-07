#include "projeto_struct.h"
#include <string.h>
#include <ctype.h>
#define TAM 50


pLista cria_listaDoentes() {

    pLista aux;
    aux = (pLista) malloc (sizeof (noLista));
    if (aux != NULL) {
        aux->prox = NULL;
    }
    return aux;
}

void insere_doente(pLista lista, Doente d1) {
    pLista no, ant, atual;
    no = (pLista) malloc (sizeof (noLista));   // cria um novo nó 
    if (no != NULL) {  // se a alocação de memória for bem sucedida
        no->doente = d1;    // d1 é associada ao campo doente do nó
        
        ant = lista;
        atual = lista->prox;
        // Encontra a posição onde o novo doente deve ser inserido
        while (atual != NULL && strcasecmp(atual->doente.nome, d1.nome) < 0) {
            ant = atual;
            atual = atual->prox;
        }
        // Insere o novo doente na posição encontrada
        no->prox = ant->prox;
        ant->prox = no;
        
        printf("Doente inserido com sucesso\n");
    }
}

pLista procurar_pessoa_por_id(pLista lista, int id) {
    pLista atual = lista;
    while (atual != NULL) {
        if (atual->doente.id == id) {
            return atual;
        }
        atual = atual->prox;
    }
    return NULL; // Paciente não encontrado
}


void insere_doentes_do_ficheiro(pLista lista, char *nome_ficheiro, char *ficheiro_registos) {
    FILE *ficheiro = fopen(nome_ficheiro, "r");
    FILE *reg = fopen(ficheiro_registos, "r");
    if (ficheiro == NULL) {
        printf("Não se consegue abrir o ficheiro ou ele não existe.\n");
        return;
    }
    if (reg == NULL) {
        printf("Não se consegue abrir o ficheiro ou ele não existe.\n");
        return;
    }

    char line[100];
    Doente novo_doente;
    Data data; // Variável para armazenar a data temporariamente

    while (fgets(line, sizeof(line), ficheiro)) {
        novo_doente.id = atoi(line); // Lê e converte o ID diretamente da linha

        fgets(line, sizeof(line), ficheiro);
        line[strcspn(line, "\n")] = '\0'; // Remove o caractere de nova linha
        strcpy(novo_doente.nome, line); // Copia o nome diretamente da linha

        fgets(line, sizeof(line), ficheiro);
        sscanf(line, "%d/%d/%d", &data.dia, &data.mes, &data.ano); // Lê e armazena a data

        fgets(line, sizeof(line), ficheiro);
        novo_doente.cc = atoi(line);

        fgets(line, sizeof(line), ficheiro);
        novo_doente.telefone = atoi(line);

        fgets(line, sizeof(line), ficheiro);
        line[strcspn(line, "\n")] = '\0'; // Remove o caractere de nova linha
        strcpy(novo_doente.email, line);

        novo_doente.nascimento = data;
        novo_doente.lista_dados = NULL;
        insere_doente(lista, novo_doente);
    }



while (fgets(line, sizeof(line), reg)) {
    RegistoDados dados;

    int id = atoi(line); 
    pLista paciente = procurar_pessoa_por_id(lista, id);

    fgets(line, sizeof(line), reg);
    sscanf(line, "%d/%d/%d", &dados.data.dia, &dados.data.mes, &dados.data.ano); 

    fgets(line, sizeof(line), reg);
    dados.ta_max = atof(line);
     
    fgets(line, sizeof(line), reg);
    dados.ta_min = atof(line);

    fgets(line, sizeof(line), reg);
    dados.peso = atof(line);

    fgets(line, sizeof(line), reg);
    dados.altura  = atoi(line);

    if(paciente == NULL){
        printf("O paciente com o id:%d não existe logo não é possível associar-lhe o resgisto do ficheiro", id);

    }else{
        // Criar um novo nó para armazenar os dados
        NoRegisto *novo_registo = (NoRegisto *)malloc(sizeof(NoRegisto));
        if (novo_registo == NULL) {
            printf("Erro ao alocar memória para os dados do paciente.\n");
            return;
        }
        novo_registo->registo = dados;
        novo_registo->prox = NULL;

        // Adicionar o novo registo à lista de dados do paciente
        if (paciente->doente.lista_dados == NULL) {
            paciente->doente.lista_dados = (ListaDados *)malloc(sizeof(ListaDados));
            if (paciente->doente.lista_dados == NULL) {
                printf("Erro ao alocar memória para a lista de dados do paciente.\n");
                free(novo_registo);
                return;
            }
            paciente->doente.lista_dados->inicio = novo_registo;
            paciente->doente.lista_dados->fim = novo_registo;
        } else {
            NoRegisto *atual = paciente->doente.lista_dados->inicio;
            NoRegisto *anterior = NULL;

            // Encontrar a posição correta para inserção
            while (atual != NULL && atual->registo.ta_max > dados.ta_max) {
                anterior = atual;
                atual = atual->prox;
            }

            if (anterior == NULL) {
                // Inserir no início da lista
                novo_registo->prox = paciente->doente.lista_dados->inicio;
                paciente->doente.lista_dados->inicio = novo_registo;
            } else {
                // Inserir entre anterior e atual
                anterior->prox = novo_registo;
                novo_registo->prox = atual;
            }

            // Atualizar o ponteiro 'fim' se necessário
            if (atual == NULL) {
                paciente->doente.lista_dados->fim = novo_registo;
            }
        }
    }

}
    printf("Dados registados com sucesso\n");

    fclose(ficheiro);
    fclose(reg);
}


void guarda_no_ficheiro(pLista lista) {
    FILE *ficheiro = fopen("doentes.txt", "w");
    FILE *reg = fopen("registos.txt", "w");
    
    if (ficheiro == NULL) {
        printf("Não se consegue abrir o ficheiro doentes.txt\n");
        if (reg != NULL) fclose(reg);
        return;
    }
    if (reg == NULL) {
        printf("Não se consegue abrir o ficheiro registos.txt\n");
        if (ficheiro != NULL) fclose(ficheiro);
        return;
    }

    pLista doente_aux = lista;
    while (doente_aux->prox != NULL) {
        doente_aux = doente_aux->prox;
        fprintf(ficheiro, "%d\n%s\n%d/%d/%d\n%d\n%d\n%s\n", 
            doente_aux->doente.id,
            doente_aux->doente.nome,
            doente_aux->doente.nascimento.dia, 
            doente_aux->doente.nascimento.mes, 
            doente_aux->doente.nascimento.ano,
            doente_aux->doente.cc,
            doente_aux->doente.telefone,
            doente_aux->doente.email);
    }

    pLista doente_atual = lista;
    
    while (doente_atual->prox != NULL) {
        ListaDados *lista_dados = doente_atual->prox->doente.lista_dados;
        if (lista_dados != NULL) {
            NoRegisto *atual = lista_dados->inicio;
            while (atual != NULL) {
                fprintf(reg,"%d\n%d/%d/%d\n%f\n%f\n%f\n%d\n",
                doente_atual->prox->doente.id,
                atual->registo.data.dia,
                atual->registo.data.mes,
                atual->registo.data.ano,
                atual->registo.ta_max,
                atual->registo.ta_min,
                atual->registo.peso,
                atual->registo.altura);
                atual = atual->prox;
            }
        }
        doente_atual= doente_atual->prox;
       
    }

    printf("Doentes guardados com sucesso no ficheiro\n");
    printf("Registos guardados com sucesso no ficheiro\n");
  
    

    fclose(ficheiro);
    fclose(reg);
}

void listar_doentes_ta(pLista lista) {
    float tensao;
    printf("Digite o valor de tensão máxima (ta_max) para filtrar: ");
    if (scanf("%f", &tensao) != 1) {     //Barreira para o user
        printf("Erro: o valor digitado não é um float.\n");
        while (getchar() != '\n');  // Limpar o buffer de entrada
        return;
    }

    noLista *doente_aux = lista;          //inicializa-se a lista
        while (doente_aux != NULL) {        //percorrer a lista
        ListaDados *listaDados = doente_aux->doente.lista_dados;   //inicia-se a lista de dados, para acesso á tensão
        if (listaDados) {
            NoRegisto *reg = listaDados->inicio;              //caso a lista de dados exista, colocamos o ponteiro no inicio, para iniciar a comparação
            while (reg != NULL) {                       
                if (reg->registo.ta_max > tensao) {
                    printf("Nome: %s, cc: %d, Tensão Máxima: %.2f\n",doente_aux->doente.nome,doente_aux->doente.cc,reg->registo.ta_max);
                }
                reg = reg->prox;      //avança-se com o registos
            }
        }
        doente_aux = doente_aux->prox;    //avança-se com os doentes
    }
}


int existe_cc(pLista lista, int cc) {
    
    pLista aux = lista;   //atribui o ponteiro "lista" a um novo ponteiro "aux"
    
    while (aux != NULL) {
        if (aux->doente.cc == cc) {
            return 1;  // o cc já existe na lista
        }
        aux = aux->prox;  //passa para o próximo nó
    }
    
    return 0;  // o cc não foi encontrado na lista
}

int id_maxi(pLista lista) {
    int max_id = 0; 
    pLista atual = lista;
    while (atual != NULL) {
        if (atual->doente.id > max_id) {
            max_id = atual->doente.id; // esta retorna o id máximo na lista
        }
        atual = atual->prox;
    }
    return max_id;
}

// Função para validar o email

int validarEmail( char *email) {
    const char *tem = strchr(email, '@');
    if (!tem) {
        return 0; // Não contém '@'
    }

    // Verificar se há números após o '@'
    for (const char *p = tem + 1; *p; ++p) {
        if (isdigit((unsigned char)*p)) {
            return 0; // Contém número após '@'
        }
    }

    return 1; // Email válido
}

void menu_add_doente(pLista lista) {
    pLista aux = calloc(sizeof(noLista), 1);  // Armazena temporariamente as informações do doente
    
    int nome_valido = 0; // Assume que o nome não é válido

    while (!nome_valido) {
        char nome[100];
        printf("Nome: ");
        scanf("%99s", nome);

        int i;
        int tem_numero = 0;
        for (i = 0; nome[i] != '\0'; i++) {
            if (!isalpha(nome[i]) && nome[i] != ' ') {
                tem_numero = 1;
                break;
            }
        }

        if (tem_numero) {
            printf("Erro: O nome não pode conter números.\n");
        } else {
            nome_valido = 1;
            strcpy(aux->doente.nome, nome);
        }
    }

     // Limpar o buffer de entrada 
    while (getchar() != '\n');

    // Entrada do número de telefone
    int count2 = 0;
    while (1) {
        printf("Digite o número de telefone: ");
        if (scanf("%d", &(aux->doente.telefone)) != 1) {    // Verificar se a entrada contém apenas números
            printf("Erro: o valor digitado não é um número inteiro.\n");
            while (getchar() != '\n');  // Limpar o buffer de entrada
        } else {
            int temp = aux->doente.telefone;
            while (temp != 0) {
                temp /= 10;
                count2++; // Conta quantos dígitos tem o telefone
            }

            if (count2 != 9) {  // Verificar se o telefone tem 9 dígitos
                printf("Número incorreto, insira novamente.\n");
                count2 = 0;
            } else {
                break;
            }
        }
    }

    // Limpar o buffer de entrada após a leitura do número de telefone
    while (getchar() != '\n');

    // Entrada do email
    printf("Digite o email: ");
    scanf("%s", aux->doente.email);
    while(!validarEmail(aux->doente.email)){
        printf("Formato do email incorreto\n");
        printf("Digite o email: ");
        scanf("%s", aux->doente.email);
    }

    // Entrada da data de nascimento
    int data_valida = 0;
    int j=1;
    while(data_valida == 0){
        printf("Digite a data de nascimento (dia/mês/ano): ");
        if (scanf("%d/%d/%d", &aux->doente.nascimento.dia, &aux->doente.nascimento.mes, &aux->doente.nascimento.ano) != 3) {
            printf("Erro: formato de data inválido.\n");
            j=0;
        }else{j=1;}
    
        if (aux->doente.nascimento.dia < 1 || aux->doente.nascimento.dia > 31 ||
            aux->doente.nascimento.mes < 1 || aux->doente.nascimento.mes > 12 ||
            aux->doente.nascimento.ano < 1900 || aux->doente.nascimento.ano > 2024) {
            printf("Erro: data de nascimento inválida.\n");
            j=0;
        }else if((aux->doente.nascimento.mes == 2 && aux->doente.nascimento.dia>29) || aux->doente.nascimento.dia>30 && (aux->doente.nascimento.mes == 4 || aux->doente.nascimento.dia == 6 || aux->doente.nascimento.dia ==9 || aux->doente.nascimento.dia == 11)){
                printf("Erro: data inválida.\n");
                j=0;
        }else{j=1;}
        if(j){
        data_valida = 1;
        }
    }

    aux->doente.lista_dados = NULL;

    aux->doente.id = id_maxi(lista) + 1;

    // Limpar o buffer de entrada 
    while (getchar() != '\n');

    // Entrada do número do cartão de cidadão
    int count = 0;
    while (1) {
        printf("Digite o número do cartão de cidadão: ");
        if (scanf("%d", &(aux->doente.cc)) != 1) {    // Verificar se a entrada contém apenas números
            printf("Erro: o valor digitado não é um número inteiro.\n");
            while (getchar() != '\n');  // Limpar o buffer de entrada
        } else {
            int temp = aux->doente.cc;
            while (temp != 0) {
                temp /= 10;
                count++; // Conta quantos dígitos tem o cc
            }

            if (count != 8) {  // Verificar se o CC tem 9 dígitos
                printf("Número incorreto, insira novamente.\n");
                count = 0;
            } else {
                if (existe_cc(lista, aux->doente.cc)) {    // Verificar se o CC já existe na lista de doentes
                    printf("Número de cartão de cidadão já existe na lista de doentes.\n");
                    break;
                } else {
                    insere_doente(lista, aux->doente);    // Se o CC não existir, insere o doente
                    break;  // CC válido e não existe na lista, sair do loop
                }
            }
        }
    }
   getchar();
}



void remover_doente(pLista lista) {
    int cc;
    int count = 0;
    
    printf("Insira os dados do Doente a ser removido:\n");

    
    while (1) {
        printf("Digite o número do cartão de cidadão do doente a ser removido: ");
        if (scanf("%d", &cc) != 1) {    // Verificar se a entrada contém apenas números
            printf("Erro: o valor digitado não é um número inteiro.\n");
            while (getchar() != '\n');  // Limpar o buffer de entrada
        } else {
            int temp = cc;
            while (temp != 0) {
                temp /= 10;
                count++; // Conta quantos dígitos tem o cc
            }

            if (count != 8) {  // Verificar se o CC tem 8 dígitos
                printf("Número incorreto, insira novamente.\n");
                count = 0;
            } else {
                    break;
                }
            }
        }
    getchar();
    
    pLista ant = NULL;
    pLista atual = lista;

    while (atual != NULL) {
        if ((atual->doente.cc == cc)) {
            
            // Encontrou o doente, remove da lista
            if (ant == NULL) {
                // O nó a ser removido é o primeiro da lista
                lista = atual->prox;
            } else {
                // O nó a ser removido está no meio ou no final da lista
                ant->prox = atual->prox;
            }
            
            free(atual);
            printf("Removido com sucesso!\n");
            return;
        }
        
        ant = atual;
        atual = atual->prox;
    }
    
    printf("Nao foi possivel encontrar o Doente.\n");
}


void imprime_listaDoentes (pLista lista){

    pLista aux = lista->prox; /* Salta o header */
    while (aux) {   //enquanto o ponteiro não for NULL, dá print aos dados do nó
        Data data = aux->doente.nascimento; 
        printf(" %d\n %s\n %d/%d/%d\n %d\n %d \n %s \n\n", aux->doente.id, aux->doente.nome , data.dia,data.mes,data.ano,aux->doente.cc,aux->doente.telefone, aux->doente.email);
        aux = aux->prox;
    }

}

pLista procurar_pessoa_por_cc(pLista lista, int cc) {
    pLista atual = lista;
    while (atual != NULL) {
        if (atual->doente.cc == cc) {
            return atual;
        }
        atual = atual->prox;
    }
    return NULL; // Paciente não encontrado
}

void registar_dados_doente(pLista lista) {
    int cc;
    int count = 0;
    while (1) {
        printf("Digite o número do cartão de cidadão: ");
        if (scanf("%d", &cc) != 1) {    // Verificar se a entrada contém apenas números
            printf("Erro: o valor digitado não é um número inteiro.\n");
            while (getchar() != '\n');  // Limpar o buffer de entrada
        } else {
            int temp = cc;
            while (temp != 0) {
                temp /= 10;
                count++; // Conta quantos dígitos tem o cc
            }

            if (count != 8) {  // Verificar se o CC tem 8 dígitos
                printf("Número incorreto, insira novamente.\n");
                count = 0;
            } else {
                    break;
                }
            }
        }

    // Procurar o paciente na lista pelo cc
    pLista paciente = procurar_pessoa_por_cc(lista, cc);
    
    if (paciente != NULL ) {
        // Paciente encontrado, solicitar os dados
        RegistoDados dados;   

        int data_valida = 0;
        int j=1;
        while(data_valida == 0){
            printf("Digite a data (dia/mês/ano): ");
            if (scanf("%d/%d/%d", &dados.data.dia,&dados.data.mes, &dados.data.ano) != 3) {
                printf("Erro: formato de data inválido.\n");
                j=0;
            }else{j=1;}
        
            if (dados.data.dia < 1 || dados.data.dia> 31 ||
                dados.data.mes < 1 || dados.data.mes > 12 ||
                dados.data.ano < 1900 || dados.data.ano > 2024) {
                printf("Erro: data inválida.\n");
                j=0;
            }else if((dados.data.mes == 2 && dados.data.dia>29) || dados.data.dia>30 && (dados.data.mes == 4 || dados.data.dia == 6 || dados.data.dia ==9 || dados.data.dia == 11)){
                printf("Erro: data inválida.\n");
                j=0;
            }else{j=1;}
            
            if(j){
            data_valida = 1;
            }
        }

        while (getchar() != '\n');
            
         while (1) {
        printf("Digite a tensão mínima: ");
        if (scanf("%f", &dados.ta_min) != 1) {    // Verificar se a entrada contém apenas números
            printf("Erro: o valor digitado não é válido.\n");
            while (getchar() != '\n');  // Limpar o buffer de entrada
        } else if (dados.ta_min < 0 || dados.ta_min > 240) {  // Verificar se está entre 0 e 240
            printf("Erro: o valor deve estar entre 0 e 240.\n");
        } else {
            break;
        }
        }


        //tensão máxima
        while (1) {
        printf("Digite a tensão máxima: ");
        if (scanf("%f", &dados.ta_max) != 1) {    // Verificar se a entrada contém apenas números
            printf("Erro: o valor digitado não é inválido.\n");
            while (getchar() != '\n');  // Limpar o buffer de entrada
        } else if (dados.ta_max < 0 || dados.ta_max > 300) {  // Verificar se está entre 0 e 300
            printf("Erro: o valor deve estar entre 0 e 300.\n");
        } else {
            break;
        } 
        }

        //peso
        while (1) {
        printf("Digite o peso(Kg): ");
        if (scanf("%f", &dados.peso) != 1) {    // Verificar se a entrada contém apenas números
            printf("Erro: o valor digitado não é um float.\n");
            while (getchar() != '\n');  // Limpar o buffer de entrada
        } else if (dados.peso < 0 || dados.peso > 300) {  // Verificar se está entre 0 e 300
            printf("Erro: o valor deve estar entre 4 e 300.\n");
        } else {
            break;
        }
        }
        
        //altura
        while (1) {
        printf("Digite a altura(cm): ");
        if (scanf("%d", &dados.altura) != 1) {    // Verificar se a entrada contém apenas números
            printf("Erro: o valor digitado é inválido.\n");
            while (getchar() != '\n');  // Limpar o buffer de entrada
        } else if (dados.altura < 25 || dados.altura > 260) {  // Verificar se está entre 0 e 260
            printf("Erro: o valor deve estar entre 25 e 250.\n");
        } else {
            break;
        }
        }
        
        // Criar um novo nó para armazenar os dados
        NoRegisto *novo_registo = (NoRegisto *)malloc(sizeof(NoRegisto));
        if (novo_registo == NULL) {
            printf("Erro ao alocar memória para os dados do paciente.\n");
            return;
        }
        novo_registo->registo = dados;
        novo_registo->prox = NULL;

        // Adicionar o novo registo à lista de dados do paciente
        if (paciente->doente.lista_dados == NULL) {
            paciente->doente.lista_dados = (ListaDados *)malloc(sizeof(ListaDados));
            if (paciente->doente.lista_dados == NULL) {
                printf("Erro ao alocar memória para a lista de dados do paciente.\n");
                free(novo_registo);
                return;
            }
            paciente->doente.lista_dados->inicio = novo_registo;
            paciente->doente.lista_dados->fim = novo_registo;
        } else {
            NoRegisto *atual = paciente->doente.lista_dados->inicio;
            NoRegisto *anterior = NULL;

            while (atual != NULL && atual->registo.ta_max > dados.ta_max) {
                anterior = atual;
                atual = atual->prox;
            }

            if (anterior == NULL) {
                novo_registo->prox = paciente->doente.lista_dados->inicio;
                paciente->doente.lista_dados->inicio = novo_registo;
            } else {
                anterior->prox = novo_registo;
                novo_registo->prox = atual;
            }

            if (atual == NULL) {
                paciente->doente.lista_dados->fim = novo_registo;
            }
        }
        
        printf("Dados registados com sucesso para o paciente %s.\n", paciente->doente.nome);
    } else {
        printf("Paciente não encontrado.\n");
    }
}



void imprimir_dados_doente(pLista lista) {
    pLista paciente = NULL;

    int cc;
    int count = 0;
    while (1) {
        printf("Digite o número do cartão de cidadão: ");
        if (scanf("%d", &cc) != 1) {    // Verificar se a entrada contém apenas números
            printf("Erro: o valor digitado não é um número inteiro.\n");
            while (getchar() != '\n');  // Limpar o buffer de entrada
        } else {
            int temp = cc;
            while (temp != 0) {
                temp /= 10;
                count++; // Conta quantos dígitos tem o cc
            }

            if (count != 8) {  // Verificar se o CC tem 8 dígitos
                printf("Número incorreto, insira novamente.\n");
                count = 0;
            } else {
                    break;
                }
            }
        }

    paciente = procurar_pessoa_por_cc(lista, cc);
    

    if (paciente != NULL) {
        printf("Dados do paciente %s:\n", paciente->doente.nome);

        Data data = paciente->doente.nascimento; 
        printf("ID: %d\nNome: %s\nCartão de cidadão: %d\nData de Nascimento: %d/%d/%d\n\n", paciente->doente.id, paciente->doente.nome, paciente->doente.cc, data.dia, data.mes, data.ano);

        ListaDados *lista_dados = paciente->doente.lista_dados;

        if (lista_dados != NULL && lista_dados->inicio != NULL) {
            // Percorrer a lista de dados e imprimir cada registro
            NoRegisto *atual = lista_dados->inicio;
            while (atual != NULL) {
                printf("Data: %02d/%02d/%d\n", atual->registo.data.dia, atual->registo.data.mes, atual->registo.data.ano);
                printf("Tensão Mínima: %.2f\n", atual->registo.ta_min);
                printf("Tensão Máxima: %.2f\n", atual->registo.ta_max);
                printf("Peso: %.2f\n", atual->registo.peso);
                printf("Altura: %d\n", atual->registo.altura);
                printf("\n");

                atual = atual->prox;
            }
        } else {
            printf("O paciente não possui registos de dados.\n");
        }
    } else {
        printf("Paciente não encontrado.\n");
    }
}
