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
#define REQUEST	 1
#define ENTRAR_RC   2
#define PRIVILEGE   3
#define SALIR_RC	4
#define HELLO	   5

#define FALSE 0
#define TRUE 1

typedef char* string;
typedef int bool;

struct msg {
	int tipo;
	int from;
};

int skr,skw;
struct sockaddr_in canal_recepcion;
struct sockaddr_in canal_envio;
socklen_t len_canal;
bool inicializada;
bool debugging;

void fatal(string mensaje);
void debug(string mensaje);
int inicializar(struct sockaddr_in *canal, int puerto, bool any, bool envio);
void send_msg(struct msg mensaje, int puerto_destino);
void receive_msg(struct msg* mensaje);

#endif /* _common_h */

