/*
 * This is sample code generated by rpcgen.
 * These are only templates and you can use them
 * as a guideline for developing your own functions.
 */
#include "ej2.h"
#include <string.h>
void MostrarDialogo();

void
ej2_1(char *host)
{
	CLIENT *clnt;
	char opcion;
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
	char* call;
	char caracteres[100];
	FILE* arch; 
#ifndef	DEBUGresult_1
	clnt = clnt_create (host, ej2, ej2v1, "tcp");
	if (clnt == NULL) {
		clnt_pcreateerror (host);
		exit (1);
	}
#endif	/* DEBUG */
	creararchivo_1_arg=(char*)malloc (sizeof(char)*100);
	while (opcion!='5'){
	    MostrarDialogo();
	    fflush(stdin);
	    scanf ("%c",&opcion);
	    fflush(stdin);
	    switch (opcion){
	      case ('1'):
		printf ("Ingrese el nombre del archivo a crear:");
		scanf ("%s",creararchivo_1_arg);
		result_1 = creararchivo_1(&creararchivo_1_arg, clnt);
		if (result_1 == (int *) NULL) {
			clnt_perror (clnt, "call failed");
		}
		if (*result_1==1)
		  printf("archivo creado correctamente\n");
		else
		  printf("El archivo no pudo ser creado ya que existe un archivo con ese nombre\n");
		break;
	      case ('2'):
		printf ("Ingrese el nombre del archivo a modificar:");
		scanf ("%s",&modificararchivo_1_arg.nombre);
		strcat (call,"rm ");
		strcat (call,modificararchivo_1_arg.nombre);
		system (call);
		call="";
		strcat (call,"vi ");
		strcat (call,modificararchivo_1_arg.nombre);
		system (call);
		call="";
		arch = fopen(modificararchivo_1_arg.nombre,"r");
		if (arch!=NULL){
		  while (feof(arch) == 0)
		  {
			  fgets(caracteres,100,arch);
			  strcat (modificararchivo_1_arg.contenido,caracteres);
		  }
		  fclose (arch);
		  result_2 = modificararchivo_1(&modificararchivo_1_arg, clnt);
		  if (result_2 == (int *) NULL) {
			  clnt_perror (clnt, "call failed");
		  }
		}
		break;
	      case ('3'):
	      /* result_3 = tamanoarchivo_1(&tamanoarchivo_1_arg, clnt);
		if (result_3 == (long *) NULL) {
			clnt_perror (clnt, "call failed");
		}*/
	        printf ("Ingrese el nombre del archivo a leer:");
		scanf ("%s",getarchivo_1_arg.nombre);
		printf ("Ingrese la version del archivo a leer:");
		scanf ("%i",getarchivo_1_arg.v);
		result_4 = getarchivo_1(&getarchivo_1_arg, clnt);
		if (result_4 == (char **) NULL) {
			clnt_perror (clnt, "call failed");
		}
		printf ("%s\n",*result_4);
	        break;
	      case ('4'):
		result_5 = listararchivos_1((void*)&listararchivos_1_arg, clnt);
		if (result_5 == (char **) NULL) {
			clnt_perror (clnt, "call failed");
		}
		printf ("%s\n",*result_5);
	    }
	}
	printf ("CHAU \n");
#ifndef	DEBUG
	clnt_destroy (clnt);
#endif	 /* DEBUG */
}

void MostrarDialogo(){
  printf ("Opciones\n");
  printf("1 - Crear archivo \n 2 - Modificar archivo \n  3 - Mostrar archivo \n 4 - Listar archivos \n 5 - Salir\n" );
  printf ("INGRESE UNA OPCION:");
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
