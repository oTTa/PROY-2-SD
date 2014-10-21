#include "socket.h"
#include "tablero.h"
#include <stdio.h>
#include <string.h>
#include <time.h>
#include <stdlib.h>


char buf[MSG_SIZE];
void Acciones(char * mensaje);
char * ingresarMensaje();
coord* ingresarCoordenada ();
void insertarBarcoEnTablero (int tamano, int barco);
void CargarTablero ();
void informarExito (int exito);
coord* mensajeAcoordenada (char* mensaje);
char* ingresarCoordenaDeAtaque(coord* coordA);
int verificarGanar();

int main(int argc, char **argv)
{
    char * contrincante;
    char * mensaje;
    char * puerto;
    coord* coordAtaque;
    int exitoAtaque;
    int gane=1;
    int termino=1;
    //controlamos la cantidad de parametros ingresada
    if(argc<3 || argc>3)
    {
		printf("Cantidad de parametros invalido, debe ingresar <IP CONTRINCANTE,PUERTO> -\n"); 
		exit(1);
    }
    
    contrincante=argv[1];
    puerto = argv[2];
    //mensaje= (char*)malloc (sizeof(char)*MSG_SIZE);
   inicializarTablero();
   CargarTablero();
   coordAtaque=(coord*) malloc(sizeof(coord));
   if ( contrincanteConectado(contrincante, puerto)){//Modo cliente
	while(termino)
	{
            //Menu();
	    
	    mensaje = ingresarCoordenaDeAtaque(coordAtaque);
            enviarMsj(mensaje);
            mensaje = recibirMsj();
	    exitoAtaque=*mensaje-'0';
	    informarAtaque(coordAtaque,exitoAtaque);
	    informarExito (exitoAtaque);
	    mostrarTableros();
            mensaje = recibirMsj();
	    coordAtaque=mensajeAcoordenada(mensaje);
	    exitoAtaque=recibirAtaque(coordAtaque);
	    if (exitoAtaque==3){
	      gane= verificarGanar();
	      termino=0;
	       if (gane==0)
		printf ("EMPATASTE\n");
	      else
		  printf ("Perdiste\n");  
	    
	    }
	    else
	      if (!obtenerTotalBarcosEnemigos()){
		termino=0;
		printf ("Ganaste\n");
	      }
	    buf[0]=(char)(((int)'0')+exitoAtaque);
	    buf [1]='\0';
	    mensaje=buf;
            enviarMsj(mensaje);
	}
		cerrarSesion();
    }
    else //Modo Servidor
    {
        iniciarServicio(puerto); 
	while(termino){
	    mensaje = recibirMsj();
	    coordAtaque=mensajeAcoordenada(mensaje);
	    exitoAtaque=recibirAtaque(coordAtaque);
	    buf[0]=(char)(((int)'0')+exitoAtaque);
	    buf [1]='\0';
	    mensaje=buf;
            enviarMsj(mensaje);
            mensaje = ingresarCoordenaDeAtaque(coordAtaque);
            enviarMsj(mensaje);
            mensaje = recibirMsj();
	    exitoAtaque=*mensaje-'0';
	    informarAtaque(coordAtaque,exitoAtaque);
	      if (exitoAtaque==3){
	      gane= verificarGanar();
	      termino=0;
	      if (gane==0)
		printf ("EMPATASTE\n");
	      else
		if (gane<0)
		  printf ("Ganaste\n");  
	    
	    }
	    else
	      if (!obtenerTotalBarcos()){
		termino=0;
		printf ("PERDISTE\n");
	      }
	    informarExito (exitoAtaque);
	    mostrarTableros();
	}
		cerrarSesion();
    }
    exit(0);
}



char* ingresarCoordenaDeAtaque(coord* coordA)
{
    printf("\n\n Ingrese la cordenada de ataque:\n");
    *coordA=*ingresarCoordenada();
    buf[0]=(char)(((int)'0')+coordA->c1);
    buf[1]='-';
    buf[2]=(char)(((int)'0')+coordA->c2);
    buf[4]='\0';
    return buf;
}

coord* ingresarCoordenada (){
  int fila,columna;
  coord* c=(coord*)malloc(sizeof(coord));
  int validar=0;
  while (!validar){
    printf("[Batalla Naval]$");
    printf("Ingrese la fila(0-9):");
    validar=scanf("%d", &fila);
    if (!validar)
      printf("Opcion invalida, debes ingresar un numero\n");
    else
      if (fila<0 || fila>9){
	printf("Opcion invalida, la fila debe estar entre  el rango [0-9]\n");
	validar=0;
      }
      else
	validar=1;
  }
  validar=0;
  while (!validar){
    printf("[Batalla Naval]$");
    printf("Ingrese la columna(0-9):");
    validar=scanf("%d", &columna);
    if (!validar)
      printf("Opcion invalida, debes ingresar un numero\n");
    else
      if (columna<0 || columna>9){
	printf("Opcion invalida, la columna debe estar entre  el rango [0-9]\n");
	validar=0;
      }
      else
	validar=1;
  }
  c->c1=fila;
  c->c2=columna;
  return c;
}

int ingresarOrientacion (){
  char orientacion;
  int seguir =1;
  int resu=-1;
  while (seguir==1){
    printf("[Batalla Naval]$");
    printf("Ingrese la orienteación:(v)vertical-(h)horizontal:");
    fflush(stdin);
    scanf(" %c", &orientacion);
    if (orientacion=='v'){
      resu= 1;
      seguir=0;
    }
    else{
      if (orientacion=='h'){
	 resu=0;
	 seguir=0;
      }
      else
	printf ("\nOpcion invalida \n");
    }
  }
  return resu;
}

void insertarBarcoEnTablero (int tamano, int barco){
  coord* c;
  int orientacion;
  int resu=1;
  printf("Se insertara un barco de longitud %i\n",tamano);
  while (resu){
    c=ingresarCoordenada();
    orientacion=ingresarOrientacion();
    resu=insertarBarco (c,orientacion,barco);
    if (resu==1)
      printf("Imposible insertar el barco en esa coordenada ya que el barco queda fuera del rango del tablero \n");
    else
      if (resu==2)
	printf("Imposible insertar el barco en esa coordenada ya que el barco colisiona con otro \n");
  }
  printf("##### Barco %i puesto en flota con exito #####\n",barco);
}

void CargarTablero (){
  printf("Se procedera a cargar el tablero:\n\n");
  mostrarTableros();
  //cargamos el barco 1
  insertarBarcoEnTablero(5,1);
  mostrarTableros();
  //cargamos el barco 2
  /*insertarBarcoEnTablero(4,2);
  mostrarTableros();
  //cargamos el barco 3
  insertarBarcoEnTablero(4,3);
  mostrarTableros();
  //cargamos el barco 4
  insertarBarcoEnTablero(3,4);
  mostrarTableros();
  //cargamos el barco 5
  insertarBarcoEnTablero(3,5);
  mostrarTableros();
  //cargamos el barco 6
  insertarBarcoEnTablero(2,6);
  mostrarTableros();
  //cargamos el barco 7
  insertarBarcoEnTablero(2,7);
  mostrarTableros();
  //cargamos el barco 8
  insertarBarcoEnTablero(2,8);
  mostrarTableros();
  //cargamos el barco 9
  insertarBarcoEnTablero(2,9);*/
  printf("Todo los  barcos fueron puesto en flota con exito\n");
  mostrarTableros();
}

void informarExito (int exito){
  switch (exito){
    case (0): printf("Fallaste\n");break;
    case (1): printf("Le diste\n");break;
    case (2): printf("Hundiste el barco\n");break;
  }
}

coord* mensajeAcoordenada (char* mensaje){
  coord* c=(coord*)malloc(sizeof(coord));
  c->c1=*mensaje-(int)'0';
  c->c2=*(mensaje+2)-(int)'0';
  return c;
}


int verificarGanar(){
    return (obtenerTotalBarcosEnemigos()-obtenerTotalBarcos());
}

