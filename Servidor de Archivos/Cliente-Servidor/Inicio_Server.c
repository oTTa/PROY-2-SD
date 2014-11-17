#include "Servidor.h"
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <netdb.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include "../binder/binder.h"

#include <sys/types.h>
#include <sys/socket.h>
#include <sys/ioctl.h>
#include <net/if.h>


//Declaracion de metodos
char *ip_local();
void inicioServer ();
int invertir (int numero);
void IpServerActualizador (char* ip,char* ips,CLIENT *binder_clnt);
int* versionesActualizar (char* nombre,int ultimaVersion);
void actualizar (char* nombre,int* versiones,char* ips,CLIENT *binder_clnt);
/**************************************************/

char* ip_hamachi(){
   int fd;
 char* ip;
 struct ifreq ifr;

 fd = socket(AF_INET, SOCK_DGRAM, 0);

 /* I want to get an IPv4 IP address */
 ifr.ifr_addr.sa_family = AF_INET;

 /* I want IP address attached to "eth0" */
 strncpy(ifr.ifr_name, "ham0", IFNAMSIZ-1);

 ioctl(fd, SIOCGIFADDR, &ifr);

 close(fd);
 ip=inet_ntoa(((struct sockaddr_in *)&ifr.ifr_addr)->sin_addr);
 return ip;
}

void inicioServer (char *binderIP){
	//declaro las variables a utilizar
	char * *update_result;
	char *update_1_arg;
	char* call; 
	char* misArchivos;
	char** ips;
	char* iplocal;
	char* nombre;
	int tamanio;
	int posUpdate;
	int posNombre;
	char* caracteres;
	int versionArchivoBinder;
	int leerVersion;
	int* misVersiones; 
	char *getipregistradas_1_arg;
	char *nombreArchivo;
	int  *registrado;
	int necesitoActualizar;
	
	CLIENT *binder_clnt;
	FILE* arch; 
	
	arch=fopen ("binder.txt","w");
	fprintf(arch,binderIP);
	fclose (arch);
	//creo una conexion con el binder
	binder_clnt = clnt_create (binderIP, binder, binderv1, "tcp");
	if (binder_clnt == NULL) {
		printf("Imposible establecer una conexion con el binder\n");
		clnt_pcreateerror (binderIP);
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

	//Actualizacion
	nombre=(char*) malloc(200);
	*nombre='\0';
	nombreArchivo=(char*) malloc(200);
	*nombreArchivo='\0';
	posNombre=0;
	posUpdate=0;
	leerVersion=0;
	versionArchivoBinder=0;
	necesitoActualizar=0;
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
		  strncpy( nombreArchivo, nombre, strlen(nombre)-1 );
		  *(nombreArchivo+strlen(nombre)-1)='\0';
		  if (*misVersiones!=-1){//es necesario actualizar el archivo
		    necesitoActualizar=1;
		    if (**ips!='\0')//me actualizo solo si hay alguien conectado
		      actualizar (nombreArchivo,misVersiones,*ips,binder_clnt);
		  }
		  free(misVersiones);
		}
		 posUpdate++;
	}
	free(nombre);
	free(nombreArchivo);
	if (necesitoActualizar==1 && **ips=='\0'){
	  printf("No hay nadie conectado al binder para actualizarme y empezar el servicio\n");
	}
	else{
	  //iplocal=ip_local();
	  iplocal=ip_hamachi();
	  registrado = registrarse_1(&iplocal, binder_clnt);
	  if (registrado == (int *) NULL) {
				  printf("Imposible registrarse en el binder\n");
				  clnt_perror (binder_clnt, "call failed");
	  }
	  printf("Me registre en el binder con la ip (%s)\n",iplocal);
	  clnt_destroy(binder_clnt);
	  printf("++++++++++++ Servidor listo para recibir peticiones ++++++++++\n\n");
	  execv("./Servidor_server", NULL);
	}
}

void eliminarmeDelBinder (CLIENT *binder_clnt){
  int* elimino;
  char* iplocal;
  printf("Me elimino del binder por que estoy registrado pero no conectado con los demas servidores.\n");
  //iplocal=ip_local();
  iplocal=ip_hamachi();
  elimino = eliminarip_1(&iplocal, binder_clnt);
  if (elimino == (int *) NULL) {
	clnt_perror (binder_clnt, "call failed");
  }
  else{
    if (*elimino==0)
	    printf ("Alguien ya me elimino del binder\n");
    else
	    printf("Eliminado del binder con exito\n");
  }
}

void actualizar (char* nombre,int* versiones,char* ips,CLIENT *binder_clnt){
  char ip[17];
  char *ipborrar;
  char **ipsReintentar;
  CLIENT *servidor_actualizados;
  stream_t  *archivo;
  argumento  getversion_1_arg;
  char* nombreArchivoCrear;
  char* path;
  FILE* arch; 
  int aux;
  char str [15];
  char *getipregistradas_1_arg;
  path=(char*)malloc (150);
  *path='\0';
  nombreArchivoCrear=(char*)malloc (120);
  *nombreArchivoCrear='\0';
  
  IpServerActualizador(ip,ips,binder_clnt);
  
  if (*ip!='\0'){//en caso de que la ip actualizadora haya sido yo.
      servidor_actualizados = clnt_create (ip, ServersFile, ServersV1, "tcp");
      if (servidor_actualizados == NULL) {
		    clnt_pcreateerror (ip);
		    printf("Se elimino el server %s ya que esta desconectado\n",ip);
		    ipborrar=(char*)malloc (17);
		    *ipborrar='\0';
		    strcat (ipborrar,ip);
		    eliminarip_1(&ipborrar, binder_clnt);
		    free (ipborrar);
		    ipsReintentar = getipregistradas_1((void*)&getipregistradas_1_arg, binder_clnt);
		    ips=*ipsReintentar;
		    printf("Se reintentara actualizar con otro servidor\n");
		    actualizar (nombre,versiones,ips,binder_clnt);
      }
      else{
	aux=0;
	while (aux<strlen(nombre)){
	  getversion_1_arg.nombre[aux]=*(nombre+aux);
	  aux++;
	}
	getversion_1_arg.nombre[aux]='\0';
	aux=0;
	
	printf("Se procedera a actualizar el archivo %s\n",getversion_1_arg.nombre);
	while (*(versiones+aux)!=-1){
	  *path='\0';
	  *str='\0';
	  getversion_1_arg.ver=*(versiones+aux);
	  getversion_1_arg.file.stream_t_val="";
	  getversion_1_arg.file.stream_t_len=0;
	  printf ("Descarganlo la version %i ",getversion_1_arg.ver);
	  archivo = getversion_1(&getversion_1_arg, servidor_actualizados);
	  if (archivo == (stream_t *) NULL) {
		clnt_perror (servidor_actualizados, "call failed");
		printf("Se elimino el server %s ya que esta desactualizado\n",ip);
		ipborrar=(char*)malloc (17);
		*ipborrar='\0';
		strcat (ipborrar,ip);
		eliminarip_1(&ipborrar, binder_clnt);
		free (ipborrar);
		ipsReintentar = getipregistradas_1((void*)&getipregistradas_1_arg, binder_clnt);
		ips=*ipsReintentar;
		printf("Se reintentara actualizar con otro servidor.\n");
		actualizar (nombre,versiones,ips,binder_clnt);
	  }
	  else{
	    strcat (path,"../archivos/");
	    strcat(path,nombre);
	    strcat(path,"-");
	    sprintf(str, "%d",*(versiones+aux));
	    strcat(path,str);
	    strcat(path,".c");
	    
	    arch=fopen (path,"w");
	    fprintf(arch,archivo->stream_t_val);
	    fclose (arch); 
	    printf("- Descargada con exito.\n");
	    aux++;
	  }
	} 
	free(nombreArchivoCrear);
	clnt_destroy(servidor_actualizados);
      }
  }
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
   *(misVersiones+posVersiones)=-1;
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
   *(resu+posResu)=-1;
   return resu;
}


void IpServerActualizador (char* ip,char* ips,CLIENT *binder_clnt){
  int posIP;
  char* iplocal;
  int continuar;
  int i;
  i=0;
  continuar=1;
  posIP=0;
  //iplocal=ip_local();
  iplocal=ip_hamachi();
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
	      printf("La actualizacion se realizara con el servidor %s\n",ip);
	    }
	    else
	      eliminarmeDelBinder(binder_clnt);
	  }
	  i++;
  }
  if (strcmp(ip,iplocal) == 0)
    *ip='\0';
}


char *ip_local() {
  struct sockaddr_in host;
  char nombre[255];
  char *ip;
  gethostname(nombre, 255);
  host.sin_addr = * (struct in_addr*) gethostbyname(nombre)->h_addr;
  ip = inet_ntoa(host.sin_addr);
  return ip;
}

int
main (int argc, char *argv[])
{
  char *binderIP;

	if (argc < 2) {
		printf ("Debes ingresar la ip del binder como argumento\n");
		exit (1);
	}
	binderIP = argv[1];
  inicioServer (binderIP);
  exit (0);
} 
