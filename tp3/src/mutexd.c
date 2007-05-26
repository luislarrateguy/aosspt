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

/* Variables globales a las funciones */
int self, holder, cliente;
bool asked, using = FALSE;
Queue colaServers;
struct puertos servidores;

/* Estructura utilizada para retornar la configuración de puertos leída en la
 * función 'leer_puertos'. En la primer columna de la matriz está el puerto del
 * mutex y en la segunda el puerto del padre */
struct puertos {
	int puerto[CANT_MUTEXD][2];
};

/* Retorna una estructura 'puertos' con el numero de puerto de cada mutex y el
 * de su padre, leidos del archivo 'mutex.cfg' */ 
void leer_puertos (struct puertos *resultado) {
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

/* Es utilizado luego de leer el archivo de configuración. Retorna el holder
 * inicial el proceso mutexd actual. */
int obtener_holder() {
	/* Aca intento leer el puerto del holder */
	leer_puertos(&servidores);
	int i = 0;
	int h = -1;
	
	while((i<CANT_MUTEXD) && (servidores.puerto[i][0] != self)) {
		i++;
	}
	if (i == CANT_MUTEXD) {
		fatal("No se encontró el puerto actual en la lista\n");
	} else {
		if (servidores.puerto[i][1] == 0) 
			h = self;
		else 
			h = servidores.puerto[i][1];
	}
	return h;
}

/* Se encarga de llamar a la función genérica 'inicializar' con los argumentos
 * correctos para un proceso 'mutexd' */
void inicializar_servidor(int puerto) {
	skr = inicializar(&canal_recepcion, puerto, TRUE, FALSE);
	skw = inicializar(&canal_envio, 0, TRUE, TRUE);
	inicializada = TRUE;
}

/* Sólo actúa si el proceso actual posee ahora el token, si no lo está usando y
 * si la cola de pedidos (colaServers) no está vacía.
 * Báscicamente transfiere el token a otro servidor, o le da permiso de utilizar
 * la región crítica a su cliente si éste se lo ha pedido */
void assign_privilege() {
	if (!inicializada)
		fatal("assign_privilege: conexión no inicializada aún.");

	if(holder == self 
		&& !using 
		&& !IsEmpty(colaServers) ) {

		/* Creo el mensaje a enviar, común a todos los casos */
		struct msg mensaje;
		mensaje.tipo = PRIVILEGE;
		mensaje.from = self;
		
		/* Mi nuevo holder será el que esté en la cabeza de la cola,
		 * ya que en caso de ser alguien distinto a este proceso, le voy
		 * a enviar el token y ciertamente será mi nuevo holder.
		 * En caso de ser yo mismo, el control pasa sólo al cliente, y
		 * yo sigo teniendo el token (holder = self). */
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

/* Sólo actúa si el proceso actual no tiene ahora el token, si la cola no está
 * vacía y si no ha enviado un mensaje REQUEST antes (asked = FALSE).
 * Lo que hace 'make_request' es justamente pedir al holder actual el token */
void make_request() {
	if (!inicializada)
		fatal("make_request: conexión no inicializada aún.");

	if (holder != self
		&& !IsEmpty(colaServers)
		&& !asked) {
		
		debug("Haciendo un REQUEST por token");

		/* Armo el mensaje de pedido (REQUEST) */
		struct msg mensaje;
		mensaje.tipo = REQUEST;
		mensaje.from = self;

		/* Envío el mensaje a mi holder y seteo la variable asked a TRUE,
		 * para no volver a pedirle lo mismo */
		send_msg(mensaje,holder);
		asked = TRUE;
	}
}

/* Se ejecuta antes de comenzar el algoritmo de Raymond. Se encarga de verificar que
 * todos los demás servidores estén activos */
void saludo_inicial() {
	struct msg mensaje;
	int i, fromaux;

	/* Indica si todos los servidores están activos */
	bool todos_los_mutexd_activos = FALSE;

	/* Array de bools que indica qué servido está activo. Cuando
	 * todos los elementos del mismo son TRUE, entonces
	 * 'todos_los_mutexd_activos' también es TRUE */
	bool servidor_activo[CANT_MUTEXD];

	if (!inicializada)
		fatal("saludo_inicial: conexión no inicializada aún.");

	/* Envío un mensaje HELLO a todos los servidores */
	printf("Enviando mensaje HELLO a los demás servidores mutexd...");
	fflush(stdout);

	mensaje.tipo = HELLO;
	mensaje.from = self;

	/* Envío a cada servidor un mensaje HELLO. Es probable que alguno de ellos
	 * no se haya ejecutado aún, en ese caso no recibirá el mensaje obviamente.
	 * En este bucle también se inicializa el array 'servidor_activo' a FALSE */
	for (i=0; i < CANT_MUTEXD; i++) {
		if (servidores.puerto[i][0] == self) {
			servidor_activo[servidores.puerto[i][0] - 5001] = TRUE;
			continue;
		}

		servidor_activo[servidores.puerto[i][0] - 5001] = FALSE;
		send_msg(mensaje, servidores.puerto[i][0]);
	}

	printf(" ¡Listo!\n");
	printf("Esperando respuesta de todos ellos antes de continuar...\n");
	fflush(stdout);

	/* Ahora espero las respuestas de los otros servidores */
	while (!todos_los_mutexd_activos) {
		receive_msg(&mensaje);

		/* Si el mensaje es del tipo HELLO y no lo tenía como activo al servidor
		 * emisor del mismo, entonces lo seteo como activo y le envío una
		 * respuesta */
		if (mensaje.tipo == HELLO && !servidor_activo[mensaje.from - 5001]) {
			fromaux = mensaje.from;

			servidor_activo[fromaux - 5001] = TRUE;
			
			mensaje.tipo = HELLO;
			mensaje.from = self;
			send_msg(mensaje, fromaux);

			printf("   %d -> ¡activo!\n", fromaux);
		}

		/* Si se da la casualidad de que se ha recibido un mensaje distinto a HELLO
		 * en la fase de inicialización, entonces algo anda mal */
		else if (mensaje.tipo != HELLO) {
			debug("ATENCION: Se recibió un mensaje distinto de HELLO\n");
		}

		/* El siguiente código simplemente verifica la condición para que
		 * 'todos_los_mutexd_activos' sea TRUE y así salir del bucle y
		 * finalizar la fase de saludo o inicialización */
		for (i=0; i < CANT_MUTEXD; i++) {
			if (servidor_activo[i] == FALSE)
				break;
		}

		if (i == CANT_MUTEXD)
			todos_los_mutexd_activos = TRUE;
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
	holder = obtener_holder();
	
	printf("Mi puerto: %d\n",self);
	printf("El holder: %d\n",holder);
	
	/* Inicializo las estructuras para la comunicación */
	inicializar_servidor(puerto);

	printf("Conexiones inicializadas\n");

	/* Hago el saludo inicial antes de comenzar con el algoritmo */
	saludo_inicial();

	/* En este punto todos los servidores están activos */
	printf("Todos los servidores están activos.");
	printf("Comienza a operar el algoritmo.\n\n");

	/* Comienza el algoritmo */
	while (TRUE) {
		receive_msg(&mensaje);
		
		printf("------\n");
		printf("Recibido un mensaje de %s:%d\n",
			inet_ntoa(canal_recepcion.sin_addr), mensaje.from);
		printf("Tipo del mensaje recibido: %s\n\n", nombre_mensajes[mensaje.tipo]);

		if (mensaje.tipo == ENTRAR_RC) {
			debug("Pide entrar en la region crítica mi cliente");
			cliente = mensaje.from;
			Enqueue(self,colaServers);
		}
		else if (mensaje.tipo == REQUEST) {
			if (!using)
				debug("Pide un vecino el TOKEN");
			else
				debug("Pide un vecino el TOKEN, pero lo estoy usando.");

			Enqueue(mensaje.from,colaServers);
		}
		else if (mensaje.tipo == PRIVILEGE) {
			debug("Me ha llegado un TOKEN");
			holder = self;
		}
		else if (mensaje.tipo == SALIR_RC) {
	   		debug("El cliente libero la region crítica");
			using = FALSE;
			cliente = -1;
		}
		else if (mensaje.tipo == HELLO) {
			debug("Mensaje HELLO ignorado");
		}

		assign_privilege();
		make_request();

		printf("------\n\n");
	}

	DisposeQueue(colaServers);
	/* TODO: cerrar los sockets skw skr */
}

