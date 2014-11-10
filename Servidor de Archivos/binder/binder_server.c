#include "binder.h"
#include <unistd.h>
#include  <stdio.h>
#include <string.h>
#include <pthread.h>

#define COMISIONES 6
#define init pthread_mutex_init     /* Macro para la función pthread_mutex_init */
#define lock pthread_mutex_lock     /* Macro para la función pthread_mutex_lock */
#define unlock pthread_mutex_unlock /* Macro para la función pthread_mutex_unlock */

struct archivoVersion{
int v;
char* archivo;
};



//Tabla de archivos con la ultima version
struct archivoVersion* arch;
char* ips[COMISIONES];
pthread_mutex_t versiones = PTHREAD_MUTEX_INITIALIZER;
int cantIps=0;
int cantArch=0;
//init( &versiones, NULL);

//retorna 1 si te pudiste registrarse, 0 en caso de que ya estes registrado, 2 este lleno el binder
int *
registrarse_1_svc(char **argp, struct svc_req *rqstp)
{
	static int result;
	result=0;
	int existe=0;
	int i=0;
	//verificamos que no esten todas las comisiones registradas
	if (cantIps<COMISIONES){
	  //verifico que ya no este registrado
	  while (i<cantIps && !existe)
	  {
	    if (!strcmp(ips[i],*argp))
	      existe=1;
	    else
	      i++;
	  }
	  //si no esta registrado lo agrego
	  if (!existe)
	  {
	    printf("se registro:%s\n\n",*argp);
	    ips[cantIps]=malloc(strlen(*argp));
	    *(ips[cantIps])='\0';
	    strcat(ips[cantIps],*argp);
	    cantIps++;
	    result=1;
	  }
	}
	//en caso de que este lleno el binder, nunca deberia ocurrir
	else
	  result=2;
	return  &result;
}

char **
getipregistradas_1_svc(void *argp, struct svc_req *rqstp)
{
  
	static char * result;
	int i;
	char* aux;
	aux=(char *)malloc(100);
	*aux='\0';
	//armo un string con el formato ip\nip\nip\n
	for (i=0;i<cantIps;i++)
	{
	  sprintf(aux,"%s%s\n",aux,ips[i]);
	  //strcat(aux,ips[i]);
	  //strcat(aux,"\n");
	}
	strcat(aux,"\0");
	result =aux;
	return &result;
}

// retorna 1 si pudo eliminar la ip 0 en caso de que no exista la ip a eliminar
int *
eliminarip_1_svc(char **argp, struct svc_req *rqstp)
{
	static int result;

	int i,j;
	int encontre;
	i=0;
	encontre=0;
	result=0;
	//busco la ip a eliminar
	while (i<cantIps && !encontre)
	{
	  //en caso de encontrarla la borro
	  if (!strcmp(ips[i],*argp))
	  {
	    encontre=1;
	    result=1;
	    free(ips[i]);
	    cantIps--;
	  }
	  else
	    i++;
	}
	//dejo todas las ip al principio del arreglo
	if (encontre==1 && cantIps!=0)
	    for (j=i;j<COMISIONES-1;j++)
		ips[j]=ips[j+1];
      
	return &result;
}

char **
update_1_svc(void *argp, struct svc_req *rqstp)
{
	static char * result;
	char* aux;
	int i=0;
	char c;
	//el +2 es para reservar para el espacio y \n. El +1 reserva espacio para \0 al final de la tabla
	aux=(char * )malloc((cantArch* (sizeof(struct archivoVersion)+2))+1);
	*aux='\0';
	//concateno los nombres de los archivos y version es un string con el formato archivo version\n
	while (i<cantArch){
	  sprintf(aux,"%s%s %i\n",aux,(arch+i)->archivo, (arch+i)->v);
	  /*	Codigo Anterior
	   * strcat(aux,(arch+i)->archivo);
	  strcat(aux," ");
	  c=(char)((arch+i)->v+'0');
	  strcat(aux,&c);
	  c='\n';
	  strcat(aux,&c);*/
	  i++;
	}
	//marco el final
	c='\0';
	strcat(aux,&c);
	result=aux;
	return &result;
}

int *
getversionaescribir_1_svc(char **argp, struct svc_req *rqstp)
{
	static int  result;
	int i;
	int encontre;
	//locacion dinamica de la tabla
	if (cantArch==0)
	  arch=(struct archivoVersion*)malloc(100* sizeof(struct archivoVersion));
	else
	  if (cantArch%100==0)
	    arch=(struct archivoVersion*)realloc (arch,(cantArch+100)*sizeof(struct archivoVersion));
	i=0;
	encontre=0;
	//verifico si el nombre del archivo ya esta en la tabla
	while (i<cantArch && !encontre)
	{
	  //si esta verifico que version debe crear el cliente
	  if (strcmp((arch+i)->archivo,*argp)==0)
	  {
	    encontre=1;
	    lock (&versiones);
	    ((arch+i)->v)++;
	    result=(arch+i)->v;
	    unlock (&versiones);
	  }
	  i++;
	}
	//en caso de no existir se lo agrega a la table  y la version a crear es 0
	if (encontre==0)
	{
	  result=0;
	  (arch+cantArch)->archivo=(char*)malloc(100);
	  *((arch+cantArch)->archivo)='\0';
	  strcat(((arch+cantArch)->archivo),*argp);
	  lock (&versiones);
	  (arch+cantArch)->v=0;
	  cantArch++;
	  unlock (&versiones);
	}
	return &result;
}
