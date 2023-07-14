#pragma once
#pragma once
#pragma once
#ifndef CALCULAR_MATRIZ_P_H
#define CALCULAR_MATRIZ_P_H

/* Include files */

#include <stddef.h>
#include <stdlib.h>
#include <stdbool.h>
#include "Tipos_Optimizacion.h"
#include "osqp.h"
#ifdef __cplusplus
extern "C" {
#endif
	int Configurar_Objetivo_Precio(const Objetivos* Array_Objetivos, const Caracteristicas_Simulacion* Tiempo_Simulacion, const c_float Coeficiente_Precio,
		c_float* Array_Optimizacion_Precios, const c_float* Precio_Compra, const c_float* Precio_Venta);
	int Configurar_Referencia_Operador_Lineal(const Objetivos* Array_Objetivos, c_float* Array_Referencia_Operador,
		const Operador_Red* Referencia_Operador, const Caracteristicas_Simulacion* Tiempo_Simulacion);
	/* Function Declarations */


#ifdef __cplusplus
}
#endif

#endif