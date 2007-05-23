#include <stdio.h>
#include <stdlib.h>

#define TAM_LINEA 81

typedef char* string;

void fatal(string mensaje) {
	fprintf(stderr, "%s\n", mensaje);
	exit(1);
}

struct msg {
	int tipo;
};

