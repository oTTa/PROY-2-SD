typedef char stream_t<>;

struct nombreContenido
{
  string contenido<>;
  string nombre<>;
};


struct nombreVersion
{
  int v;
  string nombre<>;
};

struct argumento
{
	char nombre[100];
	int ver;
	stream_t file;
};

program ServersFile {
	version ServersV1 {
		void ESCRIBIRVERSION(argumento)=1;
		stream_t GETVERSION(argumento)=2;
		int crearArchivo (string) = 3;
		int modificarArchivo(nombreContenido) = 4;
		string getArchivo(nombreVersion) =5;
		string listarArchivos ()  = 6;
	}=1;
}=0x20000000;  
