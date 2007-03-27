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
#include "conexion.c"

#define PORT 2222
#define TAMMAXDATO 256

int main(int argc, char *argv[]) {
	int socket_fd, socket_nuevo_fd, tam_dir_cliente;
	char buffer[TAMMAXDATO];
	char response[TAMMAXDATO];
	struct sockaddr_in dir_servidor, dir_cliente;
	int n,cant;

	socket_fd = abrirSocket();
		printf("Iniciando servidor...\n", inet_ntoa(dir_servidor.sin_addr)); 
	inicializarDireccion(&dir_servidor,INADDR_ANY,PORT);
		printf("Binding...\n"); 
	conectar(socket_fd,&dir_servidor);
		printf("Conectado.\n"); 
	escuchar(socket_fd);
		printf("Escuchando en IP: %s Puerto: 2222\n", inet_ntoa(dir_servidor.sin_addr)); 
	
	/*
	sa.sa_handler = sigchld_handler; // reap all dead processes
	sigemptyset(&sa.sa_mask);
	sa.sa_flags = SA_RESTART;
	if (sigaction(SIGCHLD, &sa, NULL) == -1) {
		perror("sigaction");
		exit(1);
	}
	*/

	while(1) {  // Bucle infinito. Se corta con senyales.
		int err = aceptarRequest(socket_fd, &socket_nuevo_fd, &dir_cliente);
		if (err)
			continue;
		
		if (!fork()) { // Este es el hijo.
			close(socket_fd); // ciero el primer Listener, el del padre porque no lo necesito.
				printf("Aceptando request de IP: %s Puerto: %d\n", inet_ntoa(dir_cliente.sin_addr),ntohs(dir_cliente.sin_port)); 
			
			/*Aca deberia leer los datos del cliente*/
			if ((cant=recv(socket_nuevo_fd, buffer, TAMMAXDATO-1, 0)) == -1) {
				perror("recv");
				exit(1);
			}
			/*MILTON's code.
			Yo cambie el read por el recv porque es no bloqueante. Una pavada. 
			n = read(socket_nuevo_fd,buffer,255);
			if (n < 0) error("ERROR reading from socket");
			*/
			buffer[cant] = '\0';
			
			/*Luego buscar en el archivo con lo que esta haciendo Cesar */

			/* Mostrar en pantalla en el formato que pide el TP */
			printf("El cliente dice: %s\n",buffer);

			if (send(socket_nuevo_fd, "Te digo Pong! en respuesta a:\n", 29, 0) == -1)
				perror("send");
			if (send(socket_nuevo_fd, buffer, cant, 0) == -1)
				perror("send");
			close(socket_nuevo_fd);
			exit(0);
		}
		close(socket_nuevo_fd);  // Cierro el descriptor en el padre del FORK
	}
	
	
	/*
	MILTON's code.

	listen(socket_fd,5);
	tam_dir_cliente = sizeof(dir_cliente);
	socket_nuevo_fd = accept(socket_fd, 
			(struct sockaddr *) &dir_cliente, 
			&tam_dir_cliente);

	if (socket_nuevo_fd < 0) 
		error("ERROR on accept");

	bzero(buffer,256);

	printf("Here is the message: %s\n",buffer);
	n = write(socket_nuevo_fd,"I got your message",18);
	if (n < 0) error("ERROR writing to socket");

	return 0; 
	*/
}
