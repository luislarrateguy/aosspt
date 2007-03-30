#include <stdio.h>
#include <stdlib.h>

#define PUERTO_SERVIDOR 2222
#define TAM_BUFFER 256
#define TAM_COLA 10
#define RUTA_ARCHIVO_GUIA "../etc/guia"
#define TAM_LINEA 81

typedef char* string;

const string TEL_NO_ENCONTRADO 	= "99999999999;ERROR";
const string FIN_DATOS 			= "00000000000;FIN";

void fatal(string mensaje) {
	fprintf(stderr, "%s\n", mensaje);
	exit(1);
}
