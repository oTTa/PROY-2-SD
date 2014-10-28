/*
 * This is sample code generated by rpcgen.
 * These are only templates and you can use them
 * as a guideline for developing your own functions.
 */

#include "ej2.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

//crea un archivo con el nombre *Argp
int *
creararchivo_1_svc(char **argp, struct svc_req *rqstp)
{
	static int  result;
	char * path;
	strcat (path,"./archivos/");
	strcat (path,*argp);
	FILE *archivo;
	result=0;
	//abre un archivo para lectura
	archivo = fopen(path,"r");
	//verifica si el archivo abierto no existe
	if (archivo==null){
	  fclose(archivo);
	  archivo = fopen(path,"w");
	    //verifica si se creo el archivo
	    if (archivo!=null)
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
	char call[100] = "ls ./archivos |grep "; 
	char nombre [100];
	FILE *archivo;
	FILE *archivoNuevo;
	char caracter;
	
	strcat(argp->nombre,"-");
	strcat(call,argp->nombre);
	strcat(call," > versiones.txt");
	
	result=0;
	pos=0;
	//listo las versiones del archivo en versiones.txt
	system (call);
 
	archivo = fopen("versiones.txt","r");
 
	if (archivo == NULL){
		printf("\nError de apertura del archivo del archivo versiones.txt \n\n");
            else{
	      //busco la ultima version del archivo  
	      while (feof(archivo) == 0)
	      {
		  caracter = fgetc(archivo);
		  if (caracter=='\n')
			pos=0;
		  if (!feof(archivo))
		    nombre[pos]==caracter;
		  else
		    pos++;
	      }
	    }
        }
        
        fclose(archivo);
	
	char* path,path1;
	//path para verificar la existe del archivo
	strcat (path,"./archivos/");
	strcat(path,nombre);
	//archivo a crear
	strcat (path1,"./archivos/");
	strcat(path1,argp->nombre);
	strcat(path1,"-");
	int  version=0;
	archivo=fopen (path,"r");
	
	// el archivo que se va a modificar existe
	if (archivo!=NULL){
	  fclose (archivo);
	  while (*(nombre+pos)!='-'))
		version=version*10+*(nombre+pos)-((int)'0');
	  version++;
	  strcat(path1,(char)(((int)'0')+version));
	  archivoNuevo=fopen (path1,"w");
	  fprintf(fp,argp->contenido);
	}
	//el archivo a modicar no existe y por lo tanto se creara una nueva version
	else{
	  strcat(path1,'0');
	  archivoNuevo=fopen (path1,"w");
	  fprintf(fp,argp->contenido);
	}
	close (archivoNuevo);
	
	return &result;
}

long *
tamanoarchivo_1_svc(char **argp, struct svc_req *rqstp)
{
	static long  result;

	FILE* arch; 
	char *path;
	strcat (path,"./archivos/")
	strcat (path,*argp);
	arch=fopen(path, "rb"); // abro el archivo de solo lectura.
	if (arch!=NULL){
	  fseek(arch, SEEK_END);            // me ubico en el final del archivo.
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
	long tamanio;
	FILE* arch; 
	char caracteres[100];
	tamanio=0;
	
	strcat (argp->nombre,"-");
	strcat (argp->nombre,(char)(((int)'0')+argp->v));
	
	arch=fopen(argp->nombre, "rb"); // abro el archivo de solo lectura.
	if (arch!=NULL){
	  fseek(arch, SEEK_END);            // me ubico en el final del archivo.
	  tamanio=ftell(arch);                     // obtengo su tamanio en BYTES.
	  fclose(arch);                               // cierro el archivo.
	}
	
	//reservamos espacio para mandar el archivo
	resu=malloc (sizeof(char)*tamanio+100);
	
	arch = fopen(argp->nombre,"r");
	if (arch!=NULL){
	  while (feof(arch) == 0)
	  {
		  fgets(caracteres,100,arch);
		  strcat (resu,caracteres);
	  }
	}
	result=resu;
	
	return &result;
}

char **
listararchivos_1_svc(void *argp, struct svc_req *rqstp)
{
	static char * result;
	FILE* arch; 
	char caracteres[100];
	char* resu;
	FILE* arch; 
	
	system ("ls ./archivos > archivos.txt");
	
	arch=fopen("archivos.txt", "rb"); // abro el archivo de solo lectura.
	fseek(arch, SEEK_END);            // me ubico en el final del archivo.
	tamanio=ftell(arch);                     // obtengo su tamanio en BYTES.
	fclose(arch);                               // cierro el archivo.
	
	//reservamos espacio para mandar el archivo
	resu=malloc (sizeof(char)*tamanio+100);
	
	arch = fopen("archivos.txt","r");
	while (feof(arch) == 0)
 	{
 		fgets(caracteres,100,arch);
 		strcat (resu,caracteres);
 	}
	result=resu;
	
	return &result;
}
