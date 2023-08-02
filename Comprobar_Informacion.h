#pragma once
#ifndef COMPROBAR_INFORMACION_H
#define COMPROBAR_INFORMACION_H

/* Include files */

#include <stddef.h>
#include <stdlib.h>
#include "Tipos_Optimizacion.h"
#ifdef __cplusplus
extern "C" {
#endif

	/* Function Declarations */
	void Escribir_CSV_Vehiculos(const char* Nombre_Archivo, Celda*** data, int Filas, int* Columnas);

#ifdef __cplusplus
}
#endif

#endif