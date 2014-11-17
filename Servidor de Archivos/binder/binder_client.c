#include "binder.h"
#include <unistd.h>
#include <netdb.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include  <stdio.h>

char *ip_local() {
  struct sockaddr_in host;
  char nombre[255];
  char *ip;
  gethostname(nombre, 255);
  host.sin_addr = * (struct in_addr*) gethostbyname(nombre)->h_addr;
  ip = inet_ntoa(host.sin_addr);
  return ip;
}

void
binder_1(char *host)
{
	CLIENT *clnt;
	int  *result_1;
	char * registrarse_1_arg;
	char * *result_2;
	char *getipregistradas_1_arg;
	int  *result_3;
	char * eliminarip_1_arg;
	char * *result_4;
	char *update_1_arg;
	int  *result_5;
	char * getversionaescribir_1_arg;
	char opcion;

#ifndef	DEBUG
	clnt = clnt_create (host, binder, binderv1, "tcp");
	if (clnt == NULL) {
		clnt_pcreateerror (host);
		exit (1);
	}
#endif	/* DEBUG */

	
	opcion=0;
	eliminarip_1_arg=malloc (16);
	*eliminarip_1_arg='\0';
	getversionaescribir_1_arg= malloc (100);
	*getversionaescribir_1_arg='\0';
	while (opcion!='6'){
	    printf("\n1-Registrarse 2-Mostrar IP 3-Eliminar IP 4-Ver tabla 5-Archivo a ver la version a crear 6-Salir\n\n");
	    fflush(stdin);
	    printf("Ingrese una opcion:");
	    scanf (" %c",&opcion);
	    fflush(stdin);
	    system("clear");
	    //printf("\n\n");
	    switch (opcion){
	      case ('1'):
			registrarse_1_arg=ip_local();
			result_1 = registrarse_1(&registrarse_1_arg, clnt);
			if (result_1 == (int *) NULL) {
				clnt_perror (clnt, "call failed");
			}
			if (*result_1==0)
			  printf ("Ya estas registrado, no puedes volver a registrarte\n");
			else
			  if (*result_1==1)
			    printf("Registrado con exito.\n");
			  else
			    printf("binder lleno, debes esperar a que alguien salga.\n");
			break;
	      case ('2'):
			result_2 = getipregistradas_1((void*)&getipregistradas_1_arg, clnt);
			if (result_2 == (char **) NULL) {
			    clnt_perror (clnt, "call failed");
			}
			printf("\nIPs:\n%s\n",*result_2);
			break;
	      case('3'):
			printf("ingrese la ip a eliminar:");
			scanf("%s",eliminarip_1_arg);
			result_3 = eliminarip_1(&eliminarip_1_arg, clnt);
			if (result_3 == (int *) NULL) {
				clnt_perror (clnt, "call failed");
			}
			if (*result_3==0)
			  printf ("No existe la ip a eliminar\n");
			else
			  printf("IP eliminada correctamente\n");
			*eliminarip_1_arg='\0';
			break;
	      case('4'):
			result_4 = update_1((void*)&update_1_arg, clnt);
			if (result_4 == (char **) NULL) {
				clnt_perror (clnt, "call failed");
			}
			printf("Tabla de archivo-version:\n\n");
			printf ("%s\n",*result_4);
			break;
	      case('5'):
			printf("ingrese el nombre del archivo para ver la version que deberias crear:");
			scanf("%s",getversionaescribir_1_arg);
			result_5 = getversionaescribir_1(&getversionaescribir_1_arg, clnt);
			if (result_5 == (int *) NULL) {
				clnt_perror (clnt, "call failed");
			}
			printf ("La version que deberias crear es %i\n",*result_5);
			break;
	    }
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
	binder_1 (host);
exit (0);
}