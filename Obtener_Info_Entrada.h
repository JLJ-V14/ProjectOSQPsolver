#pragma once
#pragma once
#ifndef OBTENER_INFO_ENTRADA_H
#define OBTENER_INFO_ENTRADA_H

/* Include files */
#include <stddef.h>
#include <stdlib.h>
#include "Tipos_Optimizacion.h"
#include "osqp.h"
#ifdef __cplusplus
extern "C" {
#endif
	Celda *** Leer_CSV(const char* Nombre_Archivo, int* Filas, int* Columnas);
	void freeCSV(Celda*** data, int filas,int columnas);
	int Obtener_Restricciones_Red_Electrolinera(Restricciones_Electrolinera* Restricciones);
	int Leer_Tipos_Objetivos(Objetivos* Objetivos_Optimizacion);
	int Obtener_Informacion_Electrolinera(Sistema_Carga* Electrolinera, const Caracteristicas_Simulacion Tiempo_Simulacion);
	int Configurar_Tiempo_Simulacion(Caracteristicas_Simulacion* Tiempo_Simulacion);
	int Obtener_Datos_Panel_Fotovoltaico(Panel_Solar* Paneles_Solares, const Caracteristicas_Simulacion Caracteristicas_Tiempo);
	int Obtener_Datos_Cargas(Carga_Adicional* Carga, const Caracteristicas_Simulacion Caracteristicas_Tiempo);
	int Leer_Precios(double** Precio_compra, double** Precio_venta, const int Puntossimulacion);
	int Leer_Datos_Operador(Operador_Red** operador, const int Numhoras);
	int Leer_Baterias(Bateria* Baterias_Sistema);
	int Leer_Informacion(Objetivos* Objetivos_Optimizacion, Restricciones_Electrolinera* Restricciones_Potencia,
		Caracteristicas_Simulacion* Tiempo_Algoritmo, Sistema_Carga* Electrolinera,
		Panel_Solar* Datos_Panel_Fotovoltaico, Carga_Adicional* Datos_Carga, c_float* Precio_Compra,
		c_float* Precio_Venta, Operador_Red* Demanda_Operador, Bateria* Baterias_Sistema);

#ifdef __cplusplus
}
#endif

#endif