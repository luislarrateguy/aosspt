#include <string.h>
#include <sys/types.h>
#include <sys/fcntl.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>

#include "datos.h"
#include "strings.c"

int main(int argc, char** argv) {
	int s, b, l, fd, sa, bytes, on = 1, encontrado;
	int size_sin = sizeof(struct sockaddr_in);
	char nombre_buscado[TAM_BUFFER];
	char buffer_envio[TAM_BUFFER];
	struct sockaddr_in canal,dir_cliente;
	
	FILE* archivo;
	char linea[81];

	/* Abrimos el archivo guia */
	if ((archivo = fopen(RUTA_ARCHIVO_GUIA, "r")) == NULL)
		fatal("No se pudo abrir el archivo guia");

	memset(&canal, 0, sizeof(canal));
	canal.sin_family = AF_INET;
	canal.sin_addr.s_addr = htonl(INADDR_ANY);
	canal.sin_port = htons(PUERTO_SERVIDOR);
	
	s = socket(PF_INET, SOCK_STREAM, IPPROTO_TCP);
	if (s < 0) fatal("No se pudo crear el socket");

	/* Manipulamos algunas opciones del socket:
	 *  - SOL_SOCKET especifica el nivel de las opciones que queremos
	 *    modificar.
	 *  - SO_REUSEADDR es la opción que vamos a modificar.
	 *  - Los siguientes argumentos activan dicha opción.  */ 
	setsockopt(s, SOL_SOCKET, SO_REUSEADDR, (char*) &on, sizeof(on));

	b = bind(s, (struct sockaddr*) &canal, sizeof(canal));
	if (b < 0) fatal("Error al ejecutar bind");

	l = listen(s, TAM_COLA);
	if (l < 0) fatal("Error al ejecutar listen");

	/* Esperamos una conexión y la atendemos. */
	while (1) {
		/* Nos bloqueamos para atender la solicitud. Si llega
		 * una nueva solicitud mientras procesamos esta, la
		 * misma se guardará en una cola. */
		 
		sa = accept(s, (struct sockaddr *) &dir_cliente, &size_sin);
		if (sa < 0) fatal("Error al ejecutar accept");
		printf("Recibiendo consulta del cliente (%s) puerto %d\n",
			inet_ntoa(dir_cliente.sin_addr),ntohs(dir_cliente.sin_port));
		read(sa, nombre_buscado, TAM_BUFFER);

		printf("El cliente solicita: '%s'\n", nombre_buscado);
		
		encontrado = 0;
		
		/* Mientras no se termine el archivo, leemos una linea nueva */		
		while (!feof(archivo))  {
			/* Leemos la linea */
			fgets(linea, TAM_LINEA, archivo);
			
			/* Quito el fin de linea ('\n') de la línea leída */
			linea[strlen(linea) - 1] = '\0';
			
			strcpy(buffer_envio, linea);
			
			/* Si la cadena recibida es subcadena de la linea leida
			   en el archivo, se la enviamos al cliente */
			if (substring(nombre_buscado, getNombre(linea))) {
				encontrado = encontrado + 1;
				printf("\tEnviando respuesta %d: '%s'\n",
					encontrado, buffer_envio);
				write(sa, buffer_envio, TAM_BUFFER);
			}
		}

		/* Si no encontramos ningun nombre relacionado retornamos
		   un mensaje de error */
		if (!encontrado) {
			write(sa, TEL_NO_ENCONTRADO,
				strlen(TEL_NO_ENCONTRADO) + 1);
			printf("\tNo se han encontraron coincidencias\n");
			
		}
		else {
			write(sa, FIN_DATOS, strlen(FIN_DATOS) + 1);
			printf("\tRespuesta a la consulta completada\n");
		}

		printf("----\n");

		close(sa);
		
		/* Volvemos a posicionarnos al principio del archivo */
		rewind(archivo);
	}
	
	/* Cerramos el archivo */
	if (fclose(archivo) == EOF)
		printf("Error al cerrar el archivo\n");
}

