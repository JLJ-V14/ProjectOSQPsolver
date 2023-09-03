
#ifndef  VERIFICAR_BATERIAS_H
#define VERIFICAR_BATERIAS_H

/* Include files */

#include "Tipos_Optimizacion.h"

#ifdef __cplusplus
extern "C" {
#endif
	int Verificar_Baterias(const Datos_CSV*** Datos_CSV_Baterias, const Datos_CSV*** Datos_CSV_Algoritmo,
		const int Numero_Filas_CSV_Baterias);

#ifdef __cplusplus
}
#endif

#endif