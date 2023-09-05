#pragma once
#ifndef CONFIGURAR_PRECIOS_H
#define CONFIGURAR_PRECIOS_H
#include <stddef.h>
#include <stdlib.h>
#include <stdbool.h>
#include "Tipos_Optimizacion.h"
#ifdef __cplusplus
extern "C" {
#endif
	void Configurar_Precios(const Datos_CSV *** Data_Precio_Compra, const Datos_CSV *** Data_Precio_Venta, Puntos_Optimizacion* Array_Puntos_Simulacion,
		const Elementos_Electrolinera *Electrolinera, const int Filas_CSV_Compra, const int Filas_CSV_Venta);
#endif
#ifdef __cplusplus
}
#endif
