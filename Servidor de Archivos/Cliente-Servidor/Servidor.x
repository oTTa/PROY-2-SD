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
		int modificarArchivo(nombreContenido) = 3;
		string getArchivo(nombreVersion) =4;
		string listarArchivos ()  = 5;
	}=1;
}=0x20000000;  
