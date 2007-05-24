#include "common.h"

void fatal(string mensaje) {
	fprintf(stderr, "%s\n", mensaje);
	exit(1);
}

void inicializar(struct sockaddr_in *canal, int puerto, bool any) {
	int b;
	struct hostent* nombre_local = gethostbyname("localhost");

	/* Creo el socket */
	sock = socket(PF_INET, SOCK_DGRAM, IPPROTO_UDP);
	if (sock < 0)
		fatal("inicializar: No se pudo crear el socket.");


	/* Inicializo el canal */
	memset(&canal, 0, sizeof(*canal));
   	canal->sin_family = AF_INET;
   	canal->sin_port = htons(puerto);

    /* canal que puede recibir/enviar de cualquiera */
	if (any) {
	    canal->sin_addr.s_addr = htonl(INADDR_ANY);
    } else {
    	memcpy(&canal->sin_addr.s_addr, nombre_local->h_addr,
    		nombre_local->h_length);   
    }

	b = bind(sock, (struct sockaddr*) canal,
		sizeof(*canal));
	if (b < 0) fatal("Error al ejecutar bind.");

	len_canal = sizeof(*canal);

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
			&len_canal) < 0)
		fatal("receive_msg: Error en recvfrom.");

	return;
}

