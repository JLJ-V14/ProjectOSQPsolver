#pragma once
#pragma once
#include "osqp.h"
#include "Tipos_Optimizacion.h"
#ifndef  CONFIGURARPROBLEMA_H
#define  CONFIGURARPROBLEMA_H
#ifdef __cplusplus
extern "C" {
#endif

	void Configurar_Problema_Optimizacion(OSQPSettings* Settings, OSQPData* Data, const Sistema_Carga* Electrolinera, const Caracteristicas_Simulacion* Tiempo_Simulacion,
		const c_float* Precio_Compra, const c_float* Precio_Venta, const Operador_Red* Referencia_Operador, const int* Array_Objetivos,
		const Bateria* Baterias_Sistema, const Panel_Solar* Datos_Paneles, const Carga_Adicional* Cargas_Sistema, const Restricciones_Electrolinera* Restricciones);

#ifdef __cplusplus
}
#endif

#endif