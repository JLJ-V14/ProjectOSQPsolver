#pragma once
#ifndef OPTIMIZACION_CARGA_H
#define OPTIMIZACION_CARGA_H

//Incluir Archivos.
#include "Calcular_Vector_L_U.h"
#include "Tipos_Optimizacion.h"
#include "osqp.h"

#ifdef __cplusplus
extern "C" {
#endif

	void Optimizacion_Cuadratica(const Elementos_Electrolinera* Informacion_Sistema,
		const Restricciones_Electrolinera* Restricciones_Sistema);
#ifdef __cplusplus
}
#endif

#endif
