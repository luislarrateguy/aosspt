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

#define RUTA_MUTEX_CNF "../etc/mutex.cfg"
#define CANT_MUTEXD 7

/***** Variables Globales a las funciones *******/
/* nacho: si les parece metanlas en otro lado, common.c por ejemplo
 * milton: para mi esta bien que estén acá, porque son propias del mutexd */

int self,holder,cliente;
bool asked, using;
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
struct puertos leerPuertos () {
	FILE* archivo;
	char linea[TAM_LINEA];
	char* puerto;
	char* puertoPadre;
	struct puertos resultado;
	int i = 0;

	/* Abrimos el archivo */
	if ((archivo = fopen(RUTA_MUTEX_CNF, "r")) == NULL)
		fatal("No se pudo abrir el archivo /etc/mutexd.conf\n");
	
	while (!feof(archivo) && (i < CANT_MUTEXD))  {
		/* Leemos la línea */
		fgets(linea, TAM_LINEA, archivo);

		/* Quito el fin de linea ('\n') de la línea leída */
		linea[strlen(linea) - 1] = '\0';

		puerto = strtok(linea, " ");
		puertoPadre = strtok(NULL, "\0");
		
		if (puerto != NULL && puertoPadre != NULL) {
			resultado.puerto[i][0] = atoi(puerto);
			resultado.puerto[i][1] = atoi(puertoPadre);
		}

		i++;
	}
	
	/* Cerramos el archivo */
	if (fclose(archivo) == EOF)
		fatal("Error al cerrar el archivo\n");
	
	return resultado;
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
			using = TRUE;
			send_msg(mensaje, cliente);
		} else {
			send_msg(mensaje, holder);
		}
	}
}

void makeRequest() {
	if (holder != self
		&& !IsEmpty(colaServers)
		&& !asked) {
		struct msg mensaje;
		mensaje.tipo = REQUEST;
		mensaje.from = self;
		send_msg(mensaje,holder);
		asked = TRUE;
	}
}

int main(int argc, char* argv[]) {
	int puerto;

	struct msg mensaje;

	if (argc != 2)
		fatal("Uso: mutexd <puerto>");

	inicializada = FALSE;

	puerto = atoi(argv[1]);

	/* TODO: Se busca el puerto en el archivo de configuración, y
	 * se halla el puerto del holder. */
	/* self no debería ser modificado nunca. Indica el puerto del servidor
	 * actual */
	self = puerto;
	
	/* Inicializo las estructuras para la comunicación */
	inicializar(&canal_recepcion,puerto,TRUE);
	inicializar(&canal_envio,0,TRUE);


	while (1) {

		receive_msg(&mensaje);
		
		printf("Recibido un mensaje de %s\n", inet_ntoa(canal_recepcion.sin_addr));
		printf("Tipo del mensaje recibido: %d\n\n", mensaje.tipo);

		/* TODO: Gestión de la región crótica  e implementación del
 		 * algoritmo de Raymond. 
		 * Comenzado! */
			
		if (mensaje.tipo == ENTRAR_RC) {
			cliente = mensaje.from;
			Enqueue(self,colaServers);
		}
		if (mensaje.tipo == REQUEST) {
			Enqueue(mensaje.from,colaServers);
		}
		if (mensaje.tipo == PRIVILEGE) {
			holder = self;
		}
		if (mensaje.tipo == SALIR_RC) {
			using = FALSE;
			cliente = -1;
		}
		assignPrivilege();
		makeRequest();
	}
}

