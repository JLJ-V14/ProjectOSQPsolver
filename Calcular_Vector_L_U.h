
#pragma once
#ifndef CALCULAR_L_U_H
#define CALCULAR_L_U_H

/* Include files */

#include <stddef.h>
#include <stdlib.h>
#include "osqp.h"
#include "Tipos_Optimizacion.h"

#ifdef __cplusplus
extern "C" {
#endif
	    void Configurar_Vector_L_U(c_float* Vector_L, c_float* Vector_U, Elementos_Electrolinera* Informacion_Sistema,
		const Restricciones_Electrolinera* Restriccion_Sistema);

	/* Function Declarations */


#ifdef __cplusplus
}
#endif

#endif