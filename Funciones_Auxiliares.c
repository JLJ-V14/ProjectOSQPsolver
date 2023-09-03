#include "Funciones_Auxiliares.h"
#include "Tipos_Optimizacion.h"
#include "time.h"

//Este archivo contiene una serie de funciones que se utilizan a lo largo de todo el codigo->

int Comprobar_Orden_Fechas(const struct tm Fecha_1, const struct tm Fecha_2) {
	//Este subprograma tiene la funcion de comprobar si una fecha es anterior posterior o igual a la otra
	//Se devuelve -1 si la fecha 1 es anterior a la fecha 2
	//Se devuelve  0 si la fecha 1 es igual    a la fecha 2
	//Se devuelve  1 si la fecha 1 es posterior a la fecha 2

	struct tm Fecha_1_Aux = Fecha_1;
	struct tm Fecha_2_Aux = Fecha_2;
	time_t Tiempo_1, Tiempo_2;

    Tiempo_1 = mktime(&Fecha_1_Aux);
	Tiempo_2 = mktime(&Fecha_2_Aux);

	if (Tiempo_1 < Tiempo_2) {
		return -1;
	}
	else if (Tiempo_1 == Tiempo_2) {
		return 0;
	}
	else {
		return 1;
	}
}
int Verificar_Orden_Fechas (const struct tm Fecha_Anterior, const struct tm Fecha_Posterior) {
    //Este subprograma se utiliza para verificar que la fecha posterior es mas tarde que la fecha 
	//anterior si esto no sucede se devuelve un error.
	
	int Resultado_Comparacion = Comprobar_Orden_Fechas(Fecha_Anterior, Fecha_Posterior);
	if (Resultado_Comparacion < 0) {
		printf("Orden incorrecto de fechas \n");
		return ERROR;
	}
	return EXITO;
}

void Cargar_Fecha  (const Datos_CSV*** Datos_Entrada, struct tm* Fecha, const int Columna_Csv_Anyo,
	                const int Columna_Csv_Mes,    const int Columna_Csv_Dia, const int Columna_Csv_Hora,
	                const int Columna_Csv_Minuto, const int Numero_Fila) {
	
	//Este subprograma se utiliza para cargar la informacion de una fecha de un CSV.


	Fecha->tm_year = Datos_Entrada[Numero_Fila][Columna_Csv_Anyo]->data.dbl - OFFSET_ANYO_tm;
	Fecha->tm_mon  = Datos_Entrada[Numero_Fila][Columna_Csv_Mes]->data.dbl  - OFFSET_MES_tm;
	Fecha->tm_mday = Datos_Entrada[Numero_Fila][Columna_Csv_Dia]->data.dbl;
	Fecha->tm_hour = Datos_Entrada[Numero_Fila][Columna_Csv_Hora]->data.dbl;
	Fecha->tm_min  = Datos_Entrada[Numero_Fila][Columna_Csv_Minuto]->data.dbl;
	Fecha->tm_sec  = 0;
}

void Cargar_Fecha_Sin_Minuto(const Datos_CSV*** Datos_Entrada, struct tm* Fecha, const int Columna_Csv_Anyo,
	                         const int Columna_Csv_Mes, const int Columna_Csv_Dia, const int Columna_Csv_Hora,
	                         const int Numero_Fila) {

    //Este subprograma se utiliza para cargar la fecha de un CSV pero ignorando los minutos->
	Fecha->tm_year = Datos_Entrada[Numero_Fila][Columna_Csv_Anyo]->data.dbl - OFFSET_ANYO_tm;
	Fecha->tm_mon =  Datos_Entrada[Numero_Fila][Columna_Csv_Mes]->data.dbl - OFFSET_MES_tm;
	Fecha->tm_mday = Datos_Entrada[Numero_Fila][Columna_Csv_Dia]->data.dbl;
	Fecha->tm_hour = Datos_Entrada[Numero_Fila][Columna_Csv_Hora]->data.dbl;
	Fecha->tm_min = 0;
	Fecha->tm_sec = 0;

}
