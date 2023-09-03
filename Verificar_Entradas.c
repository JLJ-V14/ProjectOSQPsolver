#include "Definiciones_Globales.h"
#include "Funciones_Auxiliares.h"
#include "time.h"
#include "Tipos_Optimizacion.h"
#include "Verificar_Datos_Vehiculos.h"
#include "Verificar_Precios.h"
#include "Verificar_Restricciones.h"
#include "Verificar_Terminales.h"
#include "Verificar_Tiempo_Algoritmo.h"
#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>
#include <sys/types.h>
#include <string.h>


int Verificar_Entradas( const Datos_CSV *** Datos_Tiempo,           const Datos_CSV  *** Datos_Vehiculos,
	                    const Datos_CSV *** Datos_Bateria,          const Datos_CSV *** Datos_Precio_Compra,
                        const Datos_CSV *** Datos_Precio_Venta,     const Datos_CSV *** Datos_Terminales,
	                    const Datos_CSV *** Datos_Restricciones,
	                    const int Numero_Filas_CSV_Tiempo,          const int Numero_Filas_CSV_Vehiculos,       
	                    const int Numero_Filas_CSV_Bateria,         const int Numero_Filas_Precio_Compra,       
	                    const int Numero_Filas_Precio_Venta,        const int Numero_Columnas_CSV_Tiempo,       
	                    const int Numero_Columnas_CSV_Vehiculos,    const int Numero_Columnas_CSV_Bateria,     
	                    const int Numero_Columnas_CSV_Precio_Compra,const int Numero_Columnas_CSV_Precio_Venta, 
	                    const int Numero_Filas_Terminales,          const int Numero_Filas_CSV_Restricciones
                        ) {

	//Se utiliza este subprograma para verificar que los datos de entrada que estan en los CSV tienen sentido
	//logico.

	//En este subprograma se verifica que los datos temporales de la simulacion tengan sentido->
	if (Verificar_Tiempo_Algoritmo(Datos_Tiempo)==ERROR) {
		printf("Los datos del CSV de la informacion temporal del algoritmo son incorrectos \n");
		return ERROR;
	}

	//En este subprograma se verifica que los datos de entrada de los diferentes CSV tengan sentido->
	if (Comprobar_Precios(Datos_Precio_Compra, Datos_Precio_Venta, Numero_Filas_Precio_Compra,
		Numero_Filas_Precio_Venta, Datos_Tiempo)==ERROR) {
		printf("Los datos del CSV de los precios son incorrectos \n");
		return ERROR;
	}

	//Se verifica que el CSV de los terminales es correcto
	if (Verificar_Datos_Terminales(Datos_Terminales, Numero_Filas_Terminales)==ERROR) {
		printf("Los Datos del CSV de la informacion de los terminales son incorrectas \n.");
	    return ERROR;
	}

	//Se verifica que el CSV de las restricciones del sistema es correcto->
	if (Verificar_Restricciones(Datos_Restricciones, Numero_Filas_CSV_Restricciones)) {
		printf("Los Datos del CSV que contiene las restricciones del sistema son incorrectas \n");
		return ERROR;
	}

	if (Verificar_Informacion_Vehiculos(Datos_Vehiculos,Numero_Filas_CSV_Vehiculos,Datos_Vehiculos)) {
		printf("Los Datos del CSV que contiene la informacion de los vehiculos son incorrectos \n");
		return ERROR;
	}
	return EXITO;
}