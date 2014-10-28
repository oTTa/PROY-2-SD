/*
 * This is sample code generated by rpcgen.
 * These are only templates and you can use them
 * as a guideline for developing your own functions.
 */
#include "ej2.h"


void
ej2_1(char *host)
{
	CLIENT *clnt;
	int  *result_1;
	char * creararchivo_1_arg;
	int  *result_2;
	nombreContenido  modificararchivo_1_arg;
	long  *result_3;
	char * tamanoarchivo_1_arg;
	char * *result_4;
	nombreVersion  getarchivo_1_arg;
	char * *result_5;
	char *listararchivos_1_arg;
	int opcion=0;
	
#ifndef	DEBUG
	clnt = clnt_create (host, ej2, ej2v1, "udp");
	if (clnt == NULL) {
		clnt_pcreateerror (host);
		exit (1);
	}
#endif	/* DEBUG */
	while (opcion!=6)
	result_1 = creararchivo_1(&creararchivo_1_arg, clnt);
	if (result_1 == (int *) NULL) {
		clnt_perror (clnt, "call failed");
	}
	result_2 = modificararchivo_1(&modificararchivo_1_arg, clnt);
	if (result_2 == (int *) NULL) {
		clnt_perror (clnt, "call failed");
	}
	result_3 = tamanoarchivo_1(&tamanoarchivo_1_arg, clnt);
	if (result_3 == (long *) NULL) {
		clnt_perror (clnt, "call failed");
	}
	result_4 = getarchivo_1(&getarchivo_1_arg, clnt);
	if (result_4 == (char **) NULL) {
		clnt_perror (clnt, "call failed");
	}
	result_5 = listararchivos_1((void*)&listararchivos_1_arg, clnt);
	if (result_5 == (char **) NULL) {
		clnt_perror (clnt, "call failed");
	}
#ifndef	DEBUG
	clnt_destroy (clnt);
#endif	 /* DEBUG */
}


int
main (int argc, char *argv[])
{
	char *host;

	if (argc < 2) {
		printf ("usage: %s server_host\n", argv[0]);
		exit (1);
	}
	host = argv[1];
	ej2_1 (host);
exit (0);
}
