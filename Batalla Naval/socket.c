#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>
#include <string.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include "socket.h"

int sockAddrSize;
int clntSocket, miSocket, hostPort; //manejador para el socket
struct hostent * hostInfo; //guarda la información del host
struct sockaddr_in direccion; //struct para la info del socket

//verifico si hay un contrincante ya conectado
int contrincanteConectado (char * nombreHost, char * puerto){
    
    char hostName[255];
    clntSocket = 0;
    strcpy(hostName, nombreHost);
    hostPort = atoi(puerto);

    sockAddrSize = sizeof (struct sockaddr_in); 

    //creo el socket orientado a conexión (TCP)
    miSocket = socket(AF_INET, SOCK_STREAM, 0);
    if (miSocket == -1)
    {
        printf("Error al crear el socket\n");
        exit(-1);
    }
    //obtengo informacion del host a partir del hostname
    if ((hostInfo = gethostbyname(hostName)) == NULL)
    {
        printf("IP invalida \n.");
        exit(-1);
    }

    direccion.sin_addr = *((struct in_addr *) hostInfo->h_addr);//direccion del server 
    direccion.sin_port = htons(hostPort); //puerto donde se hara la comunicacion, htons usa un lenguaje independiente de la arquitectura del cpu
    direccion.sin_family = AF_INET; //es el tipo de conexión (por red o interna)
    //Solicitar conexión con el servidor , Dicha función connect quedará bloqueada hasta que el servidor acepte nuestra conexión o bien si no hay servidor en el sitio indicado, saldrá dando un error retornando -1
    if (connect(miSocket, (struct sockaddr*) & direccion, sizeof (direccion)) == -1)
       return 0; 
    else{
       printf("Soy el cliente y establecí una comunicación con %s en el puerto %d\n\n", hostName, hostPort);
       return 1;
    }
}

// Envio un mensaje al contrincante
void enviarMsj (char* msj){
    char msg[MSG_SIZE];
    strcpy(msg, msj);
    if (clntSocket)//Entra cuando es servidor
	send(clntSocket, msg, MSG_SIZE, 0);
    else //Entra cuando es cliente
	send(miSocket, msg, MSG_SIZE, 0);
}

//Recibo un mensaje del contrincante
char* recibirMsj (){
    char * msg = malloc(MSG_SIZE);
    if (clntSocket) //Entra cuando es servidor
	 recv(clntSocket, msg, MSG_SIZE, 0);
    else //Entra cuando es cliente
        recv(miSocket, msg, MSG_SIZE, 0);
    return msg;
}

//Inicio una conexion como servidor 
void iniciarServicio (char * port){
    //inicia en modo servidor
    sockAddrSize = sizeof (struct sockaddr_in); 
    //creo el socket orientado a conexión (TCP)
    miSocket = socket(AF_INET, SOCK_STREAM, 0);

    if (miSocket == -1) {
        printf("No se puede crear el socket\n");
        exit(-1);
    }
    hostPort = atoi(port);

    direccion.sin_addr.s_addr = INADDR_ANY; //acepta cualquier cliente
    direccion.sin_port = htons(hostPort);//puerto donde se hara la comunicacion, htons usa un lenguaje independiente de la arquitectura del cpu
    direccion.sin_family = AF_INET; //es el tipo de conexión (por red o interna)

    //Avisar al sistema operativo de que hemos abierto un socket y queremos que asocie nuestro programa(puerto del mismo) a dicho socket
    if (bind(miSocket, (struct sockaddr*) & direccion, sizeof (direccion)) == -1) {
        printf("Error al intentar asociar un puerto con un socket (bind) \n");
        exit(-1);
    }

    //Avisar al sistema de que comience a atender dicha conexión de red.
    if (listen(miSocket, 1) == -1) {
        printf("Error el intentar escuchar en el puerto %i\n",hostPort);
        exit(-1);
    }

    printf("Soy el servidor y escucho en el puerto %i\n\n",hostPort);
    //Pedir y aceptar las conexiones de clientes al sistema operativo.
    clntSocket = accept(miSocket, (struct sockaddr*) & direccion, (socklen_t *) & sockAddrSize);
  
}

//cerrar la conexion con el amigo
int cerrarSesion (){
  printf ("Se finalizara la conexion...\n");  
  //verifico la existe del socket cliente
  if (clntSocket)
    {
      // cerrar la conexión del descriptor de socket cliente
        if (close(clntSocket) == -1)
        {
            printf("No se puede cerrar la sesion\n");
            exit(-1);
    	}
   }
   //verifico la existencia del socket servidor
   if (miSocket)
   {
        // cerrar la conexión del descriptor de socket servidor
        if (close(miSocket) == -1)
        {
            printf("No se puede cerrar la sesion\n");
            exit(-1);
    	}
    }
  printf ("conexion Finalizada\n");  
}
