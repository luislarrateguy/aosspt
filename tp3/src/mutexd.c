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

#include "queue/queue.h"
#include "common.h"
#include <stdio.h>

#define RUTA_MUTEX_CFG "../etc/mutex.cfg"
#define CANT_MUTEXD 7

/* Variables Globales a las funciones */
int self,holder,cliente;
bool asked, using = FALSE;
Queue colaServers;

/* Estructura utilizada para retornar la configuración
 * de puertos leída en la función 'leerPuertos'. En la
 * primer columna de la matriz está el puerto del mutex
 * y en la segunda el puerto del padre */
struct puertos {
	int puerto[CANT_MUTEXD][2];
};

/* Retorna una estructura 'puertos' con el numero de puerto
 * de cada mutex y el de su padre, leidos del archivo 'mutex.cfg' */ 
struct puertos leerPuertos (struct puertos *resultado) {
	FILE* archivo;
	char linea[TAM_LINEA];
	char* puerto;
	char* puertoPadre;

	int i = 0;

	/* Abrimos el archivo */
	if ((archivo = fopen(RUTA_MUTEX_CFG, "r")) == NULL) {
		char mensaje_error[60];
		mensaje_error[0] = '\0';

		strcat(mensaje_error, "No se pudo abrir el archivo ");
		strcat(mensaje_error, RUTA_MUTEX_CFG);
		strcat(mensaje_error, "\n");

		fatal(mensaje_error);
	}
	
	while (!feof(archivo) && (i < CANT_MUTEXD))  {
		/* Leemos la línea */
		fgets(linea, TAM_LINEA, archivo);

		/* Quito el fin de linea ('\n') de la línea leída */
		linea[strlen(linea) - 1] = '\0';

		puerto = strtok(linea, " ");
		puertoPadre = strtok(NULL, "\0");
		
		if (puerto != NULL && puertoPadre != NULL) {
			resultado->puerto[i][0] = atoi(puerto);
			resultado->puerto[i][1] = atoi(puertoPadre);
		}

		i++;
	}
	
	/* Cerramos el archivo */
	if (fclose(archivo) == EOF)
		fatal("Error al cerrar el archivo\n");
}

int obtenerHolder() {
	/* Aca intento leer el puerto del holder */
	struct puertos lista;
	leerPuertos(&lista);
	int i = 0;
	int h = -1;
	
	while((i<CANT_MUTEXD) && (lista.puerto[i][0] != self)) {
		i++;
	}
	if (i == CANT_MUTEXD) {
		fatal("No se encontró el puerto actual en la lista\n");
	} else {
		if (lista.puerto[i][1] == 0) 
			h = self;
		else 
			h = lista.puerto[i][1];
	}
	return h;
}
void inicializar_servidor(int puerto) {
	skr = inicializar(&canal_recepcion, puerto, TRUE, FALSE);
	skw = inicializar(&canal_envio, 0, TRUE, TRUE);
	inicializada = TRUE;
}
void assignPrivilege() {
	if(holder == self 
		&& !using 
		&& !IsEmpty(colaServers) ) {

		struct msg mensaje;
		mensaje.tipo = PRIVILEGE;
		mensaje.from = self;
		
		holder = FrontAndDequeue(colaServers);
		asked = FALSE;

		if (holder == self) {
			debug("Tengo el poder. Ahora se lo mando al cliente");
			using = TRUE;
			send_msg(mensaje, cliente);
		} else {
			debug("Tengo el poder. Pero es para otro. Se lo mando.");
			send_msg(mensaje, holder);
		}
	}
}

void makeRequest() {
	if (holder != self
		&& !IsEmpty(colaServers)
		&& !asked) {
		
		debug("Haciendo un REQUEST por token");
		struct msg mensaje;
		mensaje.tipo = REQUEST;
		mensaje.from = self;
		send_msg(mensaje,holder);
		asked = TRUE;
	}
}

int main(int argc, char* argv[]) {
	int puerto;
	colaServers = CreateQueue(7);
	struct msg mensaje;

	if (argc < 2)
		fatal("Uso: mutexd <puerto> [0]\n  Usando [0] desactiva el debugging");

	inicializada = FALSE;

	puerto = atoi(argv[1]);
	/* Hacer volar si no vamos a usar el debugging */
	if ((argc == 3) && (atoi(argv[2]) == 0))
		debugging = FALSE;
	else 
		debugging = TRUE;

	/* self no debería ser modificado nunca. Indica el puerto del servidor
	 * actual */
	self = puerto;
	holder = obtenerHolder();
	
	fprintf(stderr,"Mi puerto: %d\n",self);
	fprintf(stderr,"El holder: %d\n",holder);
	
	/* Inicializo las estructuras para la comunicación */
	inicializar_servidor(puerto);
	
	fprintf(stderr,"Conexiones inicializadas\n");

	while (TRUE) {
		receive_msg(&mensaje);
		
		printf("Recibido un mensaje de %s\n", inet_ntoa(canal_recepcion.sin_addr));
		printf("Tipo del mensaje recibido: %d\n\n", mensaje.tipo);

		/* TODO: Espera de todos los servidores UP.
		 * Posible implementacion: a las espera de 7 HELLO. Un simple contador 
		 */
			
		if (mensaje.tipo == ENTRAR_RC) {
			debug("Pide entrar en la region crítica mi cliente");
			cliente = mensaje.from;
			Enqueue(self,colaServers);
		}
		if (mensaje.tipo == REQUEST) {
			debug("Pide un vecino el TOKEN");
			Enqueue(mensaje.from,colaServers);
		}
		if (mensaje.tipo == PRIVILEGE) {
			debug("Me ha llegado un TOKEN");
			holder = self;
		}
		if (mensaje.tipo == SALIR_RC) {
	   		debug("El cliente libero la region crítica");
			using = FALSE;
			cliente = -1;
		}
		assignPrivilege();
		makeRequest();
	}
	DisposeQueue(colaServers);
	/* TODO: cerrar los sockets skw skr */
}

