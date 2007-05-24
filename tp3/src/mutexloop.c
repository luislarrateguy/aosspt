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
	int puerto;
	struct msg mensaje;
	struct hostent *nombre_local;

	if (argc != 2) fatal("Uso: mutexloop <puerto>");

	puerto = atoi(argv[1]);

	/* Inicializo las estructuras para la comunicación */
	inicializar_servidor(puerto);
	
	mensaje.tipo = 45;
	send_msg(mensaje, puerto);
	
	while(TRUE)	{
		printf("Estoy en la FUERA de la Región Crítica\n");
		printf("Presionar una tecla para Ingresar a la RC...\n");
		getc(); /* leer del teclado */
		Entrar_RC();
		printf("Estoy en la DENTRO de la Región Crítica\n");
		printf("Presionar una tecla para Salir de la RC...\n");
		getc(); /* leer del teclado */
		Salir_RC();
	}
	return 0;
}

