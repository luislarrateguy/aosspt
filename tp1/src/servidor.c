#include <string.h>
#include <sys/types.h>
#include <sys/fcntl.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>

#include "datos.h"

int main(int argc, char** argv) {
	int s, b, l, fd, sa, bytes, on = 1;
	char buffer[TAM_BUFFER];
	struct sockaddr_in canal;

	memset(&canal, 0, sizeof(canal));
	canal.sin_family = AF_INET;
	canal.sin_addr.s_addr = htonl(INADDR_ANY);
	canal.sin_port = htons(PUERTO_SERVIDOR);
	
	/* Apertura pasiva. Espera una conexión. */
	s = socket(PF_INET, SOCK_STREAM, IPPROTO_TCP);
	if (s < 0) fatal("No se pudo crear el socket");
	setsockopt(s, SOL_SOCKET, SO_REUSEADDR, (char*) &on, sizeof(on));

	b = bind(s, (struct sockaddr*) &canal, sizeof(canal));
	if (b < 0) fatal("Error al ejecutar bind");

	l = listen(s, TAM_COLA);
	if (l < 0) fatal("Error al ejecutar listen");

	/* Esperamos una conexión y la atendemos. */
	while (1) {
		/* Nos bloqueamos para atender la solicitud. */
		sa = accept(s, 0, 0);
		if (sa < 0) fatal("Error al ejecutar accept");

		read(sa, buffer, TAM_BUFFER);

		/* TODO: Código para buscar el número telefónico */

		printf("Se recibió del cliente: '%s'\n", buffer);
		write(sa, buffer, TAM_BUFFER);

		close(sa);
	}
}

