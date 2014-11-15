#include "Servidor.h"
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <netdb.h>
#include <netinet/in.h>
#include <arpa/inet.h>


//VARIABLES GLOBALES
CLIENT *clnt;
int  *result_2;
nombreContenido  modificararchivo_1_arg;
char * *result_4;
nombreVersion  getarchivo_1_arg;
char * *result_5;
char *listararchivos_1_arg;
char * call;

argumento  getversion_1_arg;
stream_t  *result_getVersion;
void  *result_escribir;
argumento  escribirversion_1_arg;

//METODOS
void MostrarDialogo();
void CrearModificarArchivo();
void MostrarArchivo();
void ListarArchivos();
int verificarNombre (char* nombre);

serversfile_1(char *host)
{
	
	char opcion;
	clnt = clnt_create (host, ServersFile, ServersV1, "tcp");
	if (clnt == NULL) {
		clnt_pcreateerror (host);
		exit (1);
	}
	
	modificararchivo_1_arg.nombre=(char*)malloc (sizeof(char)*128);
	call =(char*)malloc (sizeof(char)*256);
	getarchivo_1_arg.nombre=(char*)malloc (sizeof(char)*100);
	
	escribirversion_1_arg.file.stream_t_val=(char*)malloc (sizeof(char)*22);
	*(escribirversion_1_arg.file.stream_t_val)='\0';
	
	while (opcion!='4'){
	    MostrarDialogo();
	    fflush(stdin);
	    scanf (" %c",&opcion);
	    fflush(stdin);
	    //limpiar pantalla
	    printf("\e[1;1H\e[2J");
	    
	    switch (opcion){
	      case ('1'):
		        CrearModificarArchivo();			
			break;
	      case ('2'):
			MostrarArchivo();
			break;
	      case ('3'):
			ListarArchivos();
			break;
	      case ('4'):
			printf ("Cerrando File Server - version cliente \n");
			break;
	      default:  printf("### Opcion Invalida, debes ingresar una opcion entre 1 y 4 ###\n");
	    }
	}
	free (modificararchivo_1_arg.nombre);
	free (call),
	free(getarchivo_1_arg.nombre);
	

	clnt_destroy (clnt);
	printf ("############# Conexión finalizada - File Server - version cliente ####################3\n");
}

void MostrarDialogo (){
  printf ("\n\n################################# File Server - Version Cliente #################################\n\n");
  printf ("\n\nOPCIONES:\n");
  printf(" 	1 - Crear/Modificar un archivo \n 	2 - Mostrar un archivo \n 	3 - Listar archivos \n 	4 - Salir\n" );
  printf ("INGRESE UNA OPCION (1-4): ");
}


void CrearModificarArchivo (){
	FILE* arch;
	char caracteres[100];
        int aux;
	int valido;
	valido=0;
	*modificararchivo_1_arg.nombre='\0';
	
	 while (valido==0){
	    printf ("Ingrese el nombre del archivo a crear o modificar:");
	    scanf (" %[^\n]",modificararchivo_1_arg.nombre);
	    fflush(stdin);
	    if (verificarNombre(modificararchivo_1_arg.nombre))
	      valido=1;
	    else
	      printf ("El nombre no es valido, recuerde que los nombre deben contener solo letras mayusculas y minusculas.\n");
	  }
	*getarchivo_1_arg.nombre='\0';
        strcat (getarchivo_1_arg.nombre,modificararchivo_1_arg.nombre);
	//para obtener la ultima version la cual la modificacion va a ser sobre ella
	getarchivo_1_arg.v=-1;
	result_4 = getarchivo_1(&getarchivo_1_arg, clnt);
	if (result_4 == (char **) NULL) {
		clnt_perror (clnt, "call failed");
	}
	
	arch = fopen(modificararchivo_1_arg.nombre,"w");
	if (arch!=NULL){
	  fprintf(arch, *result_4);
	  fclose(arch);
	}
	*call='\0';
	strcat (call,"vi ");
	strcat (call,modificararchivo_1_arg.nombre);
	system (call);
	*call='\0';
	arch=fopen(modificararchivo_1_arg.nombre, "rb"); // abro el archivo de solo lectura.
	if (arch!=NULL){
	  fseek(arch,0, SEEK_END);            // me ubico en el final del archivo.
	  aux=ftell(arch);                     // obtengo su tamanio en BYTES.
	  fclose(arch);                               // cierro el archivo
	}
	modificararchivo_1_arg.contenido=(char*)malloc (sizeof(char)*aux);
       *modificararchivo_1_arg.contenido='\0';
	arch = fopen(modificararchivo_1_arg.nombre,"r");
	if (arch!=NULL){
	  while (fgets(caracteres,100,arch)!=NULL)
	  {
		  strcat (modificararchivo_1_arg.contenido,caracteres);
	  }
	  fclose (arch);
	  
	  result_2 = modificararchivo_1(&modificararchivo_1_arg, clnt);
	  if (result_2 == (int *) NULL) {
		  clnt_perror (clnt, "call failed");
	  }
	}
	*call='\0';
	strcat (call,"rm ");
	strcat (call,modificararchivo_1_arg.nombre);
	arch = fopen(modificararchivo_1_arg.nombre, "r");
	//el archivo se abrio con exito y sera borrado para no dejar archivos temporales innecesarios
	if (arch!=NULL){
	  system (call);
	  fclose(arch);
	}
	free (modificararchivo_1_arg.contenido);
	if(*result_2==1)
	  printf ("\nEl archivo %s fue creado/modificado con exito\n",modificararchivo_1_arg.nombre);
	else
	  printf ("\nEl archivo %s NO pudo ser creado/modificado\n",modificararchivo_1_arg.nombre);
}

void MostrarArchivo (){
	int opcionVersion;
	int valido=0;
	opcionVersion=0;
	getarchivo_1_arg.v=-1;
	while (opcionVersion !=1 && opcionVersion!=2){
	  printf ("1- Si deseas ver la ultima version \n2- Si deseas ver una version especifica\n\n");
	  printf("Ingrese una opcion:");
	  scanf("%i",&opcionVersion);
	  if (opcionVersion !=1 && opcionVersion!=2)
	    printf ("Opcion invalida, debes ingresar 1 o 2\n\n");
	}
	
	while (valido==0){
	  printf ("Ingrese el nombre del archivo a leer:");
	  scanf (" %[^\n]",getarchivo_1_arg.nombre);
	  fflush(stdin);
	  if (verificarNombre(getarchivo_1_arg.nombre))
	    valido=1;
	  else
	    printf ("El nombre no es valido, recuerde que los nombre deben contener solo letras mayusculas y minusculas.\n");
	}
        
        if (opcionVersion==2)
	{
	  while (getarchivo_1_arg.v<0){
	    printf ("Ingrese la version del archivo a leer:");
	    scanf ("%i",&(getarchivo_1_arg.v));
	    fflush(stdin);
	    if (getarchivo_1_arg.v<0)
	      printf("La version a leer debe ser un numero positivo \n");
	  }
	}
	//-1 lee la ultima version
	else
	  getarchivo_1_arg.v=-1;
	result_4 = getarchivo_1(&getarchivo_1_arg, clnt);
	if (result_4 == (char **) NULL) {
		clnt_perror (clnt, "call failed");
	}
	else
	{
	    if (opcionVersion==2)
	    {
		printf("\n\n ++++++++++++++++++++ Archivo: %s-%i.c ++++++++++++++++++++\n\n",getarchivo_1_arg.nombre,getarchivo_1_arg.v);
		printf ("%s\n",*result_4);
		printf("\n\n ++++++++++++++++++++ Fin de archivo: %s-%i.c ++++++++++++++++++++\n\n",getarchivo_1_arg.nombre,getarchivo_1_arg.v);
	    }
	    else
	    {
		printf("\n\n ++++++++++++++++++++ Archivo: %s.c ++++++++++++++++++++\n\n",getarchivo_1_arg.nombre);
		printf ("%s\n",*result_4);
		printf("\n\n ++++++++++++++++++++ Fin de archivo: %s.c ++++++++++++++++++++\n\n",getarchivo_1_arg.nombre);
	    }
	}
}

void ListarArchivos (){
        result_5 = listararchivos_1((void*)&listararchivos_1_arg, clnt);
	if (result_5 == (char **) NULL) {
		clnt_perror (clnt, "call failed");
	}
	printf ("\n\n ++++++++++++++++++ Archivos en el servidor +++++++++++++++++\n\n");
	printf ("%s\n",*result_5);
	
}

// verifica que el nombre del archivo solo contenga letras entre a-z y A-Z
//1 si no contiene caracteres distintos a los mencionados 0 si contiene
int verificarNombre (char* nombre){
  int resu=1;
  int i=0;
  if (*nombre=='\0')
    resu=0;
  while ((*(nombre+i)!='\0') && resu==1)
  {
    if (!((*(nombre+i)>='a' && *(nombre+i)<='z') ||
	(*(nombre+i)>='A' && *(nombre+i)<='Z')))
        resu=0;
    i++;
  }
  return resu;
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
	serversfile_1 (host);
exit (0);
}
