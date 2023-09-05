#pragma once
#ifndef CONFIGURAR_BATERIAS_H
#define CONFIGURAR_BATERIAS_H
#include "Tipos_Optimizacion.h"
#ifdef __cplusplus
extern "C" {
#endif
	int Identificar_Baterias(Elementos_Electrolinera* Informacion_Sistema, const Datos_CSV *** Data_Baterias, const int Filas_CSV_Baterias,
		const Datos_CSV *** Data_Terminales, Puntos_Optimizacion* Array_Puntos_Simulacion);
#ifdef __cplusplus
}
#endif
#endif
