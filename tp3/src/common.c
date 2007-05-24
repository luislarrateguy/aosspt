#include "common.h"

void fatal(string mensaje) {
	fprintf(stderr, "%s\n", mensaje);
	exit(1);
}

void inicializar_servidor(int puerto) {
	int b;
	struct hostent* nombre_local = gethostbyname("localhost");

	/* Verifico si ya no está inicializado */
	if (inicializada)
		return;

	/* Creo el socket */
	sock = socket(PF_INET, SOCK_DGRAM, IPPROTO_UDP);
	if (sock < 0)
		fatal("inicializar_servidor: No se pudo crear el socket.");

	/* Inicializo el canal de recepción */
	memset(&canal_recepcion, 0, sizeof(canal_recepcion));
	canal_recepcion.sin_family = AF_INET;
	canal_recepcion.sin_addr.s_addr = htonl(INADDR_ANY);
	canal_recepcion.sin_port = htons(puerto);

	b = bind(sock, (struct sockaddr*)&canal_recepcion,
		sizeof(canal_recepcion));
	if (b < 0) fatal("Error al ejecutar bind.");

	len_canal_recepcion = sizeof(canal_recepcion);

	/* Inicializo el canal de envío */
	memset(&canal_envio, 0, sizeof(canal_envio));
	canal_envio.sin_family = AF_INET;
	memcpy(&canal_envio.sin_addr.s_addr, nombre_local->h_addr,
		nombre_local->h_length);
	/* El puerto al canal de envío se lo setea antes de enviar un
	 * un mensaje.
	canal_envio.sin_port = htons(puerto);*/

	inicializada = TRUE;
}

void send_msg(struct msg mensaje, int puerto_destino) {
	if (!inicializada)
		fatal("send: conexión no inicializada aún.");

	canal_envio.sin_port = htons(puerto_destino);

	if (sendto(sock, (void*)&mensaje, sizeof(mensaje), 0,
		(struct sockaddr*)&canal_envio, sizeof(struct sockaddr_in)) < 0)
		fatal("send_msg: Error al enviar mensaje.");

	return;
}

void receive_msg(struct msg* mensaje) {
	if (!inicializada)
		fatal("receive: conexión no inicializada aún.");

	if (recvfrom(sock, (void *)mensaje, sizeof(*mensaje), 0,
			(struct sockaddr*)&canal_recepcion,
			&len_canal_recepcion) < 0)
		fatal("receive_msg: Error en recvfrom.");

	return;
}

