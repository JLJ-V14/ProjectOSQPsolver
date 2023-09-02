#include "time.h"
#include "Tipos_Optimizacion.h"
#include <stdbool.h>
#include <stdlib.h>

#define COL_CSV_ANYO   0
#define COL_CSV_MES    1
#define COL_CSV_DIA    2
#define COL_CSV_HORA   3
#define COL_CSV_PRECIO 4

//Este subprograma se encarga de encontrar que precio del Kilovatio hora corrresponde a cada punto de la simulacion
int Coincide_Hora(const struct tm Fecha_1, const struct tm Fecha_2) {
	struct tm Fecha_Primera;
	struct tm Fecha_Segunda;
	//Creo nuevas Fechas para no tener en cuenta los minutos para ver si una fecha es igual, superior o inferior
	//a otra ya que el precio del Kilovatio hora cambia por horas.
	Fecha_Primera.tm_year = Fecha_1.tm_year;
	Fecha_Primera.tm_mon = Fecha_1.tm_mon;
	Fecha_Primera.tm_mday = Fecha_1.tm_mday;
	Fecha_Primera.tm_hour = Fecha_1.tm_hour;
	Fecha_Primera.tm_min = 0;
	Fecha_Primera.tm_sec = 0;

	
	Fecha_Segunda.tm_year = Fecha_2.tm_year;
	Fecha_Segunda.tm_mon  = Fecha_2.tm_mon;
	Fecha_Segunda.tm_mday = Fecha_2.tm_mday;
	Fecha_Segunda.tm_hour = Fecha_2.tm_hour;
	Fecha_Segunda.tm_min  = 0;
	Fecha_Segunda.tm_sec  = 0;

	//Se convierte la fecha a un numero simple para hacer la comparacion mas simple
	long Tiempo_1 = Fecha_Primera.tm_year * 1000000 + Fecha_Primera.tm_mon * 100000 + Fecha_Primera.tm_mday * 100 + Fecha_Primera.tm_hour;
	long Tiempo_2 = Fecha_Segunda.tm_year * 1000000 + Fecha_Segunda.tm_mon * 100000 + Fecha_Segunda.tm_mday * 100 + Fecha_Segunda.tm_hour;

	if (Tiempo_1 > Tiempo_2) {
		return 1; //La fecha 1 es mas tardia que la segunda
	}
	else if (Tiempo_1 < Tiempo_2) {
		return -1; //La fecha 1 es mas temprana que la segunda.
	}
	else {
		return 0; //Las dos fechas son iguales.
	}
}
double Encontrar_Precio(const struct tm Fecha_Punto,const Celda ***Data_Precio, const int Numero_Filas_CSV) {
	// Subprograma para encontrar el precio correspondiente al punto de simulacion en cuestion
	int Numero_Horas = Numero_Filas_CSV - 1; //El numero de Horas es el numero de filas del CSV de los precios menos 1
	int Index_Bajo = 1;
	int Index_Alto = Numero_Horas + 1;

	printf("La fecha a encontrar es \n");
	printf("%d \n", Fecha_Punto.tm_year);
	printf("%d \n", Fecha_Punto.tm_mon);
	printf("%d \n", Fecha_Punto.tm_mday);
	printf("%d \n", Fecha_Punto.tm_hour);
	struct tm Fecha_Array; //Se crea una variable para comprobar que fecha del array corresponde.
	while (Index_Bajo <= Index_Alto) {
		int Index_Medio = Index_Bajo + (Index_Alto - Index_Bajo) / 2;
		printf("El index medio es %d \n", Index_Medio);
		Fecha_Array.tm_year = Data_Precio[Index_Medio][COL_CSV_ANYO]->data.dbl-1900;
		Fecha_Array.tm_mon = Data_Precio[Index_Medio][COL_CSV_MES]->data.dbl-1;
		Fecha_Array.tm_mday = Data_Precio[Index_Medio][COL_CSV_DIA]->data.dbl;
		Fecha_Array.tm_hour = Data_Precio[Index_Medio][COL_CSV_HORA]->data.dbl;
		Fecha_Array.tm_min = 0;
		Fecha_Array.tm_sec = 0;

		printf("La fecha del array es \n");
		printf("%d \n", Fecha_Array.tm_year);
		printf("%d \n", Fecha_Array.tm_mon);
		printf("%d \n", Fecha_Array.tm_mday);
		printf("%d \n", Fecha_Array.tm_hour);
		printf("%d \n", Fecha_Array.tm_min);
		printf("%d \n", Fecha_Array.tm_sec);

		int Resultado = Coincide_Hora(Fecha_Punto, Fecha_Array);
		if (Resultado == 0) {
			printf("El precio retornado es %lf \n", Data_Precio[Index_Medio][COL_CSV_PRECIO]);
			return Data_Precio[Index_Medio][COL_CSV_PRECIO]->data.dbl;
		}
		if (Resultado < 0) 
		Index_Alto = Index_Medio -1;
		else Index_Bajo = Index_Medio + 1;
	}
	printf("Error Precio no encontrado \n");
	return -1;
}
void Configurar_Precios(const Datos_CSV*** Data_Precio_Compra,const Datos_CSV *** Data_Precio_Venta,Puntos_Optimizacion *Array_Puntos_Simulacion,
	                    const Elementos_Electrolinera *Electrolinera,const int Filas_CSV_Compra, const int Filas_CSV_Venta) {
	//Se van añadiendo los precios de compra y venta a los puntos de sinmulacion correspondiente.
	double Precio;
	for (int i = 0; i < Electrolinera->Numero_Puntos; i++) {
		Precio = Encontrar_Precio(Array_Puntos_Simulacion[i].Fecha,Data_Precio_Compra,Filas_CSV_Compra);
		printf("El precio es %f \n", Precio);
		Array_Puntos_Simulacion[i].Precio_Compra = Precio;
		Precio = Encontrar_Precio(Array_Puntos_Simulacion[i].Fecha, Data_Precio_Venta, Filas_CSV_Venta);
		Array_Puntos_Simulacion[i].Precio_Venta = Precio;
	}
}