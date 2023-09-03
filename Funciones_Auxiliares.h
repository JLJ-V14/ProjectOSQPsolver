
//Aqui se definen funciones que se van a utilizar por los diferentes archivos.c del codigo
#ifndef FUNCIONES_AUXILIARES_H
#define FUNCIONES_AUXILIARES_H

/* Include files */

#include <stddef.h>
#include <stdlib.h>
#include "Tipos_Optimizacion.h"
#include <time.h>
#ifdef __cplusplus
extern "C" {
#endif
	int Comprobar_Orden_Fechas(const struct tm Fecha_1, const struct tm Fecha_2);
    
	void Cargar_Fecha(const Datos_CSV*** Datos_Entrada, struct tm* Fecha, const int Columna_Csv_Anyo,
		const int Columna_Csv_Mes, const int Columna_Csv_Dia, const int Columna_Csv_Hora,
		const int Columna_Csv_Minuto, const int Numero_Fila);
	
	void Cargar_Fecha_Sin_Minuto(const Datos_CSV*** Datos_Entrada, struct tm* Fecha, const int Columna_Csv_Anyo,
		const int Columna_Csv_Mes, const int Columna_Csv_Dia, const int Columna_Csv_Hora,
		const int Numero_Fila);
   
	int Verificar_Orden_Fechas(const struct tm Fecha_Anterior, const struct tm Fecha_Posterior);

#ifdef __cplusplus
}
#endif

#endif