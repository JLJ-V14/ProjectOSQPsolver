
#ifndef CONFIGURAR_AJUSTES_PROBLEMA_H
#define CONFIGURAR_AJUSTES_PROBLEMA_H
#include "osqp.h"
#include "Tipos_Optimizacion.h"
#include <stdbool.h>
#ifdef __cplusplus
extern "C" {
#endif
	//void Configurar_Ajustes(OSQPSettings* settings);
	int  Configurar_Puntos_Simulacion(const    Datos_CSV *** Data_Vehiculos, Datos_CSV *** Data_Tiempo, 
    Puntos_Optimizacion** Array_Puntos_Simulacion, const int *Filas_CSV_Vehiculo,int *Numero_Puntos_Vehiculo);
	int  Calcular_Numero_Vehiculos   (const    Datos_CSV *** Data_Vehiculos, const int* N_Filas_CSV);
	void Configurar_Ajustes(OSQPSettings *settings);
	bool Comprobar_Vehiculo(const char* Dispositivo);
	bool Fechas_Iguales(const struct tm Fecha_1, const struct tm Fecha_2);
#ifdef __cplusplus
}
#endif
#endif
