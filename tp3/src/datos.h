#include <stdio.h>
#include <stdlib.h>

#define TAM_LINEA 81
#define RUTA_MUTEX_CNF "../etc/mutex.cfg"
#define CANT_MUTEXD 7

/* Estructura utilizada para retornar la configuración
 * de puertos leída en la función 'leerPuertos'. En la
 * primer columna de la matriz está el puerto del mutex
 * y en la segunda el puerto del padre */
struct puertos {
	int puerto[CANT_MUTEXD][2];
};

typedef char* string;

void fatal(string mensaje) {
	fprintf(stderr, "%s\n", mensaje);
	exit(1);
}

