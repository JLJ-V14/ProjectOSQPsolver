
#ifndef VERIFICAR_PRECIOS_H
#define VERIFICAR_PRECIOS_H

/* Include files */

#include "Tipos_Optimizacion.h"

#ifdef __cplusplus
extern "C" {
#endif

	    int Comprobar_Precios(const Datos_CSV*** Datos_Precio_Compra, const Datos_CSV*** Datos_Precio_Venta,
		const int Numero_Filas_Precio_Compra, const int Numero_Filas_Precio_Venta,
		const Datos_CSV*** Datos_Tiempo_Algoritmo);

#ifdef __cplusplus
}
#endif

#endif