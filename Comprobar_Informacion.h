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
	void Escribir_CSV(const char* Nombre_Archivo, Celda*** data, int Filas, int Columnas);
	void Escribir_Array_Puntos_Simulacion(Puntos_Optimizacion* Array_Puntos_Simulacion, const int* Numero_Puntos);
	void Comprobar_Elementos_Electrolinera(Elementos_Electrolinera* Informacion_Sistema);
	//void Comprobar_Array_Puntos_Adicionales(Puntos_Adicionales* Array_Puntos_Adicionales)
#ifdef __cplusplus
}
#endif

#endif