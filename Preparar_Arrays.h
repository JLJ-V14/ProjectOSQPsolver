#pragma once
#pragma once

#ifndef PREPARAR_ARRAYS_H
#define PREPARAR_ARRAYS_H

/* Include files */
#include "Tipos_Optimizacion.h"


#ifdef __cplusplus
extern "C" {
#endif

	/* Function Declarations */
	int  Crear_Array_Operador(Operador_Red** Referencia_operador, const int num_fases, const int NumHoras);
	void Liberar_Memoria_Estaciones(Sistema_Carga* Sistema);
	void Liberar_Memoria_Tiempo_Simulacion(Caracteristicas_Simulacion* Tiempo_Simulacion);
	void Liberar_Memoria_Referencia_Operador(Operador_Red* Operador);
	void Liberar_Memoria_Carga(Carga_Adicional* Carga);
	void Liberar_Memoria_Panel_Solar(Panel_Solar* Panel_Solar);
	void Liberar_Toda_Memoria(Panel_Solar* Panel_Solar, Carga_Adicional* Carga, Operador_Red* Operador,
		Caracteristicas_Simulacion* Tiempo_Simulacion, Sistema_Carga* Sistema);
	void Calcular_Hora_Actual(const Caracteristicas_Simulacion* Tiempo_Simulacion, int* Hora_Actual,
		int* Index_Hora, const int Punto_Simulacion);
#ifdef __cplusplus
}
#endif

#endif
