#include <stdio.h>
#include <stdlib.h>

#define PUERTO_SERVIDOR 2222
#define TAM_BUFFER 256
#define TAM_COLA 10
#define RUTA_ARCHIVO_GUIA "../etc/guia"
#define TAM_LINEA 80

typedef char* string;

void fatal(string mensaje) {
	fprintf(stderr, "%s\n", mensaje);
	exit(1);
}

