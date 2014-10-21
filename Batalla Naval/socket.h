#define MSG_SIZE         4 //tamaño del mensaje
#define MAXBUF           64

//verifico si hay un contrincante ya conectado y si es asi se inicia como cliente
int contrincanteConectado (char *hostname, char * port);

// Envio un mensaje al contrincante
void enviarMsj (char* msj);

//Recibo un mensaje del contrincante
char* recibirMsj ();

//Inicio una conexion como servidor para que luego se conecte el contrincante
void iniciarServicio ();

//cerrar la conexion con el contrincante
int cerrarSesion ();