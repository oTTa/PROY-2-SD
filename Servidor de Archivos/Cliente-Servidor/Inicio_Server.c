#include "Servidor.h"
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <netdb.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include "../binder/binder.h"

char *ip_local();
void inicioServer ();
int invertir (int numero);
void IpServerActualizador (char* ip,char* ips);
int* versionesActualizar (char* nombre,int ultimaVersion);

void inicioServer (){
	//declaro las variables a utilizar
	char * *update_result;
	char *update_1_arg;
	char* call; 
	char* misArchivos;
	char ip[17];
	char** ips;
	char* nombre;
	int tamanio;
	int posUpdate;
	int posNombre;
	char* caracteres;
	int versionArchivoBinder;
	int leerVersion;
	int* misVersiones; 
	char *getipregistradas_1_arg;
	
	CLIENT *binder_clnt;
	FILE* arch; 
	
	//creo una conexion con el binder
	binder_clnt = clnt_create ("localhost", binder, binderv1, "tcp");
	if (binder_clnt == NULL) {
		clnt_pcreateerror ("localhost");
		exit (1);
	}
	
	//obtengo las ips para recibir los archivos
	ips = getipregistradas_1((void*)&getipregistradas_1_arg, binder_clnt);
	if (ips == (char **) NULL) {
	    clnt_perror (binder_clnt, "call failed");
	}
	
	//obtengo los archivos en el server
	update_result = update_1((void*)&update_1_arg, binder_clnt);
	if (update_result == (char **) NULL) {
		clnt_perror (binder_clnt, "call failed");
		exit(0);
	}
	
	clnt_destroy (binder_clnt);
	//Actualizacion
	nombre=(char*) malloc(200);
	posNombre=0;
	posUpdate=0;
	leerVersion=0;
	versionArchivoBinder=0;
	int pos=0;
	while (*(*update_result+posUpdate)!='\0')
	{	//armo el nombre del archivo al formato de mi server, nombre-version
		if (*(*update_result+posUpdate)!='\n'){
		  if (!leerVersion){
		    if (*(*update_result+posUpdate)!=' ')
		      *(nombre+posNombre)=*(*update_result+posUpdate);
		    else{
		      *(nombre+posNombre)='-';
		       *(nombre+posNombre+1)='\0';
		      leerVersion=1;
		    }
		  }
		  else
		    versionArchivoBinder=versionArchivoBinder*10+(int)(*(*update_result+posUpdate) - '0');
		posNombre++;
		}
		else{
		  versionArchivoBinder=invertir(versionArchivoBinder);
		  leerVersion=0;
		  posNombre=0;
		  misVersiones=versionesActualizar(nombre,versionArchivoBinder);
		  versionArchivoBinder=0;
		  printf("%s:",nombre);
		  while (*(misVersiones+pos)!=NULL){
		    printf(" %i",*(misVersiones+pos));
		    pos++;
		  }
		  printf("\n");
		  pos=0;
		  free(misVersiones);
		}
		 posUpdate++;
	}
	free(nombre);
}

int invertir (int numero){
  int aux=0;
  while (numero!=0){
    aux=aux*10+numero%10;
    numero=numero/10;
  }
  return aux;
}


int* versionesActualizar (char* nombre,int ultimaVersion){
   int* misVersiones; 
   int* resu;
   char* call;
   int aux;
   int tamanio;
   char* versiones;
   char* caracteres;
   int leerVersion;
   int posVersiones;
   int posResu;
   FILE* arch;
   int i;
   int pertenece;
   
   printf("version %s%i\n",nombre,ultimaVersion);
   
   //copia las versiones es un archivo para leerlas
   misVersiones=(int*)malloc (sizeof(int)*11);
   resu=(int*)malloc (sizeof(int)*(ultimaVersion+1));
   call=(char*) malloc (150);
   *call='\0';
   strcat (call,"ls ../archivos | grep ");
   strcat (call,nombre);
   strcat (call," > misVersiones.txt");
   system (call);
   free (call);
   //calculo el tamaño del archivo donde estan las versiones
   arch=fopen("misVersiones.txt", "rb"); // abro el archivo en solo lectura.
   if (arch!=NULL){
      fseek(arch,0, SEEK_END);            // me ubico en el final del archivo.
      tamanio=ftell(arch);                     // obtengo su tamanio en BYTES.
      fclose(arch);                               // cierro el archivo.
   }
   
   //guardo el espacio necesario para pasarlo a ram
   versiones=(char*)malloc (sizeof(char)*tamanio);
   *versiones='\0';
   
   arch = fopen("misVersiones.txt","r");
   caracteres=(char*)malloc(100);
   //paso las versiones a ram para poder usarlas mas comodo
   if (arch!=NULL){
    while (fgets(caracteres,100,arch) != NULL){
	strcat (versiones,caracteres);
    }
    fclose(arch);
   }
   free(caracteres);
   aux=0;
   leerVersion=0;
   posVersiones=0;
   //obtengo las versiones que estan en mi servidor
   while (*(versiones+aux)!='\0'){
        if (posVersiones%10==0 ){
	   misVersiones=(int*)realloc (misVersiones,(posVersiones+11)*sizeof(int));
	}
	if (*(versiones+aux)!='\n'){
	  if (!leerVersion){
	    if (*(versiones+aux)=='-'){
	      leerVersion=1;
	      *(misVersiones+posVersiones)=0;
	    }
	  }
	 else
	   if(*(versiones+aux)>='0' && *(versiones+aux)<='9')
	      *(misVersiones+posVersiones)=*(misVersiones+posVersiones)*10+(int)(*(versiones+aux) - '0');
       }
       else{
	 posVersiones++;
	 leerVersion=0;
       }
       aux++;
   }
   *(misVersiones+posVersiones)=NULL;
   i=0;
   posResu=0;
   //calculo las versiones que me faltan
   for (i=0;i<=ultimaVersion;i++){
      aux=0;
      pertenece=0;     
      while (aux<posVersiones && !pertenece){
	if (*(misVersiones+aux)==i)
	  pertenece=1;
	aux++;
      }
      if (!pertenece) {
	*(resu+posResu)=i;
	posResu++;
      }
      else
	pertenece=0;
   }
   free (misVersiones);
   //marco el final del arreglo
   *(resu+posResu)=NULL;
   return resu;
}


void IpServerActualizador (char* ip,char* ips){
  int posIP;
  char* iplocal;
  int continuar;
  int i;
  i=0;
  continuar=1;
  posIP=0;
  iplocal=ip_local();
 
  while (*(ips+i)!='\0' && continuar){
	  if (*(ips+i)!='\n'){
	    ip[posIP]=*(ips+i);
	    posIP++;
	  }
	  else{
	    ip[posIP]='\0';
	    posIP=0;
	    if (strcmp(ip,iplocal) != 0){
	      continuar=0;
	      printf("Actualizacion con:(%s)\n",ip);
	    }
	  }
	  i++;
  }
}


char *ip_local() {
  struct sockaddr_in host;
  char nombre[255];
  char *ip;
  ip =(char*) malloc(17); 
  *ip='\0';
  strcat(ip,"192.168.0.2\0");
  gethostname(nombre, 255);
  host.sin_addr = * (struct in_addr*) gethostbyname(nombre)->h_addr;
  //ip = inet_ntoa(host.sin_addr);
  return ip;
}

int
main (int argc, char *argv[])
{
  inicioServer ();
  exit (0);
} 
