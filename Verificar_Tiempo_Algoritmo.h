

#ifndef VERIFICAR_TIEMPO_ALGORITMO_H
#define VERIFICAR_TIEMPO_ALGORITMO_H

/* Include files */

#include "Tipos_Optimizacion.h"

#ifdef __cplusplus
extern "C" {
#endif

	int Verificar_Tiempo_Algoritmo(const Datos_CSV*** Datos_Tiempo);
	void Cargar_Fecha_Inicial_Algoritmo(const Datos_CSV*** Datos_Tiempo, struct tm* Fecha_Inicial);
	void Cargar_Fecha_Final_Algoritmo(const Datos_CSV*** Datos_Tiempo, struct tm* Fecha_Final);
	/* Function Declarations */
	

#ifdef __cplusplus
}
#endif

#endif
