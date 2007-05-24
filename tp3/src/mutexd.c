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

#include <string.h>
#include <sys/types.h>
#include <sys/fcntl.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>
#include <string.h>

#include "queue/queue.h"
#include "common.c"

#define RUTA_MUTEX_CNF "../etc/mutex.cfg"
#define CANT_MUTEXD 7

/***** Variables Globales a las funciones *******/
/* si les parece metanlas en otro lado, common.c por ejemplo */

int self,holder;
bool asked;
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
		/* Leemos la linea */
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
	if( holder == self 
		&& !using 
		&& !IsEmpty(colaServers) ) {

		holder = FrontAndDequeue(colaServers);
		asked = false;
		if (holder = self) {
			using = true;
			/* enviar mensaje a cliente para que sepa
			 * que esta dentro de la región cr�tica */
		} else {
			struct msg mensaje;
			mensaje.tipo = PRIVILEGE;
			send(mensaje,holder);
		}
	}
}

void makeRequest() {
	if (holder != self 
		&& !IsEmtpy(colaServers) 
		&& !asked) {
		struct msg mensaje;
		mensaje.tipo = REQUEST;
		send(mensaje,holder);OA
		asked = true;
	}
}

int main(int argc, char* argv[]) {
    /* NACHO: que son s, b y l? Ver si no son holder y self definidas por mi
	 * afuera, arriba de todo. */
	int s, b, l, puerto;

	struct sockaddr_in canal;
	struct msg mensaje;
	socklen_t fromlen;

	if (argc != 2)
		fatal("Uso: mutexd <puerto>");

	puerto = atoi(argv[1]);

	/* TODO: Se busca el puerto en el archivo de configuración, y
	 * se halla el puerto del holder. */

	/* NACHO: Todo esto de conexion lo pondria en una funcion aparte
	 * por como me gusta programar. No lo hago porque séque lo van
	 * a poner de nuevo como está si lo cambio jeje
	 * */
	s = socket(PF_INET, SOCK_DGRAM, IPPROTO_UDP);
	if (s < 0) fatal("No se pudo crear el socket.");

	memset(&canal, 0, sizeof(canal));
	canal.sin_family = AF_INET;
	canal.sin_addr.s_addr = htonl(INADDR_ANY);
	canal.sin_port = htons(puerto);

	b = bind(s, (struct sockaddr*)&canal, sizeof(canal));
	if (b < 0) fatal("Error al ejecutar bind.");

	while (1) {
		fromlen = sizeof(canal);
		
		if (recvfrom(s, (void *)&mensaje, sizeof(mensaje), 0,
			(struct sockaddr*)&canal, &fromlen) < 0)
			fatal("Error en recvfrom.");

		printf("Recibido un mensaje de %s\n", inet_ntoa(canal.sin_addr));
		printf("Tipo del mensaje recibido: %d\n\n", mensaje.tipo);

		/* TODO: Gestión de la región crítica  e implementación del
 		 * algoritmo de Raymond. 
		 * Comenzado!*/
		
		/* Comienza el baile */
		
		if (mensaje.tipo == ENTRAR_RC) {
			Enqueue(self,colaServers);
		}
		if (mensaje.tipo == REQUEST) {
			Enqueue(elPuertoDelQuePideRelativo,colaServers);
		}
		if (mensaje.tipo == PRIVILEGE) {
			holder = self;
		}
		if (mensaje.tipo == SALIR_RC) {
			using = false;
		}
		assignPrivilege();
		makeRequest();
	}
}

