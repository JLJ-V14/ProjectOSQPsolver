#pragma once

#ifndef VERIFICAR_DATOS_VEHICULOS_H
#define VERIFICAR_DATOS_VEHICULOS_H

/* Include files */

#include <stddef.h>
#include <stdlib.h>
#include "Tipos_Optimizacion.h"
#ifdef __cplusplus
extern "C" {
#endif
	int Verificar_Informacion_Vehiculos(const Datos_CSV*** Datos_Vehiculos, const int Numero_Filas_CSV_Vehiculos,
		                                const Datos_CSV*** Datos_Algoritmo);
	

#ifdef __cplusplus
}
#endif

#endif