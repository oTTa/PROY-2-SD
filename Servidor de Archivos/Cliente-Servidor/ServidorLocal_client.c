#include "ServidorLocal.h"
#include <string.h>

//VARIABLES GLOBALES
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
char* call;

//METODOS
void MostrarDialogo();
void CrearArchivo();
void ModificarArchivo();
void MostrarArchivo();
void ListarArchivos();

void
ej2_1(char *host)
{	
	char opcion;	
	clnt = clnt_create (host, ej2, ej2v1, "tcp");
	
	if (clnt == NULL) {
		clnt_pcreateerror (host);
		exit (1);
	}
	
	creararchivo_1_arg=(char*)malloc (sizeof(char)*128);
	modificararchivo_1_arg.nombre=(char*)malloc (sizeof(char)*128);
	call =(char*)malloc (sizeof(char)*256);
	getarchivo_1_arg.nombre=(char*)malloc (sizeof(char)*128);
	  
	while (opcion!='5'){
	    MostrarDialogo();
	    fflush(stdin);
	    scanf (" %c",&opcion);
	    fflush(stdin);
	    printf("\n\n");
	    switch (opcion){
	      case ('1'):
			CrearArchivo();
			break;
	      case ('2'):
		        ModificarArchivo();			
			break;
	      case ('3'):
			MostrarArchivo();
			break;
	      case ('4'):
			ListarArchivos();
			break;
	    }
	}
	free (modificararchivo_1_arg.nombre);
	free (creararchivo_1_arg);
	free (call),
	free(getarchivo_1_arg.nombre);
	printf ("Cerrando File Server - version cliente \n");

	clnt_destroy (clnt);
}

void MostrarDialogo (){
  printf ("\n\n################################# File Server - Version Cliente #################################\n\n");
  printf ("\n\nOPCIONES:\n");
  printf(" 	1 - Crear archivo \n 	2 - Modificar archivo \n 	3 - Mostrar archivo \n 	4 - Listar archivos \n 	5 - Salir\n" );
  printf ("INGRESE UNA OPCION (1-5): ");
}

void CrearArchivo () {
  *creararchivo_1_arg='\0';
  int valido=0;
  while (valido==0){
    printf ("Ingrese el nombre del archivo a crear:");
    scanf ("%s",creararchivo_1_arg);
    fflush(stdin);
    if (verificarNombre(creararchivo_1_arg))
      valido=1;
    else
      printf ("El nombre no es valido, recuerde que los nombre deben contener solo letras mayusculas y minusculas.\n");
  }
  
  result_1 = creararchivo_1(&creararchivo_1_arg, clnt);
  if (result_1 == (int *) NULL)
	clnt_perror (clnt, "call failed");
  if (*result_1==1)
      printf("Archivo creado correctamente\n");
  else
      printf("El archivo no pudo ser creado ya que existe un archivo con ese nombre\n");
}

void ModificarArchivo (){
	FILE* arch;
	char caracteres[100];
        int aux;
	int valido;
	valido=0;
	*modificararchivo_1_arg.nombre='\0';
	
	 while (valido==0){
	    printf ("Ingrese el nombre del archivo a modificar:");
	    scanf ("%s",modificararchivo_1_arg.nombre);
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
	printf ("\nEl archivo %s fue modificado con exito\n",modificararchivo_1_arg.nombre);
}

void MostrarArchivo (){
	int valido=0;
	while (valido==0){
	  printf ("Ingrese el nombre del archivo a leer:");
	  scanf ("%s",getarchivo_1_arg.nombre);
	  fflush(stdin);
	  if (verificarNombre(getarchivo_1_arg.nombre))
	    valido=1;
	  else
	    printf ("El nombre no es valido, recuerde que los nombre deben contener solo letras mayusculas y minusculas.\n");
	}
        
	printf ("Ingrese la version del archivo a leer:");
	scanf ("%i",&(getarchivo_1_arg.v));
	fflush(stdin);
	result_4 = getarchivo_1(&getarchivo_1_arg, clnt);
	if (result_4 == (char **) NULL) {
		clnt_perror (clnt, "call failed");
	}
	else{
	  printf("\n\n ++++++++++++++++++++ Archivo: %s-%i ++++++++++++++++++++\n\n",getarchivo_1_arg.nombre,getarchivo_1_arg.v);
	  printf ("%s\n",*result_4);
	  printf("\n\n ++++++++++++++++++++ Fin de archivo: %s-%i ++++++++++++++++++++\n\n",getarchivo_1_arg.nombre,getarchivo_1_arg.v);
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
	ej2_1 (host);
exit (0);
}
