#include <stdio.h>
#include <stdlib.h>

#define PUERTO_SERVIDOR 2222
#define TAM_BUFFER 256
#define TAM_COLA 10

void fatal(char* mensaje) {
	fprintf(stderr, "%s\n", mensaje);
	exit(1);
}

