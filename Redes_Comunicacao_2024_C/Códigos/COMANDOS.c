#include <sys/socket.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include <netdb.h>
#include <string.h>
#include <sys/wait.h>
#include <arpa/inet.h>
#include <signal.h>
#include <stdbool.h>
#include <ctype.h>
#include <string.h>
#include "COMANDOS.h"

#define BUFLEN 512	// Tamanho do buffer

////////////////////////////////////////////
//FUNÇÔES USADAS INTERNAMENTE
////////////////////////////////////////////
int verificaNumero(char* str) {
    while (*str) {
        if (!isdigit(*str)) return 0;
        str++;
    }
    return 1;
}


void escreveUserFicheiro( char *nomefich, char* nome, char* pass, char* tipo) {

    FILE *fich = fopen(nomefich, "a");

    if (fich != NULL) {
        fprintf(fich, "%s;%s;%s\n", nome, pass, tipo);
        fclose(fich);
    } else {
        printf("Erro ao abrir o arquivo %s\n",nomefich);
    }
}

bool verificaMulticast(char* multicast) {
    FILE *file = fopen("classes.txt", "r");
    if (file == NULL) {
        printf("Erro ao abrir o arquivo classes.txt\n");
        exit(1);
    }
    char linha[100];
    while (fgets(linha, sizeof(linha), file) != NULL) {
        char *atual = strtok(linha, ";");
        if (strcmp(multicast, atual) == 0) {
            fclose(file);
            return true;
        }
    }
    fclose(file);
    return false;
}

char *criaMulticast() {
    char *multicast = (char *)malloc(19 * sizeof(char)); 
    if (multicast == NULL) {
        printf("Erro ao alocar memória para o endereço multicast.\n");
        exit(1);
    }

    bool existe = true;
    while (existe) {
        sprintf(multicast, "239.%d.%d.%d", rand() % 256, rand() % 256, rand() % 256);
        existe = verificaMulticast(multicast);
    }

    return multicast;
}


bool verificaClasses(char *nome) {
    FILE *file = fopen("classes.txt", "r");

    if (file == NULL) {
        printf("Erro ao abrir o arquivo classes.txt\n");
        exit(1);
    }

    char line[100];

    while (fgets(line, sizeof(line), file) != NULL) {
        char *atual = strtok(line, ";"); 
        atual = strtok(NULL, ";"); 
        if (atual != NULL && strcmp(atual, nome) == 0) {
            fclose(file);
            return true;
        }
    }

    fclose(file);
    return false;
}

bool escreveClassFicheiro(char* nome, int tamanho) {
    FILE *file = fopen("classes.txt", "a");
    char * multicast = criaMulticast();

    bool classeExiste = verificaClasses(nome);

    if(!classeExiste){
        if (file != NULL) {
  
            fprintf(file, "%s;%s;%d", multicast, nome, tamanho);
            fprintf(file, "\n");
            fclose(file);
            return false;
        } else {
            printf("Erro ao abrir o arquivo classes.txt\n");
            return true;
        }
    }else{
        fclose(file);
        return true;
    }
}


bool verificaNome(char *nome, char *nomefich) {
    FILE *fich = fopen(nomefich, "r");
    if (fich == NULL) {
        printf("Erro ao abrir o arquivo %s\n", nomefich);
        return false;
    }

    char line[100];
    while (fgets(line, sizeof(line), fich)) {
        char *multicast = strtok(line, ";");
        char *nomefi = strtok(NULL, ";");
        if (nomefi != NULL && strcmp(nomefi, nome) == 0) {
            fclose(fich);
            return true;
        }
    }

    fclose(fich);
    return false;
}

char *mostraClasses() {
    FILE *file = fopen("classes.txt", "r");

    if (file == NULL) {
        printf("Erro ao abrir o arquivo classes.txt\n");
        return NULL; 
    }

    char line[100];
    char* resposta = (char*)malloc(BUFLEN * sizeof(char)); 
    if (resposta == NULL) {
        printf("Erro ao alocar memória para a resposta\n");
        fclose(file);
        return NULL; 
    }
    resposta[0] = '\0'; 

    char aux[100]; 

    sprintf(aux, "CLASS "); 
    strcat(resposta, aux);

    while (fgets(line, sizeof(line), file)) {
        char* multicast = strtok(line, ";");
        char *className = strtok(NULL, ";");
        char *tamanh = strtok(NULL, ";");

        if (className != NULL && tamanh != NULL) {

            sprintf(aux, "\n      nome:%s Tamanho:%s", className, tamanh);
            strcat(resposta, aux); 
        }
    }

    fclose(file);
    return resposta; 
}

char *mostraUtilizadores() {
    FILE *file = fopen("logins.txt", "r");

    if (file == NULL) {
        printf("Erro ao abrir o arquivo logins.txt\n");
        return NULL; 
    }

    char line[100];

    char* resposta = (char*)malloc(BUFLEN * sizeof(char)); 
    if (resposta == NULL) {
        printf("Erro ao alocar memória para a resposta\n");
        fclose(file);
        return NULL; 
    }
    resposta[0] = '\0'; 

    char aux[BUFLEN]; 

    sprintf(aux, "UTILIZADORES: \n"); 
    strcat(resposta, aux);

    while (fgets(line, sizeof(line), file)) {
        char *nome = strtok(line, ";");
        char *pass = strtok(NULL, ";");

        if (nome != NULL && pass != NULL) {

            sprintf(aux, "      nome:%s  palavra-passe:%s\n", nome, pass);
            strcat(resposta, aux); 
        }
    }

    fclose(file);
    return resposta; 
}



void removeUtilizador(char *nome, char *senha) {
    FILE *velho = fopen("logins.txt", "r");
    if (velho == NULL) {
        printf("Erro ao abrir o arquivo logins.txt.\n");
        exit(1);
    }

    FILE *novo = fopen("temp.txt", "w");
    if (novo == NULL) {
        printf("Erro ao criar arquivo temporário.\n");
        exit(1);
    }

    char linha[150];
    char *passewo;
    while (fgets(linha, sizeof(linha), velho) != NULL) {
        if (linha[strlen(linha) - 1] == '\n') {
            linha[strlen(linha) - 1] = '\0';
        }

        char *atual = strtok(linha, ";");
        char *nome_atual = atual;
        if (strcmp(atual, nome) == 0) {
            atual = strtok(NULL, ";");
            passewo = atual;
            if (strcmp(atual, senha) != 0) {
                atual = strtok(NULL, ";");
                fprintf(novo, "%s;%s;%s\n", nome_atual, passewo, atual);
            }
        } else {
            atual = strtok(NULL, ";");
            passewo = atual;
            atual = strtok(NULL, ";");
            fprintf(novo, "%s;%s;%s\n", nome_atual, passewo, atual);
        }
    }
    fclose(velho);
    fclose(novo);

    remove("logins.txt");
    rename("temp.txt", "logins.txt");
}

char *subscreverTurma(char *nomeTurma, char *nomeAluno, int porta) {
    FILE *originalFile = fopen("classes.txt", "r");
    if (originalFile == NULL) {
        printf("Erro ao abrir o arquivo classes.txt\n");
        exit(1);
    }

    FILE *tempFile = fopen("tempfile.txt", "w");
    if (tempFile == NULL) {
        printf("Erro ao criar o arquivo temporário\n");
        fclose(originalFile);
        exit(1);
    }

    char linha[100];
    char *msg = malloc(200);
    if (msg == NULL) {
        printf("Erro de alocação de memória\n");
        fclose(originalFile);
        fclose(tempFile);
        exit(1);
    }

    int found = 0;
    int alunoJaInscrito = 0;

    while (fgets(linha, sizeof(linha), originalFile) != NULL) {
        linha[strcspn(linha, "\n")] = '\0';

        char aux[100];
        strcpy(aux, linha);
        char *multicast = strtok(aux, ";");
        char *nome = strtok(NULL, ";");
        char *tamanhoStr = strtok(NULL, ";");
        int tamanho = atoi(tamanhoStr);

        if (strcmp(nome, nomeTurma) == 0) {
            // Verificar se ainda há vagas disponíveis na turma
            int alunosInscritos = 0;
            char *aluno = strtok(NULL, ";"); // Primeiro aluno/professor
            while (aluno != NULL) {
                if (aluno[0] != ':') { // Verifica se é aluno (não professor)
                    alunosInscritos++;
                    // Verifica se o aluno já está inscrito na turma
                    if (strcmp(aluno, nomeAluno) == 0) {
                        alunoJaInscrito = 1;
                        break;
                    }
                }
                aluno = strtok(NULL, ";");
            }

            if (alunosInscritos < tamanho && !alunoJaInscrito) {
                // Turma não está cheia e aluno não está inscrito
                // Adicionar o nome do aluno à lista de inscritos
                strcat(linha,";");
                strcat(linha, nomeAluno);
                strcat(linha, "\n");
                fputs(linha, tempFile);
                sprintf(msg, "Endereço Multicast:%s\n", multicast);
            } else if (alunoJaInscrito) {
                sprintf(msg, "O aluno %s já está inscrito na turma %s.\n", nomeAluno, nomeTurma);
            } else {
                // Turma está cheia
                sprintf(msg, "A turma %s já está cheia.\n", nomeTurma);
            }
            found = 1; 
        } else {
            strcat(linha, "\n");
            fputs(linha, tempFile);
        }
    }

    fclose(originalFile);
    fclose(tempFile);

    if (remove("classes.txt") != 0) {
        printf("Erro ao remover o arquivo original\n");
        exit(1);
    }
    if (rename("tempfile.txt", "classes.txt") != 0) {
        printf("Erro ao renomear o arquivo temporário\n");
        exit(1);
    }

    if (!found) {
        sprintf(msg, "Turma %s não encontrada.\n", nomeTurma);
    }

    return msg;
}



int professor_existe(char *nome_professor, char* nome_classe) {
    FILE *fich = fopen("classes.txt", "r");
    if (fich == NULL) {
        perror("Erro ao abrir o arquivo");
        return 0; // Arquivo não encontrado ou erro ao abrir
    }

    char line[256];
    int encontrou = 0;

    while (fgets(line, sizeof(line), fich)) {
        // Remover o caractere de nova linha, se presente
        line[strcspn(line, "\n")] = '\0';
        
    
        char *multicast = strtok(line, ";");
        char *classe = strtok(NULL, ";");
        if (classe != NULL && strcmp(classe, nome_classe) == 0) {
            char *atual = strtok(NULL, ";");
            while (atual != NULL) {
                // Verificar se começa com ':' seguido pelo nome do professor
                if (atual[0] == ':' && strcmp(atual + 1, nome_professor) == 0) {
                    encontrou = 1;
                    break;
                }
                atual = strtok(NULL, ";");
            }
        }
        if (encontrou) {
            break;
        }
    }

    fclose(fich);
    return encontrou;
}


char *subscreverTurmaProfessor(char *nomeTurma, char *nomeProfessor) {
    FILE *originalFile = fopen("classes.txt", "r");
    if (originalFile == NULL) {
        printf("Erro ao abrir o arquivo classes.txt\n");
        exit(1);
    }

    FILE *tempFile = fopen("tempfile.txt", "w");
    if (tempFile == NULL) {
        printf("Erro ao criar o arquivo temporário\n");
        fclose(originalFile);
        exit(1);
    }

    char linha[100];
    char *msg = malloc(200);
    if (msg == NULL) {
        printf("Erro de alocação de memória\n");
        fclose(originalFile);
        fclose(tempFile);
        exit(1);
    }

    int found = 0;
    while (fgets(linha, sizeof(linha), originalFile) != NULL) {
        linha[strcspn(linha, "\n")] = '\0';
        
        char aux[100];
        strcpy(aux, linha);
        char *multicast = strtok(aux, ";");
        char *nome = strtok(NULL, ";");
        char *tamanhoStr = strtok(NULL, ";");
        int tamanho = atoi(tamanhoStr);

        if (strcmp(nome, nomeTurma) != 0) {
            strcat(linha, "\n");
            fputs(linha, tempFile);
        } else {
            found = 1;
            if(professor_existe(nomeProfessor,nomeTurma)){
                sprintf(msg, "O professor %s já está inscrito na turma %s\n", nomeProfessor, nomeTurma);
                strcat(linha,"\n"); 
                fputs(linha, tempFile);
            }else{
                strcat(linha, ";:");
                strcat(linha,nomeProfessor);
                fputs(linha, tempFile);
                fprintf(tempFile, "\n");
            }
        
        }

    }

    fclose(originalFile);
    fclose(tempFile);
    if (remove("classes.txt") != 0) {
        printf("Erro ao remover o arquivo original\n");
        exit(1);
    }
    if (rename("tempfile.txt", "classes.txt") != 0) {
        printf("Erro ao renomear o arquivo temporário\n");
        exit(1);
    }

    if (!found) {
        sprintf(msg, "Turma %s não encontrada.\n", nomeTurma);
    }

    return msg;
}

// Função para remover espaços extras do início e do fim de uma string
void strtrim(char *str) {
    int len = strlen(str);
    int start = 0, end = len - 1;
    while (isspace(str[start]) && start < len) start++;
    while (isspace(str[end]) && end >= 0) end--;
    memmove(str, str + start, end - start + 1);
    str[end - start + 1] = '\0';
}

char * imprimirClassesAluno(char *nomeAluno) {
    FILE *file = fopen("classes.txt", "r");
    if (file == NULL) {
        fprintf(stderr, "Erro ao abrir o arquivo classes.txt\n");
        exit(1);
    }

    char linha[200];
    int alunoEncontrado = 0;
    char aux[BUFLEN]; 

    char* resposta = (char*)malloc(BUFLEN * sizeof(char)); 
    if (resposta == NULL) {
        printf("Erro ao alocar memória para a resposta\n");
        fclose(file);
        return NULL; 
    }
    resposta[0] = '\0'; 
   
    sprintf(aux,"As turmas em que o professor %s está inscrito são:\n", nomeAluno);
    strcat(resposta, aux);

    while (fgets(linha, sizeof(linha), file) != NULL) {
        linha[strcspn(linha, "\n")] = 0; // Remove \n

        char *atual = strtok(linha, ";");
        char *multicast = atual;
        char *nomeTurma = strtok(NULL, ";");
        char *tamanhoStr = strtok(NULL, ";");

        // Verifica se o nome do aluno está na lista de alunos
        while ((atual = strtok(NULL, ";")) != NULL) {
            strtrim(atual);
            strtrim(nomeAluno);
            if (strcasecmp(atual, nomeAluno) == 0) {
                sprintf(aux,"- %s (Endereço Multicast: %s)\n", nomeTurma, multicast);
                strcat(resposta, aux); 
                alunoEncontrado = 1;
            }
        }
    }

    if (!alunoEncontrado) {
        sprintf(resposta,"O aluno %s não está inscrito em nehuma turma\n", nomeAluno);
    }
    fclose(file);
    return resposta;
}


char *imprimirClassesProfessor(char *nomeProfessor) {
    FILE *file = fopen("classes.txt", "r");
    if (file == NULL) {
        fprintf(stderr, "Erro ao abrir o arquivo classes.txt\n");
        exit(1);
    }

    char linha[200];
    int alunoEncontrado = 0;
    char aux[BUFLEN]; 

    char* resposta = (char*)malloc(BUFLEN * sizeof(char)); 
    if (resposta == NULL) {
        printf("Erro ao alocar memória para a resposta\n");
        fclose(file);
        return NULL; 
    }
    resposta[0] = '\0'; 
   
    sprintf(aux, "As turmas em que o professor %s está inscrito são:\n", nomeProfessor);
    strcat(resposta, aux);

    while (fgets(linha, sizeof(linha), file) != NULL) {
        linha[strcspn(linha, "\n")] = 0;

        char *atual = strtok(linha, ";");
        char *multicast = atual;
        char *nomeTurma = strtok(NULL, ";");
        char *tamanhoStr = strtok(NULL, ";");

        // Verifica se o nome do professor está na lista de inscritos
        while ((atual = strtok(NULL, ";")) != NULL) {
            if (atual[0] == ':' && strcasecmp(nomeProfessor, atual + 1) == 0) {
                sprintf(aux, "- %s (Endereço Multicast: %s)\n", nomeTurma, multicast);
                strcat(resposta, aux); 
                alunoEncontrado = 1;
            }
        }
    }

    if (!alunoEncontrado) {
        printf("O professor %s não está inscrito em nenhuma turma.\n", nomeProfessor);
    }

    fclose(file);
    return resposta;
}


char *enviarMulticastUDP(int porta, char *nome, char *mensagem, int tamanho) {
    
    char *aux = malloc(100);
    if (aux == NULL) {
        printf("Erro de alocação de memória\n");
        exit(1);
    }

    FILE *fich = fopen("classes.txt", "r");
    if (fich == NULL) {
        printf("Erro ao abrir o arquivo\n");
        return NULL;
    }

    char multicast_atual[16];
    char line[100];
    int encontrou = 0;

    while (fgets(line, sizeof(line), fich)) {
        char *multicast = strtok(line, ";");
        char *nomefi = strtok(NULL, ";");
        if (nomefi != NULL && strcmp(nomefi, nome) == 0) {
            strncpy(multicast_atual, multicast, sizeof(multicast_atual));
            encontrou = 1;
            break;
        }
    }

    fclose(fich);

    if (!encontrou) {
        printf("Nome não encontrado no arquivo\n");
        sprintf(aux, "Nome não encontrado no arquivo");
        return aux;
    }

    // Inicializar o socket UDP
    int sock;
    struct sockaddr_in addr;
    if ((sock = socket(AF_INET, SOCK_DGRAM, 0)) < 0) {
        perror("socket");
        exit(1);
    }
    
    // Configurar a estrutura de endereço multicast
    memset(&addr, 0, sizeof(addr));
    addr.sin_family = AF_INET;
    addr.sin_addr.s_addr = inet_addr(multicast_atual);
    addr.sin_port = htons(5000);
    
    // Habilitar multicast no socket
    int enable = 3;  //GNS3 pelo menos 3
    if (setsockopt(sock, IPPROTO_IP, IP_MULTICAST_TTL, &enable, sizeof(enable)) < 0) {
        perror("setsockopt");
        exit(1);
    }
    
    // Enviar a mensagem multicast
    if (sendto(sock, mensagem, strlen(mensagem), 0, (struct sockaddr *)&addr, sizeof(addr)) < 0) {
        perror("sendto");
        exit(1);
    }
    sprintf(aux, "Mensagem multicast enviada com sucesso");
    fflush(stdin);

    close(sock); 
    return aux;
}


////////////////////////////////////////////
//FUNÇÔES USADAS NO SERVER
////////////////////////////////////////////

char *comandosTCP_Aluno(char *buf, char* nome_aluno,int porta) {

    char *comand = strtok(buf, " ");
    
    char *aux2 = (char *)malloc(BUFLEN * sizeof(char)); 
    
    if (aux2 == NULL) {
        // Lidar com erro de alocação de memória
        exit(1);
    }

    if (strcmp(comand, "LIST_CLASSES") == 0){
        char * message = mostraClasses();
        return message;
    }else if(strcmp(comand, "SUBSCRIBE_CLASS") == 0){
       comand = strtok(NULL, " ");
        if (comand != NULL){
            char* nomeTurma = comand;

            if (nomeTurma != NULL){  
                char* res = subscreverTurma(nomeTurma,nome_aluno,porta);
                strcpy(aux2,res);
                return aux2;
        
            } else {
                sprintf(aux2, "Wrong usertype");
                return aux2;
            }

        }
    }else if (strcmp(comand, "LIST_SUBSCRIBED") == 0){
        char * msg = imprimirClassesAluno(nome_aluno);
        strcpy(aux2,msg);
        return aux2;

    }else {
            sprintf(aux2,"Incompatible comand\n");
            return aux2;
        }

}

char *comandosTCP_Professor(char *buf,char* nome_professor,int porta) {
    char *comand = strtok(buf, " ");
    char *aux2 = (char *)malloc(BUFLEN * sizeof(char)); // Alocar dinamicamente memória para aux2
     
    if (aux2 == NULL) {
        // Lidar com erro de alocação de memória
        exit(1);
    }

    if (strcmp(comand, "CREATE_CLASS") == 0){
        comand = strtok(NULL, " ");
        if (comand != NULL){
            char* nomeTurma = comand;
            comand = strtok(NULL, " ");
            if (comand != NULL){
                if(!verificaNumero(comand)){
                    sprintf(aux2, "Wrong usertype");
                    return aux2;
                }

                int tamanhoTurma = atoi(comand);
                
                bool jaExiste = escreveClassFicheiro(nomeTurma, tamanhoTurma);
                if(!jaExiste){
                sprintf(aux2, "The command %s, was received correctly, adding the class with the name %s with the size of ...%d...\n", buf, nomeTurma, tamanhoTurma);
                return aux2;
                }else{
                    sprintf(aux2, "Turma já existe!");
                    return aux2;
                }
            } else {
                sprintf(aux2, "Wrong usertype");
                return aux2;
            }
        }
    }else if (strcmp(comand, "LIST_CLASSES") == 0){
        char * message = mostraClasses();
        return message;
    }else if (strcmp(comand, "LIST_SUBSCRIBED") == 0){
        char * msg = imprimirClassesProfessor(nome_professor);
        strcpy(aux2,msg);
        return aux2;
    }else if(strcmp(comand, "SUBSCRIBE_CLASS") == 0){
       comand = strtok(NULL, " ");
        if (comand != NULL){
            char* nomeTurma = comand;

            if (nomeTurma != NULL){  
                char* res = subscreverTurmaProfessor(nomeTurma,nome_professor);
                strcpy(aux2,res);
                return aux2;
        
            } else {
                sprintf(aux2, "Wrong usertype");
                return aux2;
            }

        }
    }else if (strcmp(comand, "SEND") == 0){
            comand = strtok(NULL, " ");
            if (comand!=NULL){
                char *name = comand;
                comand = strtok(NULL, " ");
                if (comand!=NULL){
                    char *message = comand;
                    if(verificaNome(name, "classes.txt")) {
                        if(professor_existe(nome_professor,name)){
                        sprintf(aux2, "O comando %s foi recebido corretamente. Enviando mensagem para a turma %s: %s\n", buf, name, message);

                        strcat(aux2,enviarMulticastUDP(porta,name, message,strlen(message)));
                        return aux2;
                        }else{
                            sprintf(aux2, "O professor não tem permissões nesta turma");
                            return aux2;
                        }
                    } else {
                        sprintf(aux2, "Turma não encontrada.\n");
                        return aux2;
                    }
                }
            }
        }else if (strcmp(comand, "LIST_CLASSES") == 0){
            char * message = mostraClasses();
            return message;
        }else {
            sprintf(aux2,"Incompatible comand\n");
            return aux2;
        }
}

char *comandosUDP(char *buf) {
    char *comand = strtok(buf, " ");
    char *aux2 = (char *)malloc(BUFLEN * sizeof(char)); // Alocar dinamicamente memória para aux2
     
    if (aux2 == NULL) {
        // Lidar com erro de alocação de memória
        exit(1);
    }

    if (strcmp(comand, "ADD_USER") == 0){
        comand = strtok(NULL, " ");
        if (comand != NULL){
            char* usernames = comand;
            comand = strtok(NULL, " ");
            if (comand != NULL){
                char* passwords = comand;
                comand = strtok(NULL, " ");
                if (comand != NULL){
                    char* usertype = comand;
                    if(strcmp(usertype,"administrador")==0 || strcmp(usertype,"aluno")==0 || strcmp(usertype,"professor")==0){
                        escreveUserFicheiro("logins.txt", usernames, passwords, usertype); 
                        sprintf(aux2, "The command %s, was received correctly, adding the user %s with the password %s and with the permissions of a %s\n", buf, usernames, passwords, usertype);
                        return aux2;
                    }else{
                        sprintf(aux2, "Wrong permissiontype\n");
                        return aux2;
                    }
                } else {
                    sprintf(aux2, "Less arguments than expected\n");
                    return aux2;
                }
            }else {
                sprintf(aux2, "Less arguments than expected\n");
                return aux2;
            }
        }else {
            sprintf(aux2, "Less arguments than expected\n");
            return aux2;
        }
    } else if (strcmp(comand, "DEL")  == 0){
            comand = strtok(NULL, " ");
            if (comand!=NULL){
                char *username = comand;
                comand = strtok(NULL, " ");
                if (comand!=NULL){
                    char *password = comand;
                    
                        removeUtilizador(username,password);
                        sprintf(aux2,"the command %s, was received correctly, delete the user %s with the password %s\n", buf, username,password);
                        return aux2;
                }
            }
            else{
                sprintf(aux2,"User doesn't exist");
                return aux2;
            }

        } else if (strcmp(comand, "LIST") == 0){
            aux2 = mostraUtilizadores();
            return aux2;

        } else if (strcmp(comand, "QUIT_SERVER") == 0){
            sprintf(aux2,"The command %s, was received correctly\n", buf);
            return aux2;

        }else {
            sprintf(aux2,"Incompatible comand\n");
            return aux2;
        }
        
        exit(0); 
        return NULL; 




}