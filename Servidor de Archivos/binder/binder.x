program binder {
	version binderv1 {
		int registrarse(string) =1;
		string getIpRegistradas() =2;
		int eliminarIp(string)=3;
		string update()=4;
		int getVersionaEscribir(string) =5;
	} = 1;
} = 0x20000024;