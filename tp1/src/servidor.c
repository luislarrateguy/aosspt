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
#include <sys/wait.h>
#include <signal.h>
#include "conexion.c"														/*no se como incluir el .h de conexion.. */

#define PORT 2222
#define DEBUG 1

void sigchld_handler(int);
void buscarRespuesta(char *,char *);

int main(int argc, char *argv[]) {
	int socket_fd, socket_nuevo_fd, tam_dir_cliente;		/* socket que escucha peticiones, socket para procesar request, tam. est direccion */
	char *request = malloc(256);							/* buffer de lectura */
	char *response = malloc(256);							/* buffer de escritura */
	struct sockaddr_in dir_servidor, dir_cliente;			/*  */
	int n,cant;												/* cant=Cantidad de datos recibidos. */
	struct sigaction sa;									/* Utilizado para manejar las senyales al proceso */

	sa.sa_handler = sigchld_handler;						/* handler de senyales */
	sigemptyset(&sa.sa_mask);
	sa.sa_flags = SA_RESTART;
	if (sigaction(SIGCHLD, &sa, NULL) == -1) {
		free(response);
		free(request);
		close(socket_fd);
		perror("sigaction");
		exit(1);
	}
	
	socket_fd = abrirSocket();								/* Creo un socket para escuchas peticiones */
		#if DEBUG
		printf("Iniciando servidor...\n"); 
		#endif
	inicializarDireccion(&dir_servidor,INADDR_ANY,PORT);	/* Inicializo la estructura de direccion local */
		#if DEBUG
		printf("Binding...\n"); 
		#endif
	ligar(socket_fd,&dir_servidor);							/* Hago un bind entre el socket, la direccion y el puerto */
		#if DEBUG
		printf("Conectado.\n"); 
		#endif
	escuchar(socket_fd);									/* Activo la escucha */
		printf("Escuchando en IP: %s Puerto: 2222\n", inet_ntoa(dir_servidor.sin_addr)); 


	while(1) {  // Bucle infinito. Se corta con senyales.
		int err = aceptarRequest(socket_fd, &socket_nuevo_fd, &dir_cliente);	/* Creo un socket cuando recibo 
																				una request, para leer los datos */
		if (err)
			continue;
		
		if (!fork()) {			/* Este es el hijo. El socket original continua en el padre */
			close(socket_fd);	/* por lo tanto ciero el primer socket.*/
				#if DEBUG			
				printf("Aceptando request de IP: %s Puerto: %d\n", 
						inet_ntoa(dir_cliente.sin_addr),ntohs(dir_cliente.sin_port)); 
				#endif
			leer(socket_nuevo_fd,request, &cant);		/* Lee los datos enviados por el cliente */
			buscarRespuesta(request,response);			/* Busca una respuesta y arma el resultado en "response" */
				#if DEBUG
				printf("El cliente dice: %s\n",request);
				#endif	
			enviar(socket_nuevo_fd,response);
			close(socket_nuevo_fd);
			exit(0);
		}
		close(socket_nuevo_fd);  						/* Cierro el descriptor en el padre del FORK */
	}
	free(response);
	free(request);
	return 0;
}

void buscarRespuesta(char *request,char *response) {
	/* acá se debería armar la respuesta del servidor */
	char *c = strcpy(response,request);
}

void sigchld_handler(int s)
{
	while(waitpid(-1, NULL, WNOHANG) > 0);
}
