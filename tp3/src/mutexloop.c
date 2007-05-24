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

#include "common.h"

#define PUERTO_INICIAL 6001

int puertoServer;
int puertoLocal;

void entrar_rc() {
    struct msg mensajeEntrar,mensaje;
	mensajeEntrar.tipo = ENTRAR_RC;
	mensajeEntrar.from = puertoLocal;
	send_msg(mensajeEntrar, puertoServer);
	
	/* Se bloquea hasta que reciba el msg.. o no?*/
	receive_msg(&mensaje);
}

void salir_rc() {
    struct msg mensaje;
	mensaje.tipo = SALIR_RC;
	mensaje.from = puertoLocal;
	send_msg(mensaje, puertoServer);
}

int main(int argc, char* argv[]) {
	struct hostent *nombre_local;

	if (argc != 2) fatal("Uso: mutexloop <puerto>");
	
	/* Podriamos definir que el puerto del cliente es el puerto asignado al 
	 * servidor al que se conecta mas 1000 
	 * Ej: #mutexloop 5001
	 * usara el 6001 */
	puertoServer = atoi(argv[1]);
	puertoLocal  = puertoServer + 1000;

	/* Inicializo las estructuras para la comunicación */
	inicializar(&canal_envio,puertoServer,FALSE);
	inicializar(&canal_recepcion,puertoLocal,FALSE);
	
	while(TRUE)	{
		printf("Estoy en la FUERA de la Región Crítica\n");
		printf("Presionar una tecla para Ingresar a la RC...\n");
		getchar(); /* leer del teclado */
		entrar_rc();
		printf("Estoy en la DENTRO de la Región Crítica\n");
		printf("Presionar una tecla para Salir de la RC...\n");
		getchar(); /* leer del teclado */
		salir_rc();
	}
	return 0;
}

