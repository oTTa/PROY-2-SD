
struct nombreContenido
{
  char* contenido;
  char* nombre;
};


struct nombreVersion
{
  int v;
  char* nombre;
};

program ej2 {
	version ej2v1 {
		int crearArchivo (string) = 1;
		int modificarArchivo(nombreContenido) = 2;
		long tamanoArchivo(string) = 3;
		string getArchivo(nombreVersion) =4;
		string listarArchivos ()  = 5;

	} = 1;
} = 0x20000001;