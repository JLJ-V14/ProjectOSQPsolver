
#include "Tipos_Optimizacion.h"
#include <time.h>

//En este archivo se encuentran los subprogramas que verifican que la fecha de inicio del algoritmo es correcta->

 void Cargar_Fecha_Inicial_Algoritmo(const Datos_CSV*** Datos_Tiempo, struct tm* Fecha_Inicial) {
	//Este subprograma se utiliza para cargar en la variable Fecha_Inicial la fecha inicial 
	Fecha_Inicial->tm_year = Datos_Tiempo[FILA_INFORMACION_ALGORITMO][COLUMNA_ANYO_INICIAL_ALGORITMO]->data.dbl - OFFSET_ANYO_tm;
	Fecha_Inicial->tm_mon = Datos_Tiempo[FILA_INFORMACION_ALGORITMO][COLUMNA_MES_INICIAL_ALGORITMO]->data.dbl - OFFSET_MES_tm;
	Fecha_Inicial->tm_mday = Datos_Tiempo[FILA_INFORMACION_ALGORITMO][COLUMNA_DIA_INICIAL_ALGORITMO]->data.dbl;
	Fecha_Inicial->tm_hour = Datos_Tiempo[FILA_INFORMACION_ALGORITMO][COLUMNA_HORA_INICIAL_ALGORITMO]->data.dbl;
	Fecha_Inicial->tm_min = Datos_Tiempo[FILA_INFORMACION_ALGORITMO][COLUMNA_MINUTO_INICIAL_ALGORITMO]->data.dbl;
	Fecha_Inicial->tm_sec = 0;
}

 void Cargar_Fecha_Final_Algoritmo(const Datos_CSV*** Datos_Tiempo, struct tm* Fecha_Final) {
	//Este subprograma se utiliza para cargar la fecha final en la variable fecha final
	Fecha_Final->tm_year = Datos_Tiempo[FILA_INFORMACION_ALGORITMO][COLUMNA_ANYO_FINAL_ALGORITMO]->data.dbl - OFFSET_ANYO_tm;
	Fecha_Final->tm_mon = Datos_Tiempo[FILA_INFORMACION_ALGORITMO][COLUMNA_MES_FINAL_ALGORITMO]->data.dbl - OFFSET_MES_tm;
	Fecha_Final->tm_mday = Datos_Tiempo[FILA_INFORMACION_ALGORITMO][COLUMNA_DIA_FINAL_ALGORITMO]->data.dbl;
	Fecha_Final->tm_hour = Datos_Tiempo[FILA_INFORMACION_ALGORITMO][COLUMNA_HORA_FINAL_ALGORITMO]->data.dbl;
	Fecha_Final->tm_min = Datos_Tiempo[FILA_INFORMACION_ALGORITMO][COLUMNA_MINUTO_FINAL_ALGORITMO]->data.dbl;
	Fecha_Final->tm_sec = 0;
}

 int Verificar_Tiempo_Algoritmo(const Datos_CSV*** Datos_Tiempo) {

	//Este subprograma se utiliza para verificar que los datos del CSV donde vienen los datos temporales de la
	//simulacion correctos, la fecha de inicio del algoritmo, la fecha de finalizacion. Asi como la diferencia en
	//minutos entre los puntos de la simulacion.


	//Defino una serie de variables auxiliares 
	struct tm Fecha_Inicial;
	struct tm Fecha_Final;
	//Cargo la fecha inicial y final de la simulacion.
	Cargar_Fecha_Inicial_Algoritmo(Datos_Tiempo, &Fecha_Inicial);
	Cargar_Fecha_Final_Algoritmo(Datos_Tiempo, &Fecha_Final);
	//Compruebo si la fecha inicial y final tienen sentido

	int Resultado_Comparacion = Comprobar_Orden_Fechas(Fecha_Inicial, Fecha_Final);

	if (Resultado_Comparacion > 0) {
		printf("Error La fecha inicial del algoritmo no puede ser posterior a la fecha final \n");
		return ERROR;
	}
	if (Resultado_Comparacion == 0) {
		printf("Error la fecha inicial del algoritmo no puede ser igual a la fecha final \n");
		return ERROR;
	}

	//Se comprueba si la diferencia en minutos entre los puntos de la simulacion es correcto->

	int Delta_Simulacion;

	Delta_Simulacion = Datos_Tiempo[FILA_INFORMACION_ALGORITMO][COLUMNA_DELTA_T];

	//La diferencia en minutos entre los puntos de la simulacion ha de ser entre 1 y 60.
	if ((Delta_Simulacion < 1) || (Delta_Simulacion > 60)) {
		printf("Error el delta de simulacion ha de estar entre 1 y 60 minutos \n");
		return ERROR;
	}

	return EXITO;
}
