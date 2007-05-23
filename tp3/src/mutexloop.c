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

#include <stdio.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>

#include "common.c"

int main(int argc, char* argv[]) {
	int s, puerto, bytes_enviados;
	struct msg mensaje;
	struct sockaddr_in canal;
	struct hostent *nombre_local;

	if (argc != 2) fatal("Uso: mutexloop <puerto>");

	puerto = atoi(argv[1]);

	s = socket(PF_INET, SOCK_DGRAM, IPPROTO_UDP);
	if (s < 0) fatal("No se pudo crear el socket.");

	nombre_local = gethostbyname("localhost");

	memset(&canal, 0, sizeof(canal));
	canal.sin_family = AF_INET;
	memcpy(&canal.sin_addr.s_addr, nombre_local->h_addr,
		nombre_local->h_length);
	canal.sin_port = htons(puerto);

	mensaje.tipo = 45;
	bytes_enviados = sendto(s, (void*)&mensaje, sizeof(mensaje), 0, (struct sockaddr*)&canal, sizeof(struct sockaddr_in));
	if (bytes_enviados < 0)
		fatal("Error al enviar mensaje.");

	close(s);
	return 0;
}

