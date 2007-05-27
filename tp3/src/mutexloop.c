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

/* Variables globales del cliente */
int puertoServer;
int puertoLocal;

/* Se encarga de llamar a la función genérica 'inicializar' con los argumentos
 * correctos para un proceso 'mutexloop' */
void inicializar_cliente() {
	skr = inicializar(&canal_recepcion, puertoLocal, FALSE, FALSE);
	skw = inicializar(&canal_envio, puertoServer, FALSE, TRUE);
	inicializada = TRUE;
}

/* Entra a la región crítica. Es bloqueante obviamente */
void entrar_rc() {
	if (!inicializada)
		fatal("entrar_rc: conexión no inicializada aún.");

	struct msg mensajeEntrar, mensaje;
	mensajeEntrar.tipo = ENTRAR_RC;
	mensajeEntrar.from = puertoLocal;

	send_msg(mensajeEntrar, puertoServer);
	
	/* Se bloquea hasta que reciba el msg */
	receive_msg(&mensaje);

	if (mensaje.tipo != PRIVILEGE)
		fatal("entrar_rc: Respuesta incorrecta");
}

/* Libera la región crítica. Obviamente, no es bloqueante */
void salir_rc() {
	if (!inicializada)
		fatal("salir_rc: conexión no inicializada aún.");

	struct msg mensaje;
	mensaje.tipo = SALIR_RC;
	mensaje.from = puertoLocal;

	send_msg(mensaje, puertoServer);
}

int main(int argc, char* argv[]) {
	struct hostent *nombre_local;

	if (argc != 2) fatal("Uso: mutexloop <puerto>");
	
	/* El puerto que utiliza el cliente para escuchar las respuestas
	 * de su mutexd es el mismo que el servidor más 1000. De esta forma
	 * cada servidor y cada cliente utilizan un puerto distinto (recordar
	 * que están en la misma máquina) para escuchar los mensajes de los
	 * demás hacia él. */
	puertoServer = atoi(argv[1]);
	puertoLocal  = puertoServer + 1000;

	/* Inicializo las estructuras para la comunicación */
	inicializar_cliente();
	
	while(TRUE)	{
		printf("Estoy FUERA de la Región Crítica\n");
		printf("Presionar una tecla para Ingresar a la RC...\n");
		
		/* Entra a la región crítica */
		getchar(); /* lee del teclado */
		printf("Intentando entrar a la región crítica...");
		fflush(stdout);
		entrar_rc();
		printf(" ¡Conseguido!\n");

		printf("Estoy DENTRO de la Región Crítica\n");
		printf("Presionar una tecla para Salir de la RC...\n");

		/* Sale de la región crítica */
		getchar(); /* lee del teclado */
		salir_rc();
	}

	finalizar_conexiones();

	return 0;
}

