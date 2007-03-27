/*
 * servidor.c
 */

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include <string.h>
#include <sys/types.h> 
#include <sys/socket.h>
#include <netinet/in.h>

/*void error(char *msg) {
	perror(msg);
	exit(1);
}*/

int main(int argc, char *argv[]) {
	int socket_fd, socket_nuevo_fd, nro_puerto, tam_dir_cliente;
	char buffer[256];
	struct sockaddr_in dir_servidor, dir_cliente;
	int n;

	if (argc < 2) {
		fprintf(stderr,"ERROR, no port provided\n");
		exit(1);
	}

	socket_fd = socket(AF_INET, SOCK_STREAM, 0);

	if (socket_fd < 0) 
		error("ERROR opening socket");

	bzero((char *) &dir_servidor, sizeof(dir_servidor));
	nro_puerto = atoi(argv[1]);
	dir_servidor.sin_family = AF_INET;
	dir_servidor.sin_addr.s_addr = INADDR_ANY;
	dir_servidor.sin_port = htons(nro_puerto);

	if (bind(socket_fd, (struct sockaddr *) &dir_servidor,
			sizeof(dir_servidor)) < 0) 
		error("ERROR on binding");

	listen(socket_fd,5);
	tam_dir_cliente = sizeof(dir_cliente);
	socket_nuevo_fd = accept(socket_fd, 
			(struct sockaddr *) &dir_cliente, 
			&tam_dir_cliente);

	if (socket_nuevo_fd < 0) 
		error("ERROR on accept");

	bzero(buffer,256);
	n = read(socket_nuevo_fd,buffer,255);
	if (n < 0) error("ERROR reading from socket");
	printf("Here is the message: %s\n",buffer);
	n = write(socket_nuevo_fd,"I got your message",18);
	if (n < 0) error("ERROR writing to socket");

	return 0; 
}

