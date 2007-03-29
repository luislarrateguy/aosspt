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
	char buffer[TAM_BUFFER];
	struct sockaddr_in canal;
	
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
	 *  - Los siguientes argumentos activan dicha opción.
	 */ 
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
		sa = accept(s, 0, 0);
		if (sa < 0) fatal("Error al ejecutar accept");

		read(sa, buffer, TAM_BUFFER);

		printf("El cliente envia: '%s'\n", buffer);
		
		encontrado = 0;
		
		/* Mientras no se termine el archivo, leemos una linea nueva */		
		while (!feof(archivo))  {
			/* Leemos la linea */
			fgets(linea, TAM_LINEA, archivo);
			
			/* Quito el fin de linea ('\n') de la línea leída */
			linea[strlen(linea) - 1] = '\0';
			
			/* Si la cadena recibida es subcadena de la linea leida
			   en el archivo, se la enviamos al cliente */
			if (substring(buffer, linea)) {
				encontrado = 1;
				write(sa, linea, TAM_LINEA);
			}
			
		}

		/* Si no encontramos ningun nombre relacionado retornamos
		   un mensaje de error */
		if (!encontrado)
			write(sa, TEL_NO_ENCONTRADO, strlen(TEL_NO_ENCONTRADO) + 1);
		else
			write(sa, FIN_DATOS, strlen(FIN_DATOS) + 1);

		close(sa);
		
		/* Volvemos a posicionarnos al principio del archivo */
		rewind(archivo);
	}
	
	/* Cerramos el archivo */
	if (fclose(archivo) == EOF)
		printf("Error al cerrar el archivo\n");
}
