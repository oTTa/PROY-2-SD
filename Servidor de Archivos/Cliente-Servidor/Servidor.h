/*
 * Please do not edit this file.
 * It was generated using rpcgen.
 */

#ifndef _SERVIDOR_H_RPCGEN
#define _SERVIDOR_H_RPCGEN

#include <rpc/rpc.h>


#ifdef __cplusplus
extern "C" {
#endif


typedef struct {
	u_int stream_t_len;
	char *stream_t_val;
} stream_t;

struct nombreContenido {
	char *contenido;
	char *nombre;
};
typedef struct nombreContenido nombreContenido;

struct nombreVersion {
	int v;
	char *nombre;
};
typedef struct nombreVersion nombreVersion;

struct argumento {
	char nombre[100];
	int ver;
	stream_t file;
};
typedef struct argumento argumento;

#define ServersFile 0x20000000
#define ServersV1 1

#if defined(__STDC__) || defined(__cplusplus)
#define ESCRIBIRVERSION 1
extern  void * escribirversion_1(argumento *, CLIENT *);
extern  void * escribirversion_1_svc(argumento *, struct svc_req *);
#define GETVERSION 2
extern  stream_t * getversion_1(argumento *, CLIENT *);
extern  stream_t * getversion_1_svc(argumento *, struct svc_req *);
#define modificarArchivo 3
extern  int * modificararchivo_1(nombreContenido *, CLIENT *);
extern  int * modificararchivo_1_svc(nombreContenido *, struct svc_req *);
#define getArchivo 4
extern  char ** getarchivo_1(nombreVersion *, CLIENT *);
extern  char ** getarchivo_1_svc(nombreVersion *, struct svc_req *);
#define listarArchivos 5
extern  char ** listararchivos_1(void *, CLIENT *);
extern  char ** listararchivos_1_svc(void *, struct svc_req *);
extern int serversfile_1_freeresult (SVCXPRT *, xdrproc_t, caddr_t);

#else /* K&R C */
#define ESCRIBIRVERSION 1
extern  void * escribirversion_1();
extern  void * escribirversion_1_svc();
#define GETVERSION 2
extern  stream_t * getversion_1();
extern  stream_t * getversion_1_svc();
#define modificarArchivo 3
extern  int * modificararchivo_1();
extern  int * modificararchivo_1_svc();
#define getArchivo 4
extern  char ** getarchivo_1();
extern  char ** getarchivo_1_svc();
#define listarArchivos 5
extern  char ** listararchivos_1();
extern  char ** listararchivos_1_svc();
extern int serversfile_1_freeresult ();
#endif /* K&R C */

/* the xdr functions */

#if defined(__STDC__) || defined(__cplusplus)
extern  bool_t xdr_stream_t (XDR *, stream_t*);
extern  bool_t xdr_nombreContenido (XDR *, nombreContenido*);
extern  bool_t xdr_nombreVersion (XDR *, nombreVersion*);
extern  bool_t xdr_argumento (XDR *, argumento*);

#else /* K&R C */
extern bool_t xdr_stream_t ();
extern bool_t xdr_nombreContenido ();
extern bool_t xdr_nombreVersion ();
extern bool_t xdr_argumento ();

#endif /* K&R C */

#ifdef __cplusplus
}
#endif

#endif /* !_SERVIDOR_H_RPCGEN */
