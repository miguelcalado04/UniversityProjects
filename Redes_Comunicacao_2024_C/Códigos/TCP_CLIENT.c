#include <stdio.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <netdb.h>
#include <arpa/inet.h>

#define BUF_SIZE	1024

char* extrair_multicast(const char* buffer) {
    const char* chave = "Endereço Multicast:";
    char* pos = strstr(buffer, chave);
    
    if (pos == NULL) {
        return NULL;
    }
    
    // Mover o ponteiro para o final da chave
    pos += strlen(chave);
    
    // Ignorar espaços em branco após a chave
    while (*pos == ' ' || *pos == '\t') {
        pos++;
    }
    
    // Encontrar o final do endereço (considerar espaço ou nova linha)
    const char* fim = pos;
    while (*fim != '\0' && *fim != '\n' && *fim != ' ') {
        fim++;
    }
    
    // Calcular o comprimento do endereço
    size_t comprimento = fim - pos;
    
    // Alocar memória para o endereço e copiar
    char* endereco = (char*)malloc(comprimento + 1);
    if (endereco == NULL) {
        return NULL;
    }
    strncpy(endereco, pos, comprimento);
    endereco[comprimento] = '\0';
    
    return endereco;
}

void multicast_receber(char* endereco) {
    int sock;
    struct sockaddr_in addr;
    char msg[BUF_SIZE];
    socklen_t addrlen = sizeof(addr); // Inicializando addrlen

    // Criar um socket UDP
    if ((sock = socket(AF_INET, SOCK_DGRAM, 0)) < 0) {
        perror("socket");
        exit(1);
    }

    // Configurar a estrutura de endereço multicast
    memset(&addr, 0, sizeof(addr));
    addr.sin_family = AF_INET;
    addr.sin_addr.s_addr = INADDR_ANY; // Mudança para INADDR_ANY para receber de qualquer endereço
    addr.sin_port = htons(5000);

    // Vincular o socket à porta
    if (bind(sock, (struct sockaddr *)&addr, sizeof(addr)) < 0) {
        perror("bind");
        exit(1);
    }

    // Juntar-se ao grupo multicast
    struct ip_mreq mreq;
    mreq.imr_multiaddr.s_addr = inet_addr(endereco);
    mreq.imr_interface.s_addr = INADDR_ANY;
    if (setsockopt(sock, IPPROTO_IP, IP_ADD_MEMBERSHIP, &mreq, sizeof(mreq)) < 0) {
        perror("setsockopt");
        exit(1);
    }

    while (1) {
    // Receber a mensagem multicast
        int nbytes;
        if ((nbytes = recvfrom(sock, msg, sizeof(msg), 0, (struct sockaddr *)&addr, &addrlen)) < 0) {
            perror("recvfrom");
            exit(1);
        }
        msg[nbytes] = '\0'; // Terminar a string recebida

        printf("Mensagem multicast recebida: %s\n", msg);
    fflush(stdout);
    }

    // Sair do grupo multicast
    if (setsockopt(sock, IPPROTO_IP, IP_DROP_MEMBERSHIP, &mreq, sizeof(mreq)) < 0) {
        perror("setsockopt");
        exit(1);
    }

    close(sock);
}



int main(int argc, char *argv[]) {
	
    char endServer[100];
  	int fd;
  	struct sockaddr_in addr;
  	struct hostent *hostPtr;
	//garantir que o cliente passa o host e a porta para a qualquer quer comunicar
  	if (argc != 3) {
    	printf("%s <host> <port>\n", argv[0]);
    	return -1;
  	}

  	strcpy(endServer, argv[1]);
  	if ((hostPtr = gethostbyname(endServer)) == 0) {
    	perror("Não consegui obter endereço");
		return -1;
	}

  	bzero((void *) &addr, sizeof(addr));
  	addr.sin_family = AF_INET;
  	addr.sin_addr.s_addr = ((struct in_addr *)(hostPtr->h_addr_list[0]))->s_addr;
  	addr.sin_port = htons((short) atoi(argv[2]));

  	if ((fd = socket(AF_INET,SOCK_STREAM,0)) == -1) {
		perror("socket");
		return -1;
  	}

  	if (connect(fd,(struct sockaddr *)&addr,sizeof (addr)) < 0) {
	  	perror("Connect");
		return -1;
	}
	int aux, nread;
	char buffer[BUF_SIZE];

	nread = read(fd, buffer, BUF_SIZE);
	printf("%s\n", buffer);
	
  	while (1) {
		
		// ler o input do cliente
		fgets(buffer, BUF_SIZE, stdin);
		buffer[strlen(buffer) - 1] = '\0';
    	
		// mandar o input para o servidor
		aux = write(fd, buffer, 1 + strlen(buffer));
		if (aux == -1) break;
		
		//ler a resposta enviada pelo servidor
    	nread = read(fd, buffer, BUF_SIZE);

        if(strncmp(buffer,"Endereço Multicast:", 17)==0){
            char * multicast = extrair_multicast(buffer);
            if (fork() == 0) {
		        multicast_receber(multicast);
                exit(0);
            }
        }

    	if (nread < 1) break;
		    buffer[nread] = '\0';
    	printf("%s\n", buffer);
  	}

  	close(fd);
  	return 0;
}