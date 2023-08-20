

#ifndef CONFIGURAR_VEHICULOS_H
#define CONFIGURAR_VEHICULOS_H
#include "Tipos_Optimizacion.h"
#ifdef __cplusplus
extern "C" {
#endif
	int Identificar_Vehiculos(Elementos_Electrolinera* Informacion_Sistema, const Celda*** Data_Vehiculos,
		                      const int* Filas_CSV_Vehiculos, const Celda*** Data_Terminales,
		                      Puntos_Optimizacion* Array_Puntos_Simulacion);
	int Inicializar_Cargas_Electrolinera(Elementos_Electrolinera** Informacion_Sistema,const int Numero_Puntos);
#ifdef __cplusplus
}
#endif
#endif
