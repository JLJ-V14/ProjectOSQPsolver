// Juan López Jódar Últimos Cambios : 5 / 09 / 2023
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
#include "Optimizacion_Carga.h"          //Header file que incluye la funcion principal de optimizacion
#include "osqp.h"                        //Header file que incluye las funciones del algoritmo de optimizacion
#include "Tipos_Optimizacion.h"          //Header file que incluye los tipos propios definidos para el algoritmo de optimizacion 
#include "time.h"



void main_optimizacionC(void) {
	//FUNCIONAMIENTO DEL CODIGO:
	//PRIMERO SE LEE LA INFORMACION DEL SISTEMA, POSTERIORMENTE SE CONFIGURAN LAS MATRICES, Y FINALMENTE SE EJECUTA LA FUNCION PRINCIPAL DE OPTIMIZACION.
	//POSTERIORMENTE SE EXTRAEN LOS RESULTADOS.

	//-----------Se definen variables donde se van a almacenar los datos de los ficheros CSV que contienen la------//
	//-----------informacion de entrada del algoritmo--------------------------------------------------------------//

    Datos_CSV***                                       Datos_Terminales;     //Informacion de los terminales.
	Datos_CSV***                                       Datos_Vehiculos;      //Informacion de los vehiculos
	Datos_CSV***                                       Datos_Tiempo;         //Datos de tiempo de la simulacion
	Datos_CSV***                                       Datos_Restricciones;  //Restricciones del sistema
	Datos_CSV***                                       Datos_Precio_Compra;  //Precios de compra del kW/h
	Datos_CSV***                                       Datos_Precio_Venta;   //Precios de venta  del kW/h
	Datos_CSV***                                       Datos_Baterias;       //Datos   de las baterias del sistema

	//-----------Se definen variables para almacenar el numero de filas y columnas que tienen los diferentes CSVs.
	int* Filas_Terminales       =          (int*)malloc(sizeof(int));
	int* Columnas_Terminales    =          (int*)malloc(sizeof(int));
	int* Filas_Vehiculos        =          (int*)malloc(sizeof(int));
	int* Columnas_Vehiculos     =          (int*)malloc(sizeof(int));
	int* Filas_Tiempo           =          (int*)malloc(sizeof(int));
	int* Columnas_Tiempo        =          (int*)malloc(sizeof(int));
	int* Filas_Restricciones    =          (int*)malloc(sizeof(int));
	int* Columnas_Restricciones =          (int*)malloc(sizeof(int));
	int* Filas_Precio_Compra    =          (int*)malloc(sizeof(int));
	int* Columnas_Precio_Compra =          (int*)malloc(sizeof(int));
	int* Filas_Precio_Venta     =          (int*)malloc(sizeof(int));
	int* Columnas_Precio_Venta  =          (int*)malloc(sizeof(int));
	int* Filas_Baterias         =          (int*)malloc(sizeof(int));
	int* Columnas_Baterias      =          (int*)malloc(sizeof(int));

	
	//Se definen 4 variables      :
	
	//Informacion_Electrolinera   : Informacion de los vehiculos y baterias que estan conectados a la electrolinera
	//Restricciones_Electrolinera : Limitaciones a las que se ve sometida el sistema
	//Array_Puntos_Simulacion     : Array que contiene los puntos de simulacion y las fechas asociadas a los mismos.
	//Numero_Puntos_Simulacion    : Numero de puntos que contiene la simulacion.

	Elementos_Electrolinera*                           Informacion_Electrolinera;
	Restricciones_Electrolinera                        Restricciones_Sistema;
	Puntos_Optimizacion*                               Array_Puntos_Simulacion;
	int                                                Numero_Puntos_Simulacion;
	 
	//Se crea una serie de variables para leer los datos almacenados en los CSV.
	//Se inicializan estas variables->
	Informacion_Electrolinera  = NULL;
	Numero_Puntos_Simulacion   = 1;
	Array_Puntos_Simulacion    = NULL;

	//Reservo memoria para la variable que almacena los datos de los vehiculos, y de las posibles cargas
	//conectadas al sistema->
	
	
	
	//Se Lee la informacion de los CSV de entrada.
	Datos_Terminales    = Leer_CSV("Informacion_Terminales.csv",  Filas_Terminales,Columnas_Terminales);
	Datos_Vehiculos     = Leer_CSV("Informacion_Vehiculos.csv",   Filas_Vehiculos,Columnas_Vehiculos);
	Datos_Baterias      = Leer_CSV("Informacion_Baterias.csv",    Filas_Baterias, Columnas_Baterias);
	Datos_Tiempo        = Leer_CSV("Informacion_Tiempo.csv",      Filas_Tiempo, Columnas_Tiempo);
	Datos_Restricciones = Leer_CSV("Restricciones_Sistema.csv",   Filas_Restricciones, Columnas_Restricciones);
	Datos_Precio_Compra = Leer_CSV("Precio_Compra_Kilovatio.csv", Filas_Precio_Compra, Columnas_Precio_Compra);
	Datos_Precio_Venta =  Leer_CSV("Precio_Venta_Kilovatio.csv",  Filas_Precio_Venta, Columnas_Precio_Venta);



	
	//Se llama al subprograma que se encarga de configurar cuantos puntos de calculo tiene el algoritmo, asi como
	//asociar a cada punto de la simulacion una fecha real. 
	if (Configurar_Puntos_Simulacion(Datos_Vehiculos, Datos_Tiempo,&Array_Puntos_Simulacion,Filas_Vehiculos,&Numero_Puntos_Simulacion)==-1) {
		printf("Error Configurando los puntos de la simulacion \n");
		free(Array_Puntos_Simulacion);
	}


	//Inicializo la variable que almacena los datos de los vehiculos del sistema->
	if (Inicializar_Cargas_Electrolinera(&Informacion_Electrolinera,Numero_Puntos_Simulacion) == -1) {
		free(Informacion_Electrolinera);
	}
	//Se almacenan los datos en la variable que guarda la informacion sobre el sistema.
	if (Identificar_Vehiculos(Informacion_Electrolinera,Datos_Vehiculos,Filas_Vehiculos,Datos_Terminales,
		                      Array_Puntos_Simulacion)==-1) {
		printf("Error asignando la informacion a los vehiculos \n");
		free(Informacion_Electrolinera);
	}
	//Se almacenan y leen los datos de las baterias que hay presentes en el sistema->
	if (Identificar_Baterias(Informacion_Electrolinera, Datos_Baterias, *Filas_Baterias, Datos_Terminales,
		Array_Puntos_Simulacion) == -1) {
		printf("Error asignando la informacion a las baterias \n");
		free(Informacion_Electrolinera);
	}
	
	//Se calculan las restricciones del sistema
	Calcular_Restricciones_Sistema(Datos_Restricciones, &Restricciones_Sistema);
	Configurar_Precios(Datos_Precio_Compra,Datos_Precio_Venta,Array_Puntos_Simulacion,Informacion_Electrolinera,
		*Filas_Precio_Compra, *Filas_Precio_Venta);
	
	
	Optimizacion_Cuadratica(Informacion_Electrolinera,&Restricciones_Sistema);
	Comprobar_Elementos_Electrolinera(Informacion_Electrolinera);
	Comprobacion_Vehiculos_Baterias(Informacion_Electrolinera);
	Comprobar_Restricciones_Sistema(&Restricciones_Sistema);
	Comprobar_Precios(Informacion_Electrolinera, Array_Puntos_Simulacion);

	Escribir_CSV("Comprobar_Terminales.csv",    Datos_Terminales,    *Filas_Terminales,    *Columnas_Terminales);
	Escribir_CSV("Comprobar_Vehiculos.csv",     Datos_Vehiculos,     *Filas_Vehiculos,     *Columnas_Vehiculos);
	Escribir_CSV("Comprobar_Tiempo.csv",        Datos_Tiempo,        *Filas_Tiempo,        *Columnas_Tiempo);
	Escribir_CSV("Comprobar_Restricciones.csv", Datos_Restricciones, *Filas_Restricciones, *Columnas_Restricciones);
	Escribir_CSV("Comprobar_Precio_Compra.csv", Datos_Precio_Compra, *Filas_Precio_Compra, *Columnas_Precio_Compra);
    Escribir_CSV("Comprobar_Precio_Venta.csv",  Datos_Precio_Venta,  *Filas_Precio_Venta,  *Columnas_Precio_Venta);
	Escribir_CSV("Comprobar_Baterias.csv",      Datos_Baterias,      *Filas_Baterias,      *Columnas_Baterias);
	//Compruebo el array de que almacena los puntos de simulacion->
	//Escribir_Array_Puntos_Simulacion(Array_Puntos_Simulacion, &Numero_Puntos_Simulacion);
	
	//Libero la memoria asociada al array que contiene los puntos de simulacion 
	free(Array_Puntos_Simulacion);
	
	//Libero la memoria asoicada a la variable que almacena los datos de los vehiculos->
	free(Informacion_Electrolinera->Vehiculos_Sistema);
	free(Informacion_Electrolinera->Baterias_Sistema);
	free(Informacion_Electrolinera);

	//Libero la memoria reservada para leer los datos almacenados en el excel.
	freeCSV(Datos_Terminales, *Filas_Terminales, *Columnas_Terminales);
	freeCSV(Datos_Vehiculos, *Filas_Vehiculos, *Columnas_Vehiculos);
	freeCSV(Datos_Tiempo, *Filas_Tiempo, *Columnas_Tiempo);
	freeCSV(Datos_Restricciones, *Filas_Restricciones, *Columnas_Restricciones);
	freeCSV(Datos_Precio_Compra, *Filas_Precio_Compra, *Columnas_Precio_Compra);
	freeCSV(Datos_Precio_Venta, *Filas_Precio_Venta,   *Columnas_Precio_Venta);

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