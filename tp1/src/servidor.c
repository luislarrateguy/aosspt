/*
** server.c -- a stream socket server demo
*/

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <sys/wait.h>
#include <signal.h>

#define PORT 2222
#define ADDR "192.168.0.2"

#define BACKLOG 10	 // how many pending connections queue will hold
#define MAXDATASIZE 100 

void sigchld_handler(int s)
{
	while(waitpid(-1, NULL, WNOHANG) > 0);
}

int main(void)
{
	int fd, nfd;  		// listen en sock_fd. Las aceptadas las abro en nfd
	struct sockaddr_in myaddr;
	struct sockaddr_in remoteaddr;
	socklen_t sin_size;
	struct sigaction sa;
	int yes=1;
	char buf[MAXDATASIZE];
	int cant;

	if ((fd = socket(AF_INET, SOCK_STREAM, 0)) == -1) {
		perror("socket");
		exit(1);
	}

	if (setsockopt(fd, SOL_SOCKET, SO_REUSEADDR, &yes, sizeof(int)) == -1) {
		perror("setsockopt");
		exit(1);
	}
	
	myaddr.sin_family = AF_INET;
	myaddr.sin_port = htons(PORT);	 // BigEndian <- Formato de Red.
	myaddr.sin_addr.s_addr = htonl(INADDR_ANY); // La IP que tenga el servidor.
	memset(&(myaddr.sin_zero), '\0', 8); // zero the rest of the struct

	// Hago un bind, para enlazarme a la direccion disponible local.
	if (bind(fd, (struct sockaddr *)&myaddr, sizeof(struct sockaddr)) == -1) {
		perror("bind");
		exit(1);
	}

	// Activo la ecucha de peticiones.
	if (listen(fd, BACKLOG) == -1) {
		perror("listen");
		exit(1);
	}

	printf("Escuchando en IP: %s Puerto: 2222\n", inet_ntoa(myaddr.sin_addr)); 

	sa.sa_handler = sigchld_handler; // reap all dead processes
	sigemptyset(&sa.sa_mask);
	sa.sa_flags = SA_RESTART;
	if (sigaction(SIGCHLD, &sa, NULL) == -1) {
		perror("sigaction");
		exit(1);
	}

	while(1) {  // Bucle infinito. Se corta con señales.
		sin_size = sizeof(struct sockaddr_in);
		// Comienzo a aceptar requests.
		if ((nfd = accept(fd, (struct sockaddr *)&remoteaddr, &sin_size)) == -1) {
			perror("accept");
			continue;
		}
		if (!fork()) { // Este es el hijo.
			close(fd); // child doesn't need the listener
			if ((cant=recv(nfd, buf, MAXDATASIZE-1, 0)) == -1) {
				perror("recv");
				exit(1);
			}
		
			buf[cant] = '\0';

			printf("Request de IP: %s Puerto: %d\n\n", inet_ntoa(remoteaddr.sin_addr),ntohs(remoteaddr.sin_port)); 
			printf("El cliente dice: %s\n",buf);
			if (send(nfd, "Pong!\n", 14, 0) == -1)
				perror("send");
			close(nfd);
			exit(0);
		}
		close(nfd);  // Cierro el descriptor en el padre del FORK
	}

	return 0;
}

