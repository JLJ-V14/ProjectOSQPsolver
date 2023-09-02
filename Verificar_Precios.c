#include "Definiciones_Globales.h"
#include "Funciones_Auxiliares.h"
#include "Tipos_Optimizacion.h"


//----------------En este archivo se definen las funciones que se utilizan para comprobar que las entradas
//----------------del precio son validas.



static int Comprobar_Precio_Inicial(const Datos_CSV*** Datos_Precio, const Datos_CSV ***Datos_Algoritmo,
	const int Numero_Filas_Precio) {

	//Este subprograma se utiliza para comprobar que la hora inicial del algoritmo no es anterior a la primera 
	//fecha del CSV de los precios. Para asegurar que no hay horar que no tienen precio asignado

	struct tm Fecha_Inicial_Precio    = { 0 };
	struct tm Fecha_Inicial_Algoritmo = { 0 };

	//Se carga la fecha inicial del algoritmo->
	
	Fecha_Inicial_Algoritmo.tm_year = Datos_Algoritmo[FILA_INFORMACION_ALGORITMO][COLUMNA_CSV_PRECIO]->data.dbl - OFFSET_ANYO_tm;
	Fecha_Inicial_Algoritmo.tm_mon  = Datos_Algoritmo[FILA_INFORMACION_ALGORITMO][COLUMNA_CSV_PRECIO]->data.dbl - OFFSET_MES_tm;
	Fecha_Inicial_Algoritmo.tm_mday = Datos_Algoritmo[FILA_INFORMACION_ALGORITMO][COLUMNA_CSV_PRECIO]->data.dbl;
	Fecha_Inicial_Algoritmo.tm_hour = Datos_Algoritmo[FILA_INFORMACION_ALGORITMO][COLUMNA_CSV_PRECIO]->data.dbl;
	Fecha_Inicial_Algoritmo.tm_min  = 0;
	Fecha_Inicial_Algoritmo.tm_sec  = 0;

	//Se carga la fecha inicial del precio.
	Fecha_Inicial_Precio.tm_year = Datos_Precio[PRIMERA_FILA_CSV_PRECIO][COLUMNA_CSV_ANYO_PRECIO] - OFFSET_ANYO_tm;
	Fecha_Inicial_Precio.tm_mon  = Datos_Precio[PRIMERA_FILA_CSV_PRECIO][COLUMNA_CSV_MES_PRECIO] - OFFSET_MES_tm;
	Fecha_Inicial_Precio.tm_mday = Datos_Precio[PRIMERA_FILA_CSV_PRECIO][COLUMNA_CSV_DIA_PRECIO];
	Fecha_Inicial_Precio.tm_hour = Datos_Precio[PRIMERA_FILA_CSV_PRECIO][COLUMNA_CSV_HORA_PRECIO];
	Fecha_Inicial_Precio.tm_min = 0;
	Fecha_Inicial_Precio.tm_sec = 0;

	int Resultado_Comparacion = Comprobar_Orden_Fechas(Fecha_Inicial_Algoritmo, Fecha_Inicial_Precio);

	if (Resultado_Comparacion < 0) {
		printf("La primera fecha del CSV de los precios no puede ser anterior a la fecha inicial del algoritmo \n");
		return ERROR;
	}

	//Se pasa a comprobar que el precio inicial sea superior a 0
	double Precio = Datos_Precio[PRIMERA_FILA_CSV_PRECIO][COLUMNA_CSV_PRECIO]->data.dbl;

	if (Precio < 0) {
		return ERROR;
	}

	return EXITO;
}


static int Comprobar_Precio_Final(const Datos_CSV*** Datos_Precio, const int Numero_Filas_CSV,
	const Datos_CSV*** Datos_Tiempo) {

	//En este subprograma se comprueba que el ultimo precio del CSV o bien coincide con la fecha final del
	//algoritmo o bien es una fecha posterior. Para asegurar que esta cubierto
	//la fecha final de ejecucion del algoritmo en el CSV de lor precios.

	struct tm Fecha_Final_Algoritmo = { 0 };
	Fecha_Final_Algoritmo.tm_year = Datos_Tiempo[FILA_INFORMACION_ALGORITMO][COLUMNA_ANYO_INICIAL_ALGORITMO]->data.dbl - OFFSET_ANYO_tm;
	Fecha_Final_Algoritmo.tm_mon = Datos_Tiempo [FILA_INFORMACION_ALGORITMO][COLUMNA_MES_INICIAL_ALGORITMO]->data.dbl - OFFSET_MES_tm;
	Fecha_Final_Algoritmo.tm_mday = Datos_Tiempo[FILA_INFORMACION_ALGORITMO][COLUMNA_DIA_INICIAL_ALGORITMO]->data.dbl;
	Fecha_Final_Algoritmo.tm_hour = Datos_Tiempo[FILA_INFORMACION_ALGORITMO][COLUMNA_HORA_INICIAL_ALGORITMO]->data.dbl;
	Fecha_Final_Algoritmo.tm_min = 0;
	Fecha_Final_Algoritmo.tm_sec = 0;

	struct tm Fecha_Final_Precio = { 0 };
	Fecha_Final_Precio.tm_year = Datos_Precio[Numero_Filas_CSV - 1][COLUMNA_ANYO_FINAL_ALGORITMO]->data.dbl - OFFSET_ANYO_tm;
	Fecha_Final_Precio.tm_mon =  Datos_Precio[Numero_Filas_CSV - 1][COLUMNA_MES_FINAL_ALGORITMO]->data.dbl - OFFSET_MES_tm;
	Fecha_Final_Precio.tm_mday = Datos_Precio[Numero_Filas_CSV - 1][COLUMNA_DIA_FINAL_ALGORITMO]->data.dbl;
	Fecha_Final_Precio.tm_hour = Datos_Precio[Numero_Filas_CSV - 1][COLUMNA_HORA_FINAL_ALGORITMO]->data.dbl;
	Fecha_Final_Precio.tm_min = 0;
	Fecha_Final_Precio.tm_sec = 0;

	//Se comprueba que se tienen los datos de precios suficientes, que no hay hora sin precio.
	int Resultado_Comparacion = Comprobar_Orden_Fechas(Fecha_Final_Algoritmo, Fecha_Final_Precio);
	if (Resultado_Comparacion > 0) {
		printf("Faltan datos de precios \n");
		return ERROR;
	}

	//Se comprueba que el precio no es negativo.
	double Precio = Datos_Precio[Numero_Filas_CSV - 1][COLUMNA_CSV_PRECIO]->data.dbl;

	if (Precio < 0) {
		printf("No puede haber precios negativos \n");
		return ERROR;
	}

	return EXITO;
}

static int Comprobar_Precio(const Datos_CSV*** Datos_Precio, const int Numero_Fila_Fecha_Primera,
	const int Numero_Fila_Fecha_Segunda) {

	//Este subprograma comprueba que la fecha que viene en la fila Numero_Fila_Fecha_Primera del CSV de los precios
	//solo se diferencia de la segunda en una hora, esto se utiliza para comprobar que las fechas del CSV de los precios
	//son consecutivas por horas.

	//Cargo las fechas a comprobar
	struct tm Fecha_1 = { 0 };
	struct tm Fecha_2 = { 0 };

	//Se carga la primera fecha a comprobar
	Fecha_1.tm_year = Datos_Precio[Numero_Fila_Fecha_Primera][COLUMNA_CSV_ANYO_PRECIO]->data.dbl - OFFSET_ANYO_tm;
	Fecha_1.tm_mon = Datos_Precio[Numero_Fila_Fecha_Primera][COLUMNA_CSV_MES_PRECIO]->data.dbl - OFFSET_MES_tm;
	Fecha_1.tm_mday = Datos_Precio[Numero_Fila_Fecha_Primera][COLUMNA_CSV_DIA_PRECIO]->data.dbl;
	Fecha_1.tm_hour = Datos_Precio[Numero_Fila_Fecha_Primera][COLUMNA_CSV_HORA_PRECIO]->data.dbl;
	Fecha_1.tm_min = 0;
	Fecha_1.tm_sec = 0;

	//Se carga la segunda fecha a comprobar
	Fecha_2.tm_year = Datos_Precio[Numero_Fila_Fecha_Segunda][COLUMNA_CSV_ANYO_PRECIO]->data.dbl - OFFSET_ANYO_tm;
	Fecha_2.tm_mon = Datos_Precio[Numero_Fila_Fecha_Segunda][COLUMNA_CSV_MES_PRECIO]->data.dbl - OFFSET_MES_tm;
	Fecha_2.tm_mday = Datos_Precio[Numero_Fila_Fecha_Segunda][COLUMNA_CSV_DIA_PRECIO]->data.dbl;
	Fecha_2.tm_hour = Datos_Precio[Numero_Fila_Fecha_Segunda][COLUMNA_CSV_HORA_PRECIO]->data.dbl;
	Fecha_2.tm_min = 0;
	Fecha_2.tm_sec = 0;

	time_t Fecha_1_Normalizada = mktime(&Fecha_1);
	time_t Fecha_2_Normalizada = mktime(&Fecha_2);
	double Diferencia_Tiempo = difftime(Fecha_1_Normalizada, Fecha_2_Normalizada);

	if (Diferencia_Tiempo != SEGUNDOS_HORA) {
		printf("Los precios no son consecutivos \n");
		return ERROR;
	}

	//Se comprueba que el valor del precio no sea negativo->
	double Precio = Datos_Precio[Numero_Fila_Fecha_Segunda][COLUMNA_CSV_PRECIO]->data.dbl;
	if (Precio < 0) {
		printf("No puede haber precios negativos \n");
		return ERROR;
	}

	return EXITO;

}


static int Comprobar_Precios_CSV(const Datos_CSV*** Datos_Precio, const int Numero_Filas_Precio) {
	//Se utiliza este subprograma para comprobar que las fechas de los diferentes precios son consecutivos, 
	//y que los valores de los precios son superiores o igual a 0.

	for (int Numero_Fila_CSV = 1; Numero_Fila_CSV < Numero_Filas_Precio - 1; Numero_Fila_CSV++) {
		Comprobar_Precio(Datos_Precio, Numero_Fila_CSV, Numero_Fila_CSV + 1);

	}
}



 int Comprobar_Precios(const Datos_CSV*** Datos_Precio_Compra, const Datos_CSV*** Datos_Precio_Venta,
	const int Numero_Filas_Precio_Compra, const int Numero_Filas_Precio_Venta,
	const Datos_CSV*** Datos_Tiempo_Algoritmo) {

	//En este subprograma se comprueba que los precios de compra del kilovatio hora y de venta del kilovatio hora
	//son correctos

	

	if ((Numero_Filas_Precio_Compra <= 1) || (Numero_Filas_Precio_Venta <= 1)) {
		printf("No hay datos suficientes en el CSV que contiene los precios del kilovatio hora");
		return ERROR;
	}


	//Se pasa a comprobar si los datos de los CSV de los precios de compra y de venta tienen sentido

	if (Comprobar_Precio_Inicial(Datos_Precio_Compra, Numero_Filas_Precio_Compra,Numero_Filas_Precio_Compra) == ERROR) {
		return ERROR;
	}

	if (Comprobar_Precio_Inicial(Datos_Precio_Venta, Datos_Tiempo_Algoritmo, Numero_Filas_Precio_Venta) == ERROR) {
		return ERROR;
	}

	//Se pasa a comprobar que todos los precios tengan fechas consecutivas una hora despues de la otra, y que 
	//no haya precios negativos.
	if (Comprobar_Precios_CSV(Datos_Precio_Compra, Numero_Filas_Precio_Compra) == ERROR) {
		return ERROR;
	}

	if (Comprobar_Precios_CSV(Datos_Precio_Venta, Numero_Filas_Precio_Venta) == ERROR) {
		return ERROR;
	}

	//Se pasa a comprobar que la fecha asociada del precio final se corresponde con la fecha final del algoritmo
	//o bien, que la fecha final del precio no esta por debajo de la fecha final del algoritmo
	if (Comprobar_Precio_Final(Datos_Precio_Compra, Numero_Filas_Precio_Compra, Datos_Tiempo_Algoritmo) == ERROR) {
		return ERROR;
	}

	if (Comprobar_Precio_Final(Datos_Precio_Venta, Numero_Filas_Precio_Venta, Datos_Tiempo_Algoritmo) == ERROR) {
		return ERROR;
	}
	//Se comprueba que en el CSV de los precios esta incluido la fecha final del algoritmo

	return EXITO;
}
