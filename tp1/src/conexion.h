/*
* conexion.h
*/

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include <string.h>
#include <sys/types.h> 
#include <sys/socket.h>
#include <netinet/in.h>

#define BACKLOG 10
#define TAMMAXDATO 256
typedef char string256[TAMMAXDATO];
typedef char* string;


int abrirSocket();
void inicializarDireccion(struct sockaddr_in*,long,int);
void conectar(int,struct sockaddr_in*);
void ligar(int,struct sockaddr_in*);
void escuchar(int);
int aceptarRequest(int, int*, struct sockaddr_in *);
void error(char *);
