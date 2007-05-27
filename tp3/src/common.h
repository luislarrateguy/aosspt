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

#ifndef _common_h

#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/fcntl.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>
#include <string.h>

#define TAM_LINEA 81

/* Códigos de los distintos tipos de mensajes */
#define REQUEST		0
#define ENTRAR_RC	1
#define PRIVILEGE	2
#define SALIR_RC	3
#define HELLO		4

/* Array que contiene el nombre los mensajes. Así:
 * nombre_mensajes[0] = "REQUEST"
 * nombre_mensaje[4] = "HELLO".
 * Es para el entendimiento de las salidas por consola. */
char* nombre_mensajes[5];

#define FALSE		0
#define TRUE		1

typedef char* string;
typedef int bool;

/* Estructura de mensaje. Es utilizada por el servidor y por el cliente para
 * enviar un mensaje. El campo 'tipo' es uno de los nombrados arriba: REQUEST,
 * ENTRAR_RC, etc. El campo 'from' lo utiliza el emisor para indicarle al
 * receptor en qué puerto va a esperar la respuesta (si es que el mensaje
 * emitido requiere una respuesta) */
struct msg {
	int tipo;
	int from;
};

/* Sockets de lectura y escritura */
int skr,skw;

/* Canales utilizados para recibir y enviar información */
struct sockaddr_in canal_recepcion;
struct sockaddr_in canal_envio;
socklen_t len_canal;

/* Es utilizado por las funciones send_msg, receive_msg, entrar_rc y salir_rc
 * para saber si todas las estructuras, variables y sockets necesarios han sido
 * inicializados */
bool inicializada;

/* En caso de ser TRUE activa el debug por consola */
bool debugging;

void fatal(string mensaje);
void debug(string mensaje);
int inicializar(struct sockaddr_in *canal, int puerto, bool any, bool envio);
void finalizar_conexiones();

void send_msg(struct msg mensaje, int puerto_destino);
void receive_msg(struct msg* mensaje);

#endif /* _common_h */

