#ifndef _common_h

#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/fcntl.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>
#include <string.h>

#define TAM_LINEA 81

/* Códigos de los distintos tipos de mensajes */
#define REQUEST    1
#define ENTRAR_RC  2
#define PRIVILEGE 3
#define SALIR_RC   4

#define FALSE 0
#define TRUE 1

typedef char* string;
typedef int bool;

struct msg {
	int tipo;
	int from;
};

int sock;
struct sockaddr_in canal_recepcion;
struct sockaddr_in canal_envio;
socklen_t len_canal_recepcion;
bool inicializada;

void fatal(char* mensaje);
void inicializar_servidor(int puerto);
void send_msg(struct msg mensaje, int puerto_destino);
void receive_msg(struct msg* mensaje);

#endif /* _common_h */

