

#ifndef CONFIGURAR_VEHICULOS_H
#define CONFIGURAR_VEHICULOS_H
#include "Tipos_Optimizacion.h"
#ifdef __cplusplus
extern "C" {
#endif
	int Identificar_Vehiculos(Elementos_Electrolinera* Informacion_Sistema, const Datos_CSV *** Data_Vehiculos,
		                      const int* Filas_CSV_Vehiculos, const Datos_CSV *** Data_Terminales,
		                      Puntos_Optimizacion* Array_Puntos_Simulacion);
	int   Inicializar_Cargas_Electrolinera(Elementos_Electrolinera** Informacion_Sistema,const int Numero_Puntos);
	int   Busqueda_Binaria(Puntos_Optimizacion* Array_Puntos_Simulacion, const int size, const struct tm Fecha_Objetivo);
	int   Obtener_Numero_Terminal(const Datos_CSV *** Data_Vehiculos, const int N_Fila_CSV);
	char  Obtener_Fase_Electrica(const Datos_CSV*** Data_Terminales, const int Numero_Terminal);
	

#ifdef __cplusplus
}
#endif
#endif
