#include "Servidor.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "../binder/binder.h"
#include <netdb.h>
#include <netinet/in.h>
#include <arpa/inet.h>

//METODOS AUXILIARES

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

int imprimirIPConectada(struct svc_req *rqstp)
{
printf("%d.%d.%d.%d",
(rqstp->rq_xprt->xp_raddr.sin_addr.s_addr&0xFF),
((rqstp->rq_xprt->xp_raddr.sin_addr.s_addr&0xFF00)>>8),
((rqstp->rq_xprt->xp_raddr.sin_addr.s_addr&0xFF0000)>>16),
((rqstp->rq_xprt->xp_raddr.sin_addr.s_addr&0xFF000000)>>24));

return 0;
}
//FIN METODOS AUXILIARES

void *
escribirversion_1_svc(argumento *argp, struct svc_req *rqstp)
{
	static char * result;
	char* path;
	char str[15];
	FILE *archivoNuevo;
	
	printf("\nRecibiendo el archivo %s-%i.c desde el server ",argp->nombre,argp->ver);
	imprimirIPConectada(rqstp);
	printf("\n");
	
	path=(char*) malloc (120);
	*path='\0';
	strcat(path,"../archivos/");
	strcat (path,argp->nombre);
	strcat (path,"-");
	sprintf(str, "%d",argp->ver);
	strcat (path,str);
	strcat (path,".c");
	
	archivoNuevo=fopen (path,"w");
	printf("%s\n",argp->file.stream_t_val);
	fprintf(archivoNuevo,argp->file.stream_t_val);

	fclose (archivoNuevo);

	free (path);
	printf("\n\n");
	return (void *) &result;
}

stream_t *
getversion_1_svc(argumento *argp, struct svc_req *rqstp)
{
	static stream_t  result;
	char* path;
	char* contenido;
	int tamanio;
	FILE* arch; 
	char caracteres[100];
	char str[15];
	
	printf("EL servidor ");
	imprimirIPConectada(rqstp);
	printf(" solicito el archivo %s-%i.c\n",argp->nombre,argp->ver);
	
	tamanio=0;
	path=(char*) malloc (120);
	*path='\0';
	strcat(path,"../archivos/");
	strcat (path,argp->nombre);
	strcat (path,"-");
	sprintf(str, "%d",argp->ver);
	strcat (path,str);
	strcat (path,".c");

	arch=fopen(path, "rb"); // abro el archivo de solo lectura.
	if (arch!=NULL){
	    fseek(arch,0, SEEK_END);            // me ubico en el final del archivo.
	    tamanio=ftell(arch);                     // obtengo su tamanio en BYTES.
	    fclose(arch);                               // cierro el archivo.
	}
	//reservamos espacio para mandar el archivo
	contenido=(char*)malloc (sizeof(char)*tamanio);
	*contenido='\0';
	result.stream_t_len=tamanio;
	//armamos el resultado con el contenido del archivo
	arch = fopen(path,"r");
	if (arch!=NULL){
	  while (fgets(caracteres,100,arch) != NULL)
		  strcat (contenido,caracteres);
	}
	
	result.stream_t_val=contenido;
	free (path);
	
	return &result;
}


/*modifica el archivo con nombre argp->nombre en una nueva version que obtiene del binder,
en caso de que no exista crea el archivo con version 0 ya que el binder le respondera 0
retorna 1 si pudo modificarlo 0 en caso contrario*/

int *
modificararchivo_1_svc(nombreContenido *argp, struct svc_req *rqstp)
{
	static int  result;
	int pos;
	char* call; 
	char* nombre;
	FILE *archivoNuevo;
	FILE *Binder;
	char caracter;
	char* str;
	char* path;
	char * *ips;
	char* iplocal;
	char binderIP[17];
	char ip[17];
	char *ipborrar;
	int *version;
	int i;
	int posIP;
	char *getipregistradas_1_arg;
	argumento  escribirversion_1_arg;
	void  *result_escribir;
	CLIENT *clnt,*server;
	
	path=(char*) malloc (230);
	nombre=(char*) malloc (200);
	str=(char*) malloc (15);
	*path='\0';
	*nombre='\0';
	*str='\0';
	result=1;
	
	//creo una conexion con el binder
	Binder=fopen ("binder.txt","r");
	if (Binder==NULL){
	  printf("imposible obtener la ip del binder.");
	  result=0;
	  return &result;
	}
	fgets(binderIP,17,Binder);
	clnt = clnt_create (binderIP, binder, binderv1, "tcp");
	if (clnt == NULL) {
		clnt_perror (clnt, "call failed");
		result=0;
		return &result;
	}
	//obtenlo la nueva version con la que se creara el archivo
	version = getversionaescribir_1(&(argp->nombre), clnt);
	
	//obtengo las ips para mandar el archivo
	ips = getipregistradas_1((void*)&getipregistradas_1_arg, clnt);
	if (ips == (char **) NULL) {
	    clnt_perror (clnt, "call failed");
	    printf("imposible obtener las ips de los servidores.");
	    result=0;
	    return &result;
	}
	
	
	//preraro el argumento para enviarlo a los demas servidores
	escribirversion_1_arg.ver=*version;
	*(escribirversion_1_arg.nombre)='\0';
	strcat(escribirversion_1_arg.nombre,argp->nombre);
	escribirversion_1_arg.file.stream_t_len=strlen(argp->contenido);
	escribirversion_1_arg.file.stream_t_val=(char*)malloc (sizeof(char)*strlen(argp->contenido));
	*(escribirversion_1_arg.file.stream_t_val)='\0';
	strcat(escribirversion_1_arg.file.stream_t_val,argp->contenido);
	
	
	//arma la estructura del path donde se va escribir el archivo
	strcat(argp->nombre,"-");
        strcat (path,"../archivos/");
	strcat(path,argp->nombre);
	sprintf(str, "%d",*version);
	strcat(path,str);
	strcat(path,".c");
	//escribo el contenido en el archivo 
	archivoNuevo=fopen (path,"w");
	fprintf(archivoNuevo,argp->contenido);
	fclose (archivoNuevo);
	
	//envio el archivo a los demas servidores
	i=0;
	posIP=0;
	iplocal=ip_local();
	while (*(*ips+i)!='\0'){
	  if (*(*ips+i)!='\n'){
	    ip[posIP]=*(*ips+i);
	    posIP++;
	  }
	  else{
	    ip[posIP]='\0';
	    posIP=0;
	    if (strcmp(ip,iplocal) != 0){
	       printf("enviando a:(%s)\n",ip);
	      server = clnt_create (ip, ServersFile, ServersV1, "tcp");
	      if (server == NULL) {
		    clnt_pcreateerror (ip);
		    printf("\nSe elimino el server %s ya que esta desconectado y el archivo no pudo ser enviado\n",ip);
		    ipborrar=(char*)malloc (17);
		    *ipborrar='\0';
		    strcat (ipborrar,ip);
		    eliminarip_1(&ipborrar, clnt);
		    free (ipborrar);
	      }
	      else{
	      result_escribir = escribirversion_1(&escribirversion_1_arg, server);
	      if (result_escribir == (void *) NULL){
				  printf("Fallo el envio.\n");		  
	      }
	      clnt_destroy (server);
	      }
	    }
	  }
	  i++;
	}
	
	clnt_destroy (clnt);
	free (path);
	free (str);
	free (nombre);
	
	
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
	tamanio=-1;
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
	free (call);
	arch = fopen("versiones.txt","r");
	if (arch == NULL)
		printf("\nError de apertura del archivo  versiones.txt \n\n");
	else{
	      //busco la ultima version del archivo  
	    while (feof(arch) == 0)
	    {
	    	  caracter = fgetc(arch);
	          if (caracter=='\n'){
			    *(nombre+pos)='\0';
			    pos=0;
		  }
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
	}
	arch=fopen(path, "rb"); // abro el archivo en solo lectura.
	if (arch!=NULL){
	    fseek(arch,0, SEEK_END);            // me ubico en el final del archivo.
	    tamanio=ftell(arch);                     // obtengo su tamanio en BYTES.
	    fclose(arch);                               // cierro el archivo.
	}
	//verifica que encontro un archivo con su version y procede a copiarlo para enviar
	if (strcmp(path,"../archivos/") && tamanio>=0){
	  //reservamos espacio para mandar el archivo
	  resu=malloc (sizeof(char)*tamanio);
	  *resu='\0';
	  arch = fopen(path,"r");
	  if (arch!=NULL){
	    while (fgets(caracteres,100,arch) != NULL)
		    strcat (resu,caracteres);
	    fclose(arch);
	  }
	}
	else {
	  resu=(char*)malloc(1);
	  *resu='\0';
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


