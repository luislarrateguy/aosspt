/*

  Copyright (c) 2007 by Luis I. Larrateguy, Milton Pividori y
  César Sandrigo. All Rights Reserved

  This program is free software; you can redistribute it and/or modify
  it under the terms of the GNU General Public License as published by
  the Free Software Foundation; either version 2 of the License, or
  (at your option) any later version.

  This program is distributed in the hope that it will be useful,
  but WITHOUT ANY WARRANTY; without even the implied warranty of
  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
  GNU General Public License for more details.

  You should have received a copy of the GNU General Public License
  along with this program; if not, write to the Free Software
  Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA  02110-1301  USA

*/

#include "common.h"

/* Utilizada para mostrar por consola un mensaje de error fatal que causa la
 * salida del programa */
void fatal(string mensaje) {
	fprintf(stderr, "%s\n", mensaje);
	exit(1);
}

/* Utilizada, en caso de que el debugging esté activado, para mostrar mensajes
 * de depuración por consola */
void debug(string mensaje) {
	if (debugging)
		printf("%s\n", mensaje);   
}

/* Es utilizada por el cliente y por el servidor. Es una función genérica.
 * Inicializa las estructuras de datos para que la comunicación sea posible
 * (sockets, canales, etc), así como otras estructuras de datos.  Cada proceso,
 * el cliente y el servidor, definen otra función: inicializar_cliente e
 * inicializar_servidor respectivamente, que se encargan de llamar a ésta con
 * los argumentos correctos */
int inicializar(struct sockaddr_in *canal, int puerto, bool any, bool envio) {
	int b, sk;
	struct hostent* nombre_local = gethostbyname("localhost");

	/* Inicializo el array de nombres de los mensajes, para mostrar éstos
	 * en lugar de sus números de identificación */
	nombre_mensajes[0] = "REQUEST";
	nombre_mensajes[1] = "ENTRAR_RC";
	nombre_mensajes[2] = "PRIVILEGE";
	nombre_mensajes[3] = "SALIR_RC";
	nombre_mensajes[4] = "HELLO";

	/* Creo el socket */
	sk = socket(PF_INET, SOCK_DGRAM, IPPROTO_UDP);
	if (sk < 0)
		fatal("inicializar: No se pudo crear el socket.");

	/* Inicializo el canal */
	memset(canal, 0, sizeof(*canal));
   	canal->sin_family = AF_INET;
   	canal->sin_port = htons(puerto);

	/* Si estamos inicializando una estructura sockaddr_in para recibir
	 * datos, entonces puede recibirlos de cualquier origen */
	if (!envio) {
		canal->sin_addr.s_addr = htonl(INADDR_ANY);
		b = bind(sk, (struct sockaddr*) canal,
			sizeof(*canal));
		if (b < 0) 
			fatal("inicializar: Error al ejecutar bind.");

	/* Si en cambios es para el envío, seteo la dirección IP de origen */
	} else {
		memcpy(&canal->sin_addr.s_addr, nombre_local->h_addr,
		nombre_local->h_length);
	}

	len_canal = sizeof(*canal);

	return sk;
}

/* Utilizada tanto por el cliente como por el servidor. Sólo hay que pasar la
 * estructura que representa el mensaje a enviar y el puerto destino.  Si la
 * recepción del datagrama falla, entonces se muestra un mensaje de error
 * fatal. */
void send_msg(struct msg mensaje, int puerto_destino) {
	if (!inicializada)
		fatal("send_msg: conexión no inicializada aún.");

	canal_envio.sin_port = htons(puerto_destino);

	if (sendto(skw, (void*)&mensaje, sizeof(mensaje), 0,
		(struct sockaddr*)&canal_envio, sizeof(struct sockaddr_in)) < 0)
		fatal("send_msg: Error al enviar el mensaje");

	return;
}

/* Es semejante a send_msg. Si la recepción del datagrama falla, entonces
 * muestra un mensaje por consola con la función fatal */
void receive_msg(struct msg* mensaje) {
	if (!inicializada)
		fatal("receive_msg: conexión no inicializada aún.");

	if (recvfrom(skr, (void *)mensaje, sizeof(*mensaje), 0,
			(struct sockaddr*)&canal_recepcion,
			&len_canal) < 0)
		fatal("receive_msg: Error al recibir el mensaje");

	return;
}

