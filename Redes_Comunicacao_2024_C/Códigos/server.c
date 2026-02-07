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
#include "COMANDOS.h"

#define BUF_SIZE 1024
int porta_udp;
int porta_tcp;
struct fileInfo {
    char user[BUF_SIZE];
    char password[BUF_SIZE];
    char tipo[BUF_SIZE];
};

int sock_tcp, sock_udp;
int tamFich;
struct fileInfo* ficheiro;
int conf_login = 0;

void termina(int sig) {
    close(sock_tcp);

    close(sock_udp);
    free(ficheiro);
    conf_login = 0;
    exit(0);
}   
struct fileInfo* lerFicheiro(char name[], int *tam) {
    FILE *fich = fopen(name, "r");
    char linha[2000];
    *tam = 0;
    struct fileInfo* lido = NULL;

    for (int i = 0; fgets(linha, BUF_SIZE, fich); i++) {
        if (linha[strlen(linha) - 1] == '\n') {
            linha[strlen(linha) - 1] = '\0';
        }
        lido = realloc(lido, ++(*tam) * sizeof(struct fileInfo));
        char* aux = strtok(linha, ";");
        strcpy(lido[i].user, aux);
        aux = strtok(NULL, ";");
        strcpy(lido[i].password, aux);
        aux = strtok(NULL, ";");
        strcpy(lido[i].tipo, aux);
    }
    fclose(fich);
    return lido;
}

void erro(char *msg) {
    printf("Erro: %s\n", msg);
    exit(-1);
}

bool check_login(char buffer[BUF_SIZE], char *auxiliar) {
    auxiliar = strtok(buffer, " ");

    if (strcmp(auxiliar, "LOGIN")) {
        return false;
    }
    auxiliar = strtok(NULL, " ");
    char ff[strlen(auxiliar)];
    strcpy(ff, auxiliar);
    auxiliar = strtok(NULL, " ");

    int newline_index = strcspn(auxiliar, "\n");
    if (newline_index < strlen(auxiliar)) {
        auxiliar[newline_index] = '\0';
    }

    for (int i = 0; i < tamFich; i++) {
        if (!strncmp(ficheiro[i].user, ff, strlen(ff))) {
            if (!strncmp(ficheiro[i].password, auxiliar, strlen(auxiliar)) && (strlen(auxiliar) == strlen(ficheiro[i].password))) {
                return true;
            }
        }
    }
    return false;
}


char* confirmaPermissoes(char buffer[BUF_SIZE], char *auxiliar) {
    fflush(stdout);
    // Faz uma cópia da string buffer
    char buffer_copy[BUF_SIZE];
    strcpy(buffer_copy, buffer);
    // Continua com o processamento da cópia
    auxiliar = strtok(buffer_copy, " ");
    char* permiss = malloc(BUF_SIZE * sizeof(char)); // Aloca memória para permiss

    if (permiss == NULL) {
        printf("Erro ao alocar memória para permiss.\n");
        return NULL;
    }
    
    auxiliar = strtok(NULL, " ");
    char aa[BUF_SIZE];

    strcpy(aa, auxiliar);

    auxiliar = strtok(NULL, " ");

    int newline_index = strcspn(auxiliar, "\n");
    if (newline_index < strlen(auxiliar)) {
        auxiliar[newline_index] = '\0';
    }
    
    for (int i = 0; i < tamFich; i++) {
        if (!strncmp(ficheiro[i].user, aa, strlen(aa))) {
            if (!strncmp(ficheiro[i].password, auxiliar, strlen(auxiliar)) && (strlen(auxiliar) == strlen(ficheiro[i].password))) {
                if (!strncmp(ficheiro[i].tipo, "administrador", strlen("administrador"))) {
                    strcpy(permiss, "administrador");

                } else if (!strncmp(ficheiro[i].tipo, "professor", strlen("professor"))) {
                    strcpy(permiss, "professor");

                } else if (!strncmp(ficheiro[i].tipo, "aluno", strlen("aluno"))) {
                    strcpy(permiss, "aluno");

                }
                return permiss; 
            }
        }
    }

    strcpy(permiss, "null");
    return permiss;
}


void controla_comandos(int client_fd, int is_tcp, struct sockaddr_in client_addr,char * permiss) {
    char ip[INET_ADDRSTRLEN];
    
    char buffer_copy[BUF_SIZE];
    char buffer_copy_second[BUF_SIZE];

    char * nome_funcoes;

    inet_ntop(client_addr.sin_family, &(client_addr.sin_addr), ip, INET_ADDRSTRLEN);
    printf("Connection established with %s:%d\n", ip, ntohs(client_addr.sin_port));

    ssize_t bytes_received;
    char buffer[BUF_SIZE];

    const char *welcome_msg = "Escreve o comando da operação que deseja realizar: ";
    if (is_tcp) {
        write(client_fd, welcome_msg, strlen(welcome_msg));
    } else {
        sendto(sock_udp, welcome_msg, strlen(welcome_msg), 0, (struct sockaddr*)&client_addr, sizeof(client_addr));
    }

    while (1) {
        if (is_tcp) {
            bytes_received = read(client_fd, buffer, BUF_SIZE);
        } else {
            socklen_t addr_len = sizeof(client_addr);
            bytes_received = recvfrom(sock_udp, buffer, BUF_SIZE, 0, (struct sockaddr*)&client_addr, &addr_len);
        }

        if (bytes_received < 0) {
            perror("Error receiving data");
            break;
        }

        buffer[bytes_received] = '\0';

        if (strncmp("QUIT_SERVER", buffer, strlen("QUIT_SERVER")) == 0) {
            const char *quit_msg = "Até Logo!";
            if (is_tcp) {
                write(client_fd, quit_msg, strlen(quit_msg));
            } else {
                sendto(sock_udp, quit_msg, strlen(quit_msg), 0, (struct sockaddr*)&client_addr, sizeof(client_addr));
            }
            break;
        }

        if (conf_login == 0) {
            strcpy(buffer_copy,buffer);
            strcpy(buffer_copy_second,buffer);
            nome_funcoes = strtok(buffer_copy_second, " ");
            nome_funcoes = strtok(NULL," ");
            bool verifica_login = check_login(buffer, NULL);
            if (!verifica_login) {
                const char *err_msg = "Login errado!\nTente novamente!\n";
                if (is_tcp) {
                    write(client_fd, err_msg, strlen(err_msg));
                } else {
                    sendto(sock_udp, err_msg, strlen(err_msg), 0, (struct sockaddr*)&client_addr, sizeof(client_addr));
                }
                continue;
            }
            
            conf_login = 1;
        }

        if (strncmp("LOGIN", buffer, strlen("LOGIN")) == 0) {
            
            permiss = confirmaPermissoes(buffer_copy,NULL);
            
            if(!(is_tcp) && strncmp(permiss, "administrador", strlen("administrador"))){ // se não for tcp e não for admistrador
                sendto(sock_udp, "Não tem as permissões necessárias para realizar login...\nFaça login com um utilizador com permissões de admistrador!\n", strlen("Não tem as permissões necessárias para realizar login...\n Faça login com um utilizador com permissões de admistrador!\n"), 0, (struct sockaddr*)&client_addr, sizeof(client_addr));
                conf_login = 0;
                continue;

            }
            const char *login_msg = "LOGIN ACEITE!\n";
            const char *welcome_msg = "Escreve o comando da operação que deseja realizar: \n";
            if (is_tcp) {
                write(client_fd, login_msg, strlen(login_msg));
                write(client_fd, welcome_msg, strlen(welcome_msg));
            } else {
                sendto(sock_udp, login_msg, strlen(login_msg), 0, (struct sockaddr*)&client_addr, sizeof(client_addr));
                sendto(sock_udp, welcome_msg, strlen(welcome_msg), 0, (struct sockaddr*)&client_addr, sizeof(client_addr));
            }
        } else {
            char *resposta;
            if (is_tcp) {
                int newline_index = strcspn(buffer, "\n");
                    if (newline_index < strlen(buffer)) {
                        buffer[newline_index] = '\0';
                    }
                    if(strncmp(permiss, "aluno", strlen("aluno")) ==0 ){ // se não for tcp e não for admistrador
                        resposta = comandosTCP_Aluno(buffer,nome_funcoes,porta_tcp);
                    }else if(strncmp(permiss, "professor", strlen("professor")) ==0 ){ // se não for tcp e não for admistrador
                        resposta = comandosTCP_Professor(buffer,nome_funcoes,porta_tcp);
                    }

            } else {
                int newline_index = strcspn(buffer, "\n");
                    if (newline_index < strlen(buffer)) {
                        buffer[newline_index] = '\0';
                    }
                resposta = comandosUDP(buffer);
                
            }
            

            if (is_tcp) {
                write(client_fd, resposta, strlen(resposta));
            } else {
                sendto(sock_udp, resposta, strlen(resposta), 0, (struct sockaddr*)&client_addr, sizeof(client_addr));
            }
        }
    }

    printf("Connection terminated with %s:%d\n", ip, ntohs(client_addr.sin_port));
    close(client_fd);
    conf_login = 0; // Reinicia o estado de login ao encerrar a conexão
}

void process_client(int client_fd, struct sockaddr_in client_addr) {
    char ip[INET_ADDRSTRLEN];
    char * permiss;
    inet_ntop(client_addr.sin_family, &(client_addr.sin_addr), ip, INET_ADDRSTRLEN);
    printf("Connection established with %s:%d\n", ip, ntohs(client_addr.sin_port));
    controla_comandos(client_fd, 1, client_addr,permiss);
    printf("Connection terminated with %s:%d\n", ip, ntohs(client_addr.sin_port));
    close(client_fd);
}

void process_client_udp(char *buffer, ssize_t bytes_received, struct sockaddr_in client_addr) {
    char ip[INET_ADDRSTRLEN];
    char * permiss;
    inet_ntop(client_addr.sin_family, &(client_addr.sin_addr), ip, INET_ADDRSTRLEN);
    buffer[bytes_received] = '\0';
    socklen_t addrSize = sizeof(client_addr);

    controla_comandos(sock_udp, 0, client_addr,permiss); // Chama controla_comandos para processar os comandos
}

int Servertcp(int porta) {
    struct sockaddr_in addr, client_addr;
    if ((sock_tcp = socket(AF_INET, SOCK_STREAM, 0)) < 0) {
        erro("Error creating TCP socket");
        return -1;
    }

    ficheiro = lerFicheiro("logins.txt", &tamFich);

    bzero((void *)&addr, sizeof(addr));
    addr.sin_family = AF_INET;
    addr.sin_addr.s_addr = htonl(INADDR_ANY);
    addr.sin_port = htons(porta);

    if (bind(sock_tcp, (struct sockaddr *)&addr, sizeof(addr)) < 0) {
        erro("Error binding TCP socket");
        close(sock_tcp);
        free(ficheiro);
        exit(1);
    }

    if (listen(sock_tcp, 5) < 0) {
        erro("Error listening on TCP socket");
        close(sock_tcp);
        free(ficheiro);
        return -1;
    }

    socklen_t client_addr_size = sizeof(client_addr);
    signal(SIGINT, termina);
    while (1) {
        while (waitpid(-1, NULL, WNOHANG) > 0);
        int cliente = accept(sock_tcp, (struct sockaddr *)&client_addr, &client_addr_size);
        if (cliente > 0) {
            if (fork() == 0) {
                close(sock_tcp);
                process_client(cliente, client_addr);
                exit(0);
            }
            close(cliente);
        }
    }
}

int Serverudp(int porta) {
    struct sockaddr_in addr, client_addr;
    if ((sock_udp = socket(AF_INET, SOCK_DGRAM, 0)) < 0) {
        erro("Error creating UDP socket");
        return -1;
    }

    ficheiro = lerFicheiro("logins.txt", &tamFich);

    bzero((void *)&addr, sizeof(addr));
    addr.sin_family = AF_INET;
    addr.sin_addr.s_addr = htonl(INADDR_ANY);
    addr.sin_port = htons(porta);

    if (bind(sock_udp, (struct sockaddr *)&addr, sizeof(addr)) < 0) {
        erro("Error binding UDP socket");
        close(sock_udp);
        free(ficheiro);
        exit(1);
    }

    signal(SIGINT, termina);

    ssize_t bytes_received;
    char buffer[BUF_SIZE];
    socklen_t addrSize = sizeof(client_addr);

    while (1) {
        bytes_received = recvfrom(sock_udp, buffer, BUF_SIZE, 0, (struct sockaddr *)&client_addr, &addrSize);
        if (bytes_received < 0) {
            perror("Error receiving data");
            continue;
        }
        
        if (bytes_received > 0) {
            process_client_udp(buffer, bytes_received, client_addr);
        }
    }
    
    close(sock_udp);
}

int main(int argc, char** argv) {
    if (argc != 4) {
        printf("%s <porta_turma> <porta_udp> <filename>\n", argv[0]);
        return -1;
    }
    porta_udp = atoi(argv[2]) ;
    porta_tcp = atoi(argv[1]);
    signal(SIGINT, termina);
    if (fork() == 0) {
        Servertcp(atoi(argv[1]));
    }

    if (fork() == 0) {
        Serverudp(atoi(argv[2]));
    }

    wait(NULL);
    wait(NULL);

    return 0;
}