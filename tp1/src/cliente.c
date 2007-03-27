/*
 * cliente.c
 */

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h> 
#include "conexion.c"								/*no se como incluir el .h de conexion.. */
#define PORTREMOTO 2222

#define DEBUG 1


int main(int argc, char *argv[]) {
	int sockfd, portno, n, cant;
	struct sockaddr_in serv_addr;
	struct hostent *server;
	char *buffer,*envbuffer;
	if (argc < 3) {
		fprintf(stderr,"Se usa: %s <HOSTIP> <BUSQUEDA>\n", argv[0]);
		exit(0);
	}
	portno = PORTREMOTO;
	bzero((char *) &serv_addr, sizeof(serv_addr));

	/*se conecta*/
	sockfd = abrirSocket();											/* Creo un socket para escuchas peticiones */
		#if DEBUG
		printf("Iniciando cliente...\n"); 
		#endif
	inicializarDireccion(&serv_addr,inet_addr(argv[1]),portno);	/* Inicializo la estructura de direccion local */
		#if DEBUG
		printf("Binding...\n"); 
		#endif
	conectar(sockfd,&serv_addr);								/* Hago un bind entre el socket, la direccion y el puerto */
		#if DEBUG
		printf("Conectado.\n"); 
		#endif
	enviar(sockfd,argv[2]);
	leer(sockfd,buffer,&cant);
	printf("%s\n",buffer);
	close(sockfd);
	return 0;
}






