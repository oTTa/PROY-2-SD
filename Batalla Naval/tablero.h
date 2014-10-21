typedef struct coord{
      int c1;
      int c2;
} coord;

void inicializarTablero();

//0 exito
//1 fuera de rango
//2 colision con otro barco
int insertarBarco(coord* c,int orientacion, int barco);

void mostrarTableros();

//0 si no hay barco
//1 si acertaste al barco
//2 si acertaste al barco y lo hundiste
//3 si acertaste al barco y ganaste
int recibirAtaque(coord *c);

void informarAtaque(coord *c, int exito);

int obtenerTotalBarcosEnemigos();

int obtenerTotalBarcos();