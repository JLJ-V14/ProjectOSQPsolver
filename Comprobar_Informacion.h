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
	void Comprobar_Restricciones_Sistema(Restricciones_Electrolinera* Restricciones);
	void Comprobar_Precios(const Elementos_Electrolinera* Informacion_Sistema, Puntos_Optimizacion* Array_Puntos_Simulacion);
	void Comprobar_Vector_L_U(const c_float* Vector_L, const  c_float* Vector_U, const Elementos_Electrolinera* Informacion_Sistema);
	//void Comprobar_Array_Puntos_Adicionales(Puntos_Adicionales* Array_Puntos_Adicionales)
#ifdef __cplusplus
}
#endif

#endif