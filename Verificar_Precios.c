#include "Definiciones_Globales.h"
#include "Funciones_Auxiliares.h"
#include "Tipos_Optimizacion.h"
#include <stdio.h>
#include <time.h>


//----------------Revisado------------------------------------------------------------------------------

//----------------En este archivo se definen las funciones que se utilizan para comprobar que las entradas
//----------------del precio son validas-----------------------------------------------------------


static int Comprobar_Valor_Precio(const Datos_CSV ***Datos_Precio, const int Numero_Fila) {

	double Precio = Datos_Precio[Numero_Fila][COLUMNA_CSV_PRECIO]->data.dbl;

	if (Precio < 0) {
		printf("El precio inicial no puede ser inferior a 0 \n");
		return ERROR;
	}
	return EXITO;
}

static int Comprobar_Precio_Inicial(const Datos_CSV*** Datos_Precio, const Datos_CSV ***Datos_Algoritmo) {

	//Este subprograma se utiliza para comprobar que la hora inicial del algoritmo no es anterior a la primera 
	//fecha del CSV de los precios. Para asegurar que no hay horar que no tienen precio asignado

	struct tm Fecha_Inicial_Precio    = { 0 };
	struct tm Fecha_Inicial_Algoritmo = { 0 };

	//Se carga la fecha inicial del algoritmo->
	Cargar_Fecha_Sin_Minuto(Datos_Algoritmo, &Fecha_Inicial_Algoritmo, COLUMNA_ANYO_INICIAL_ALGORITMO,
		                    COLUMNA_MES_INICIAL_ALGORITMO,COLUMNA_DIA_INICIAL_ALGORITMO,COLUMNA_HORA_INICIAL_ALGORITMO,
		                    FILA_INFORMACION_ALGORITMO);


	

	//Se carga la fecha inicial del precio.
	Cargar_Fecha_Sin_Minuto(Datos_Precio, &Fecha_Inicial_Precio, COLUMNA_CSV_ANYO_PRECIO, COLUMNA_CSV_MES_PRECIO,
		                    COLUMNA_CSV_DIA_PRECIO, COLUMNA_CSV_HORA_PRECIO, PRIMERA_FILA_CSV_PRECIO);


	//Se comprueba que el algoritmo no empieza antes de tener datos de precio.
	if (Verificar_Orden_Fechas(Fecha_Inicial_Precio,Fecha_Inicial_Algoritmo) == ERROR) {
		printf("Error la fecha inicial del algoritmo empieza antes que los precios disponibles \n");
		return ERROR;
	}

	//Se pasa a comprobar que el precio inicial sea superior a 0

	if (Comprobar_Valor_Precio(Datos_Precio, PRIMERA_FILA_CSV_PRECIO) == ERROR) {
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
	struct tm Fecha_Final_Precio = { 0 };
	//Se define el indice de la fila final del CSV donde se encuentra el ultimo precio disponible.
	int Fila_Final = Numero_Filas_CSV - 1;

	Cargar_Fecha_Sin_Minuto(Datos_Tiempo, &Fecha_Final_Algoritmo, COLUMNA_ANYO_FINAL_ALGORITMO,
		                    COLUMNA_MES_FINAL_ALGORITMO, COLUMNA_DIA_FINAL_ALGORITMO,
		                    COLUMNA_HORA_FINAL_ALGORITMO, FILA_INFORMACION_ALGORITMO);
	
	
	Cargar_Fecha_Sin_Minuto(Datos_Precio, &Fecha_Final_Precio, COLUMNA_ANYO_FINAL_ALGORITMO,
		                    COLUMNA_MES_FINAL_ALGORITMO, COLUMNA_DIA_FINAL_ALGORITMO,
		                    COLUMNA_HORA_FINAL_ALGORITMO, Fila_Final);


	//Se comprueba que se tienen los datos de precios suficientes, que no hay hora sin precio.

	if (Verificar_Orden_Fechas(Fecha_Final_Algoritmo, Fecha_Final_Precio) == ERROR) {
		printf("Error la fecha inicial del algoritmo empieza antes que los precios disponibles \n");
		return ERROR;
	}

	//Se comprueba que el precio no es negativo.
	if (Comprobar_Valor_Precio(Datos_Precio, Numero_Filas_CSV-1) == ERROR) {
		return ERROR;
	}

	return EXITO;
}

static int Comprobar_Consecutividad_Horas(struct tm Fecha_1, struct tm Fecha_2) {

	//Este subprograma se utiliza para comprobar que los precios del CSV están en horas consecutivas

	time_t Fecha_1_Normalizada = mktime(&Fecha_1);
	time_t Fecha_2_Normalizada = mktime(&Fecha_2);
	double Diferencia_Tiempo = difftime(Fecha_1_Normalizada, Fecha_2_Normalizada);

	if (Diferencia_Tiempo != SEGUNDOS_HORA) {
		printf("Los precios no son consecutivos \n");
		return ERROR;
	}

	return EXITO;
}
static int Comprobar_Precio(const Datos_CSV*** Datos_Precio, const int Fila_Csv_Primer_Precio,
	                        const int Fila_Csv_Segundo_Precio) {

	//Este subprograma comprueba que la fecha que viene en la fila Numero_Fila_Fecha_Primera del CSV de los precios
	//solo se diferencia de la segunda en una hora, esto se utiliza para comprobar que las fechas del CSV de los precios
	//son consecutivas por horas.

	//Cargo las fechas a comprobar
	struct tm Fecha_1 = { 0 };
	struct tm Fecha_2 = { 0 };

	//Se carga la primera fecha a comprobar
	Cargar_Fecha_Sin_Minuto(Datos_Precio,&Fecha_1, COLUMNA_CSV_ANYO_PRECIO, COLUMNA_CSV_MES_PRECIO, 
		                    COLUMNA_CSV_DIA_PRECIO,COLUMNA_CSV_HORA_PRECIO, Fila_Csv_Primer_Precio);



	//Se carga la segunda fecha a comprobar
	Cargar_Fecha_Sin_Minuto(Datos_Precio, &Fecha_2, COLUMNA_CSV_ANYO_PRECIO, COLUMNA_CSV_MES_PRECIO,
		                   COLUMNA_CSV_DIA_PRECIO, COLUMNA_CSV_HORA_PRECIO, Fila_Csv_Segundo_Precio);

	
	if (Comprobar_Consecutividad_Horas(Fecha_1, Fecha_2) == ERROR) {
		printf("Los precios deben estar en horas consecutivas \n");
		return ERROR;
	}

	//Se comprueba que el valor del precio no sea negativo->

	if (Comprobar_Valor_Precio(Datos_Precio, Fila_Csv_Primer_Precio) == ERROR) {
		printf("Error precio negativo en la fila %d", Fila_Csv_Primer_Precio);
		return ERROR;
	}

	if (Comprobar_Valor_Precio(Datos_Precio, Fila_Csv_Segundo_Precio) == ERROR) {
		printf("Error precio negativo en la fila %d", Fila_Csv_Segundo_Precio);
		return ERROR;
	}

	return EXITO;

}


static int Comprobar_Precios_CSV(const Datos_CSV*** Datos_Precio, const int Numero_Filas_Precio) {
	//Se utiliza este subprograma para comprobar que las fechas de los diferentes precios son consecutivos, 
	//y que los valores de los precios son superiores o igual a 0.

	for (int Numero_Fila_CSV = 1; Numero_Fila_CSV < Numero_Filas_Precio - 1; Numero_Fila_CSV++) {
		if (Comprobar_Precio(Datos_Precio, Numero_Fila_CSV, Numero_Fila_CSV + 1) == ERROR) {
			return ERROR;
		}

	}
	return EXITO;
}



 int Comprobar_Precios(const Datos_CSV*** Datos_Precio_Compra, const Datos_CSV*** Datos_Precio_Venta,
	const int Numero_Filas_Precio_Compra, const int Numero_Filas_Precio_Venta,
	const Datos_CSV*** Datos_Tiempo_Algoritmo) {

	//En este subprograma se comprueba que los precios de compra del kilovatio hora y de venta del kilovatio hora
	//son correctos

	

	if ((Numero_Filas_Precio_Compra <= NUMERO_MINIMO_FILAS_CSV_PRECIO)) {
        printf("No hay datos suficientes en el CSV que contiene los precios de compra \n");
		return ERROR;
	}

	if (Numero_Filas_Precio_Venta <= NUMERO_MINIMO_FILAS_CSV_PRECIO) {
		printf("No hay datos suficientes en el CSV que contiene los precios de venta \n");
		return ERROR;
	}

	//Se pasa a comprobar si los datos de los CSV de los precios de compra y de venta tienen sentido

	if (Comprobar_Precio_Inicial(Datos_Precio_Compra, Datos_Tiempo_Algoritmo) == ERROR) {
		return ERROR;
	}

	if (Comprobar_Precio_Inicial(Datos_Precio_Venta, Datos_Tiempo_Algoritmo) == ERROR) {
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
