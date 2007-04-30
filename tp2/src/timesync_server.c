#include "timesync.h"
#include <time.h>

long *
timesync_1_svc(void *argp, struct svc_req *rqstp)
{
	static long result;
	static int cant_peticiones = 0;
	char hora_formateada[10];

	result = time(NULL);
	strftime(hora_formateada, 10, "%H:%M:%S" ,  (struct tm*)localtime(&result) );

	printf("%d - Petición recibida. Se devuelve %s\n",
		cant_peticiones, hora_formateada);
	cant_peticiones++;

	return &result;
}

