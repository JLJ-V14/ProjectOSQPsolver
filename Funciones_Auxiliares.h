
//Aqui se definen funciones que se van a utilizar por los diferentes archivos.c del codigo
#ifndef FUNCIONES_AUXILIARES_H
#define FUNCIONES_AUXILIARES_H

/* Include files */

#include <stddef.h>
#include <stdlib.h>

#ifdef __cplusplus
extern "C" {
#endif
	int Comprobar_Orden_Fechas(const struct tm Fecha_1, const struct tm Fecha_2);
	

#ifdef __cplusplus
}
#endif

#endif