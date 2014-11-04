typedef char stream_t<>;

struct argumento
{
	char nombre[100];
	int ver;
	stream_t file;
};

program SERVERARCHIVOS {
	version VERSION {
		void ESCRIBIRVERSION(argumento)=4;
		stream_t GETVERSION(argumento)=5;
	}=1;
}=0x24929807;