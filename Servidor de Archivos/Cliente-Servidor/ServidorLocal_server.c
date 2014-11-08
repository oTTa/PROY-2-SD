/*
 * This is sample code generated by rpcgen.
 * These are only templates and you can use them
 * as a guideline for developing your own functions.
 */

#include "ServidorLocal.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "../binder/binder.h"



//crea un archivo con el nombre *Argp, devueve 1 si pudo crearlo 0 en caso contrario
int *
creararchivo_1_svc(char **argp, struct svc_req *rqstp)
{
	static int  result;
	char * path;
	char * *tablaVersiones;
	char *update_1_arg;
	int *version;
	CLIENT *clnt;
	path = (char*)malloc (150);
	*path='\0';
	strcat (path,"../archivos/");
	strcat (path,*argp);
	strcat (path,"-0");
	strcat (path,".c");
	FILE *archivo;
	result=0;
	clnt = clnt_create ("localhost", binder, binderv1, "tcp");
	if (clnt == NULL) {
		clnt_pcreateerror ("localhost");
		exit (1);
	}
	tablaVersiones=update_1((void*)&update_1_arg, clnt);
	if (tablaVersiones == (char **) NULL) {
		clnt_perror (clnt, "call failed");
	}
	//Verifico que no este en los server el archivo
	if (strstr(*tablaVersiones,*argp)==NULL){
	      archivo = fopen(path,"w");
	      if (archivo){
		fclose(archivo);
		result=1;
		//se añade al server como primera version
		version = getversionaescribir_1(argp, clnt);
		if (version == (int *) NULL) 
			clnt_perror (clnt, "call failed");
	      }
	  }
	clnt_destroy (clnt);
	free(path);
	return &result;
}

/*modifica el archivo con nombre argp->nombre en una nueva version que obtiene del binder,
en caso de que no exista crea el archivo con version 0 ya que el binder le respondera 0
retorn 1 si pudo modificarlo 0 en caso contrario*/

int *
modificararchivo_1_svc(nombreContenido *argp, struct svc_req *rqstp)
{
	static int  result;
	int pos;
	char* call; 
	char* nombre;
	FILE *archivo;
	FILE *archivoNuevo;
	char caracter;
	char* str;
	char* path;
	int *version;
	CLIENT *clnt;
	path=(char*) malloc (230);
	nombre=(char*) malloc (200);
	str=(char*) malloc (15);
	*path='\0';
	*nombre='\0';
	*str='\0';
	result=1;
	
	clnt = clnt_create ("localhost", binder, binderv1, "tcp");
	if (clnt == NULL) {
		clnt_pcreateerror ("localhost");
		result=0;
		return &result;
	}
	
	version = getversionaescribir_1(&(argp->nombre), clnt);
	clnt_destroy (clnt);
	
	strcat(argp->nombre,"-");
        strcat (path,"../archivos/");
	strcat(path,argp->nombre);
	sprintf(str, "%d",*version);
	strcat(path,str);
	strcat(path,".c");
	
	archivoNuevo=fopen (path,"w");
	fprintf(archivoNuevo,argp->contenido);

	fclose (archivoNuevo);

	free (path);
	free (str);
	free (nombre);
	
	return &result;
}

long *
tamanoarchivo_1_svc(char **argp, struct svc_req *rqstp)
{
	static long  result;

	FILE* arch; 
	char path[200];
	strcat (path,"../archivos/");
	strcat (path,*argp);
	arch=fopen(path, "rb"); // abro el archivo de solo lectura.
	if (arch!=NULL){
	  fseek(arch,0, SEEK_END);            // me ubico en el final del archivo.
	  result=ftell(arch);                     // obtengo su tamanio en BYTES.
	  fclose(arch);                               // cierro el archivo.
	}
	else
	  result=-1;
	return &result;
}

char **
getarchivo_1_svc(nombreVersion *argp, struct svc_req *rqstp)
{
	static char * result;
	char* resu;
	char* path;
	int tamanio;
	char caracter;
	char* call; 
	int pos;
	char* nombre;
	FILE* arch; 
	char caracteres[100];
	char str[15];
	tamanio=0;
	pos=0;
	nombre=(char*) malloc (210);
	path=(char*) malloc (210);
	*path='\0';
	*nombre='\0';
	strcat (argp->nombre,"-");
	strcat(path,"../archivos/");
	if (argp->v !=-1){
	  sprintf(str, "%d", argp->v);
	  strcat(argp->nombre,str);
	  strcat(path,argp->nombre);
	  strcat(path,".c");
	}
	else {
	call=(char*) malloc (300);
	*call='\0';
	strcat(call,"ls ../archivos/ |grep ");
	strcat(call,argp->nombre);
	strcat(call," > versiones.txt");
	system (call);
	arch = fopen("versiones.txt","r");
	if (arch == NULL)
		printf("\nError de apertura del archivo  versiones.txt \n\n");
	else{
	      //busco la ultima version del archivo  
	    while (feof(arch) == 0)
	    {
	    	  caracter = fgetc(arch);
	          if (caracter=='\n')
			    pos=0;
		  else {
    		      if (!feof(arch)){
		          *(nombre+pos)=caracter;
			  pos++;
			 }
		       }
	    }
	    fclose (arch);
	}
	strcat(path,nombre);
	free (call);
	}
	
	
	arch=fopen(path, "rb"); // abro el archivo de solo lectura.
	if (arch!=NULL){
	    fseek(arch,0, SEEK_END);            // me ubico en el final del archivo.
	    tamanio=ftell(arch);                     // obtengo su tamanio en BYTES.
	    fclose(arch);                               // cierro el archivo.
	  }
	//reservamos espacio para mandar el archivo
	resu=malloc (sizeof(char)*tamanio);
	*resu='\0';
	arch = fopen(path,"r");
	if (arch!=NULL){
	  while (fgets(caracteres,100,arch) != NULL)
		  strcat (resu,caracteres);
	}
	
	result=resu;
	free (path);
	return &result;
}

char **
listararchivos_1_svc(void *argp, struct svc_req *rqstp)
{
	static char * result;
	FILE* arch; 
	char caracteres[100];
	char* resu; 
	int tamanio;
	system ("ls ../archivos > archivos.txt");
	
	arch=fopen("archivos.txt", "rb"); // abro el archivo de solo lectura.
	fseek(arch,0,SEEK_END);            // me ubico en el final del archivo.
	tamanio=ftell(arch);                     // obtengo su tamanio en BYTES.
	fclose(arch);                               // cierro el archivo.
	
	//reservamos espacio para mandar el archivo
	resu=malloc (sizeof(char)*tamanio+100);
	*resu='\0';
	arch = fopen("archivos.txt","r");
	while (fgets(caracteres,100,arch) != NULL)
 	{
 		strcat (resu,caracteres);
 	}
	result=resu;
	
	return &result;
}