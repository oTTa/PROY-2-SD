/*
 * This is sample code generated by rpcgen.
 * These are only templates and you can use them
 * as a guideline for developing your own functions.
 */

#include "ServidorLocal.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
//crea un archivo con el nombre *Argp
int *
creararchivo_1_svc(char **argp, struct svc_req *rqstp)
{
	static int  result;
	char * path;
	path = (char*)malloc (150);
	strcat (path,"../archivos/");
	strcat (path,*argp);
	strcat (path,"-0");
	FILE *archivo;
	result=0;
	//abre un archivo para lectura
	archivo = fopen(path,"r");
	//verifica si el archivo abierto no existe
	if (archivo==NULL){
	  archivo = fopen(path,"w");
	    //verifica si se creo el archivo
	    if (archivo!=NULL)
	      result=1;
	}
	else
	  fclose(archivo);
	return &result;
}

/*modifica el archivo con nombre argp->nombre en una nueva version que obtiene del binder,
en caso de que no exista crea el archivo con version 0 ya que el binder le respondera 0*/
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
	char* path1;
	path=(char*) malloc (210);
	path1=(char*) malloc (210);
	call=(char*) malloc (300);
	nombre=(char*) malloc (200);
	str=(char*) malloc (15);
	*path='\0';
	*path1='\0';
	*call='\0';
	*nombre='\0';
	*str='\0';
	
	strcat(argp->nombre,"-");
	strcat(call,"ls ../archivos/ |grep ");
	strcat(call,argp->nombre);
	strcat(call," > versiones.txt");
	
	result=0;
	pos=0;
	//listo las versiones del archivo en versiones.txt
	system (call);
	archivo = fopen("versiones.txt","r");
	if (archivo == NULL)
		printf("\nError de apertura del archivo  versiones.txt \n\n");
    else{
	      //busco la ultima version del archivo  
	    while (feof(archivo) == 0)
	    {
	    	  caracter = fgetc(archivo);
	          if (caracter=='\n')
			    pos=0;
		  else {
    		      if (!feof(archivo)){
		          *(nombre+pos)=caracter;
			  pos++;
			 }
		       }
	    }
	    fclose(archivo);
	    pos=strlen(nombre);
	    //path para verificar la existe del archivo
	    strcat (path,"../archivos/");
	    strcat(path,nombre);
	    //archivo a crear
	    strcat (path1,"../archivos/");
	    strcat(path1,argp->nombre);
	    int  version=0;
	    archivo=fopen (path,"r");
	    // el archivo que se va a modificar existe
	    if (archivo!=NULL && (*nombre)!='\0'){
	      fclose (archivo);
	      while (*(nombre+pos-1)!='-' && pos>=0){
		    version=version*10+(int)(*(nombre+pos-1)-'0');
		    pos--;
	      }
	      version++;
	      sprintf(str, "%d",version);
	      strcat(path1,str);
	      archivoNuevo=fopen (path1,"w");
	      fprintf(archivoNuevo,argp->contenido);
	    }
	    //el archivo a modicar no existe y por lo tanto se creara una nueva version
	    else{
	      strcat(path1,"0");
	      archivoNuevo=fopen (path1,"w");
	      fprintf(archivoNuevo,argp->contenido);
	    }
	    fclose (archivoNuevo);
	}
	free (path);
	free (path1);
	free (call);
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