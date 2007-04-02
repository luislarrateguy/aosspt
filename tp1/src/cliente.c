#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>

#include "datos.h"

int main(int argc, char** argv) {
	int conexion, s, cantRep = 0;

	/* El nombre del servidor (no la IP) */
	struct hostent *nombreServidor;

	/* Buffer para almacenar la respuesta del servidor */
	char buffer[TAM_BUFFER];

	/* Contiene la direcci�n IP */
	struct sockaddr_in canal;

	if (argc != 3) {
		fatal("Uso: rphone <IP-SERVIDOR> <NOMBRE-PERSONA>");
	}

	/* Supongo que he pasado como par�metro el nombre del host.
	 * Intento obtener la IP del mismo. Si esto falla, quiza el par�metro
	 * sea la direcci�n IP misma (es decir, no hace falta usar DNS) */
	nombreServidor = gethostbyname(argv[1]);

	if (!nombreServidor) {
		nombreServidor = gethostbyaddr(argv[1], strlen(argv[1]), AF_INET);

		if (!nombreServidor) error("La direcci�n del servidor es inv�lida.");
	}

	s = socket(PF_INET, SOCK_STREAM, IPPROTO_TCP);
	if (s < 0) fatal("No se pudo crear el socket");

	memset(&canal, 0, sizeof(canal));
	canal.sin_family = AF_INET;
	memcpy(&canal.sin_addr.s_addr, nombreServidor->h_addr,
		nombreServidor->h_length);
	canal.sin_port = htons(PUERTO_SERVIDOR);

	conexion = connect(s, (struct sockaddr*) &canal, sizeof(canal));
	if (conexion < 0) fatal("Fallo al intentar conectar");

	/* Se ha establecido la conexi�n. Se env�a el nombre de la persona a la cual
	 * queremos averiguar el tel�fono. */
	printf("Enviando consulta al servidor %s (%s), preguntando por: '%s'\n",
		argv[1],inet_ntoa(canal.sin_addr),argv[2]);
	write(s, argv[2], strlen(argv[2]) + 1);

	/* Obtiene los datos y los escribe en la salida estandar. */
	printf("Respuesta del servidor:\n");

	while (read(s, buffer, TAM_BUFFER) != 0) {
		if (strcmp(buffer, TEL_NO_ENCONTRADO) == 0) {
			printf("\tNo hay entradas que sasfagan la b�squeda.\n");
			return 0;
		}
		else if (strcmp(buffer, FIN_DATOS) == 0)
			return 0;
		else {
			cantRep = cantRep + 1;
			printf("\tCoincidencia %d: '%s'\n", cantRep, buffer);
		}
	}

	return 0;
}

