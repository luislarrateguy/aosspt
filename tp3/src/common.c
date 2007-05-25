#include "common.h"

void fatal(string mensaje) {
	fprintf(stderr, "%s\n", mensaje);
	exit(1);
}
void debug(string mensaje) {
	if (debugging)
		fprintf(stdout, "%s\n", mensaje);   
}

int inicializar(struct sockaddr_in *canal, int puerto, bool any, bool envio) {
	int b;
	struct hostent* nombre_local = gethostbyname("localhost");

	/* Creo el socket */
	int sk = socket(PF_INET, SOCK_DGRAM, IPPROTO_UDP);
	if (sk < 0)
		fatal("inicializar: No se pudo crear el socket.");

	/* Inicializo el canal */
	memset(canal, 0, sizeof(*canal));
   	canal->sin_family = AF_INET;
   	canal->sin_port = htons(puerto);

	/* canal que puede recibir/enviar de cualquiera */
	if (!envio) {
		canal->sin_addr.s_addr = htonl(INADDR_ANY);
		b = bind(sk, (struct sockaddr*) canal,
			sizeof(*canal));
		if (b < 0) 
			fatal("Error al ejecutar bind.");
	} else {
		memcpy(&canal->sin_addr.s_addr, nombre_local->h_addr,
		nombre_local->h_length);
	}

	len_canal = sizeof(*canal);
	return sk;
}

void send_msg(struct msg mensaje, int puerto_destino) {
	if (!inicializada)
		fatal("send: conexión no inicializada aún.");

	canal_envio.sin_port = htons(puerto_destino);

	if (sendto(skw, (void*)&mensaje, sizeof(mensaje), 0,
		(struct sockaddr*)&canal_envio, sizeof(struct sockaddr_in)) < 0)
		fatal("send_msg: Error al enviar mensaje.");

	return;
}

void receive_msg(struct msg* mensaje) {
	if (!inicializada)
		fatal("receive: conexión no inicializada aún.");

	if (recvfrom(skr, (void *)mensaje, sizeof(*mensaje), 0,
			(struct sockaddr*)&canal_recepcion,
			&len_canal) < 0)
		fatal("receive_msg: Error en recvfrom.");

	return;
}

