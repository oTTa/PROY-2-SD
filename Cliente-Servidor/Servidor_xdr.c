/*
 * Please do not edit this file.
 * It was generated using rpcgen.
 */

#include "Servidor.h"

bool_t
xdr_stream_t (XDR *xdrs, stream_t *objp)
{
	register int32_t *buf;

	 if (!xdr_array (xdrs, (char **)&objp->stream_t_val, (u_int *) &objp->stream_t_len, ~0,
		sizeof (char), (xdrproc_t) xdr_char))
		 return FALSE;
	return TRUE;
}

bool_t
xdr_nombreContenido (XDR *xdrs, nombreContenido *objp)
{
	register int32_t *buf;

	 if (!xdr_string (xdrs, &objp->contenido, ~0))
		 return FALSE;
	 if (!xdr_string (xdrs, &objp->nombre, ~0))
		 return FALSE;
	return TRUE;
}

bool_t
xdr_nombreVersion (XDR *xdrs, nombreVersion *objp)
{
	register int32_t *buf;

	 if (!xdr_int (xdrs, &objp->v))
		 return FALSE;
	 if (!xdr_string (xdrs, &objp->nombre, ~0))
		 return FALSE;
	return TRUE;
}

bool_t
xdr_argumento (XDR *xdrs, argumento *objp)
{
	register int32_t *buf;

	int i;
	 if (!xdr_vector (xdrs, (char *)objp->nombre, 100,
		sizeof (char), (xdrproc_t) xdr_char))
		 return FALSE;
	 if (!xdr_int (xdrs, &objp->ver))
		 return FALSE;
	 if (!xdr_stream_t (xdrs, &objp->file))
		 return FALSE;
	return TRUE;
}
