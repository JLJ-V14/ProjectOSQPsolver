// Juan López Jódar Últimos Cambios : 24 / 08 / 2023
//Incluyo una serie de archivos
#include <stdio.h>
#include <stdbool.h>
#include "main.h" 
#include "Comprobar_Informacion.h"       //Header file que incluye las funciones de comprobar la informacion
#include "Configurar_Ajustes_Problema.h" //Header file donde se incluye las funciones que calculan los ajustes.
#include "Configurar_Vehiculos.h"        //Header file donde se incluye las funciones de configurar a los vehiculos
#include "Configurar_Baterias.h"         //Header file donde se incluye las funciones de configurar a las baterias
#include "Configurar_Precios.h"          //Header file donde se incluye las funciones de configurar el precio del kWh
#include "Configurar_Restricciones.h"    //Header file donde se incluye las funciones de configurar las restricciones
#include "Obtener_Info_Entrada.h"        //Header file que incluye las funciones de leer la entrada.
#include "Optimizacion_Cuadratica.h"     //Header file que incluye la funcion principal de optimizacion
#include "osqp.h"                        //Header file que incluye las funciones del algoritmo de optimizacion
#include "Preparar_Arrays.h"             //Header file en el que van incluidas las funciones para reservar memorias para los arrays.s
#include "Tipos_Optimizacion.h"          //Header file que incluye los tipos propios definidos para el algoritmo de optimizacion 
#include "time.h"
#define  Num_Objetivos 5                 //Numero de objetivos = 5


void main_optimizacionC(void) {
	//FUNCIONAMIENTO DEL CODIGO:
	//PRIMERO SE LEE LA INFORMACION DEL SISTEMA, POSTERIORMENTE SE CONFIGURAN LAS MATRICES, Y FINALMENTE SE EJECUTA LA FUNCION PRINCIPAL DE OPTIMIZACION.
	//POSTERIORMENTE SE EXTRAEN LOS RESULTADOS

	Celda***                                       Data_Terminales;
	Celda***                                       Data_Vehiculos;
	Celda***                                       Data_Tiempo;
	Celda***                                       Data_Restricciones;
	Celda***                                       Data_Precio_Compra;
	Celda***                                       Data_Precio_Venta;
	Celda***                                       Data_Baterias;
	Restricciones_Electrolinera                    Restricciones_Sistema;
	//Variable que almacena la informacion de los elementos presentes en la electrolinera (Vehiculo, Baterias..)
	Elementos_Electrolinera*                       Informacion_Electrolinera;
	//Se crea una variable que es un array que sirve para relacionar los puntos de simulacion con, una fecha
	//asociada.
	//Variable para crear unas variable que sirve para almacenar el numero de puntos de simulacion_>
	int Numero_Puntos_Simulacion=0;
	//Se crea una serie de variables para leer los datos almacenados en los CSV.

	int* Filas_Terminales    =    (int*)malloc(sizeof(int));
	int* Columnas_Terminales =    (int*)malloc(sizeof(int));
	int* Filas_Vehiculos     =    (int*)malloc(sizeof(int));
	int* Columnas_Vehiculos  =    (int*)malloc(sizeof(int));
	int* Filas_Tiempo        =    (int*)malloc(sizeof(int));
	int* Columnas_Tiempo     =    (int*)malloc(sizeof(int));
	int* Filas_Restricciones =    (int*)malloc(sizeof(int));
	int* Columnas_Restricciones = (int*)malloc(sizeof(int));
	int* Filas_Precio_Compra =    (int*)malloc(sizeof(int));
	int* Columnas_Precio_Compra = (int*)malloc(sizeof(int));
	int* Filas_Precio_Venta =     (int*)malloc(sizeof(int));
	int* Columnas_Precio_Venta =  (int*)malloc(sizeof(int));
	int* Filas_Baterias =    (int*)malloc(sizeof(int));
	int* Columnas_Baterias = (int*)malloc(sizeof(int));
	
	Puntos_Optimizacion* Array_Puntos_Simulacion=NULL;
	//Reservo memoria para la variable que almacena los datos de los vehiculos, y de las posibles cargas
	//conectadas al sistema->
	 Informacion_Electrolinera = NULL;
	
	

	Data_Terminales    = Leer_CSV("Informacion_Terminales.csv",  Filas_Terminales,Columnas_Terminales);
	Data_Vehiculos     = Leer_CSV("Informacion_Vehiculos.csv",   Filas_Vehiculos,Columnas_Vehiculos);
	Data_Baterias      = Leer_CSV("Informacion_Baterias.csv",    Filas_Baterias, Columnas_Baterias);
	Data_Tiempo        = Leer_CSV("Informacion_Tiempo.csv",      Filas_Tiempo, Columnas_Tiempo);
	Data_Restricciones = Leer_CSV("Restricciones_Sistema.csv",   Filas_Restricciones, Columnas_Restricciones);
	Data_Precio_Compra = Leer_CSV("Precio_Compra_Kilovatio.csv", Filas_Precio_Compra, Columnas_Precio_Compra);
	Data_Precio_Venta =  Leer_CSV("Precio_Venta_Kilovatio.csv",  Filas_Precio_Venta, Columnas_Precio_Venta);


	
	if (Configurar_Puntos_Simulacion(Data_Vehiculos, Data_Tiempo,&Array_Puntos_Simulacion,Filas_Vehiculos,&Numero_Puntos_Simulacion)==-1) {
		printf("Error Configurando los puntos de la simulacion \n");
		free(Array_Puntos_Simulacion);
	}
	//Inicializo la variable que almacena los datos de los vehiculos del sistema->
	if (Inicializar_Cargas_Electrolinera(&Informacion_Electrolinera,Numero_Puntos_Simulacion) == -1) {
		free(Informacion_Electrolinera);
	}
	//Se almacenan los datos en la variable que guarda la informacion sobre el sistema.
	if (Identificar_Vehiculos(Informacion_Electrolinera,Data_Vehiculos,Filas_Vehiculos,Data_Terminales,
		                      Array_Puntos_Simulacion)==-1) {
		printf("Error asignando la informacion a los vehiculos \n");
		free(Informacion_Electrolinera);
	}
	//Se almacenan y leen los datos de las baterias que hay presentes en el sistema->
	if (Identificar_Baterias(Informacion_Electrolinera, Data_Baterias, *Filas_Baterias, Data_Terminales,
		Array_Puntos_Simulacion) == -1) {
		printf("Error asignando la informacion a las baterias \n");
		free(Informacion_Electrolinera);
	}
	
	//Se calculan las restricciones del sistema
	Calcular_Restricciones_Sistema(Data_Restricciones, &Restricciones_Sistema);
	Configurar_Precios(Data_Precio_Compra,Data_Precio_Venta,Array_Puntos_Simulacion,Informacion_Electrolinera,
		*Filas_Precio_Compra, *Filas_Precio_Venta);

	Comprobar_Elementos_Electrolinera(Informacion_Electrolinera);
	Comprobar_Restricciones_Sistema(&Restricciones_Sistema);
	Comprobar_Precios(Informacion_Electrolinera, Array_Puntos_Simulacion);

	Escribir_CSV("Comprobar_Terminales.csv",Data_Terminales,*Filas_Terminales,*Columnas_Terminales);
	Escribir_CSV("Comprobar_Vehiculos.csv", Data_Vehiculos, *Filas_Vehiculos, *Columnas_Vehiculos);
	Escribir_CSV("Comprobar_Tiempo.csv", Data_Tiempo, *Filas_Tiempo, *Columnas_Tiempo);
	Escribir_CSV("Comprobar_Restricciones.csv", Data_Restricciones, *Filas_Restricciones, *Columnas_Restricciones);
	Escribir_CSV("Comprobar_Precio_Compra.csv", Data_Precio_Compra, *Filas_Precio_Compra, *Columnas_Precio_Compra);
    Escribir_CSV("Comprobar_Precio_Venta.csv",  Data_Precio_Venta,  *Filas_Precio_Venta,  *Columnas_Precio_Venta);
	Escribir_CSV("Comprobar_Baterias.csv", Data_Baterias, *Filas_Baterias, *Columnas_Baterias);
	//Compruebo el array de que almacena los puntos de simulacion->
	//Escribir_Array_Puntos_Simulacion(Array_Puntos_Simulacion, &Numero_Puntos_Simulacion);
	
	//Libero la memoria asociada al array que contiene los puntos de simulacion 
	free(Array_Puntos_Simulacion);
	
	//Libero la memoria asoicada a la variable que almacena los datos de los vehiculos->
	free(Informacion_Electrolinera->Vehiculos_Sistema);
	free(Informacion_Electrolinera->Baterias_Sistema);
	free(Informacion_Electrolinera);

	//Libero la memoria reservada para leer los datos almacenados en el excel.
	freeCSV(Data_Terminales, *Filas_Terminales, *Columnas_Terminales);
	freeCSV(Data_Vehiculos, *Filas_Vehiculos, *Columnas_Vehiculos);
	freeCSV(Data_Tiempo, *Filas_Tiempo, *Columnas_Tiempo);
	freeCSV(Data_Restricciones, *Filas_Restricciones, *Columnas_Restricciones);
	freeCSV(Data_Precio_Compra, *Filas_Precio_Compra, *Columnas_Precio_Compra);
	freeCSV(Data_Precio_Venta, *Filas_Precio_Venta,   *Columnas_Precio_Venta);

	free(Filas_Terminales);
	free(Columnas_Terminales);
	free(Filas_Vehiculos);
	free(Columnas_Vehiculos);
	free(Filas_Tiempo);
	free(Columnas_Tiempo);
	free(Filas_Restricciones);
	free(Columnas_Restricciones);
	free(Filas_Precio_Compra);
	free(Columnas_Precio_Compra);
	free(Filas_Precio_Venta);
	free(Columnas_Precio_Venta);
	
}

int main(int argc, char** argv) {
	(void)argc;
	(void)argv;
	main_optimizacionC();   //Main de la optimizacion
	return 0;
}
//Lista de Índices de las variables:
//De 0 a Numero de coches *2
// 