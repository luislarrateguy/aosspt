#include <stdio.h>
#include "timesync.h"
#include <time.h>

long* timesync(char *host)
{
	CLIENT *clnt;
	long  *result_1;
	char *timesync_1_arg;

	clnt = clnt_create (host, TIMESYNCPROG, TIMESYNCVERS, "udp");
	if (clnt == NULL) {
		clnt_pcreateerror (host);
		exit (1);
	}

	result_1 = timesync_1((void*)&timesync_1_arg, clnt);
	if (result_1 == (long *) NULL) {
		clnt_perror (clnt, "Falló la llamada RPC");
	}

	clnt_destroy (clnt);

	return result_1;
}

int main (int argc, char *argv[])
{
	char *host;
	char *timeStr = (char *) malloc(10);
	struct timeval setHora;
	long *res;
	size_t sres;
	time_t actual;

	if (argc < 2) {
		printf ("uso: %s server_host\n", argv[0]);
		exit (1);
	}
	host = argv[1];
	
	actual = time(NULL);
	sres = strftime( timeStr, 10, "%H:%M:%S" ,  localtime(&actual) );
	printf ("Hora actual: %s\n", timeStr);
	
	printf ("Consultando el servidor servidor [%s]... \n", host);
	res = timesync (host);
	
	sres = strftime( timeStr, 10, "%H:%M:%S" ,  (struct tm*)localtime((time_t*)res) );
	printf ("Seteando la hora: %s ...\n", timeStr);
	
	setHora.tv_sec = *res ;
	setHora.tv_usec = 0;

	if (settimeofday(&setHora,NULL) == -1 ) {
		printf("Error al setear la hora. Debe ser administrador (root).\n");
		exit(1);
	}

	actual = time(NULL);
	sres = strftime( timeStr, 10, "%H:%M:%S" ,  (struct tm*)localtime(&actual) );
	printf ("Hecho. La hora actual es: %s ...\n", timeStr);
	free(timeStr);

	exit(0);
}

