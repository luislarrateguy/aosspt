/*
 * This is sample code generated by rpcgen.
 * These are only templates and you can use them
 * as a guideline for developing your own functions.
 */

#include "sinc.h"
#include "time.h"
#include "stdio.h"

time_t *
horaactual_1_svc(void *argp, struct svc_req *rqstp)
{
	static time_t  result;
	printf("El server recibió un pedido\n");
	
	time (&result);
	printf("El resultado devuelto es %ld\n", result);	

	return &result;
}
