#ifndef COMANDOS_h
#define COMANDOS_h

int verificaNumero(char* str);
char *comandosTCP_Aluno(char* buf, char* buffer_copy, int porta);
char *comandosTCP_Professor(char* buf, char* buffer_copy,int porta);
char *comandosUDP(char* buf);
#endif