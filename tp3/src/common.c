#include <stdio.h>
#include <stdlib.h>

#define TAM_LINEA 81
#define REQUEST    1
#define ENTRAR_RC  2
#define PRIVILEGIO 3
#define SALIR_RC   4

typedef char* string;

void fatal(string mensaje) {
	fprintf(stderr, "%s\n", mensaje);
	exit(1);
}

struct msg {
	int tipo;
};

