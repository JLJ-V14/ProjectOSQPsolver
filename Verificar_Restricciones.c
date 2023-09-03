#include <stdio.h>
#include "Tipos_Optimizacion.h"
#include "Definiciones_Globales.h"

//En este Archivo se define el subprograma que comprueba el CSV de las restricciones->
//-------------------Revisado------------------------------------------------------------

static int Revisar_Potencia_Maxima(const Datos_CSV*** Datos_Restricciones,const int Numero_Fila) {
	
	//En este subprograma se revisa que la potencia maxima que puede intercambiar la electrolinera con la red
	//es un dato valido.
	
	double Maxima_Potencia = Datos_Restricciones[Numero_Fila][COLUMNA_CSV_VALOR_RESTRICCION]->data.dbl;
	
	if (Maxima_Potencia < 0) {
		printf("La potencia maxima  no puede ser negativa \n");
		return ERROR;
	}
	return EXITO;
}

static int Comprobar_Numero_Filas_Csv_Restricciones(const Datos_CSV ***Datos_Restricciones,
	                                                const int Numero_Filas_Csv_Restricciones) {
	//En este subprograma se comprueba que el numero de filas del CSV
	//que contiene las restricciones es correcta

	if (Numero_Filas_Csv_Restricciones != 13) {
		printf("El CSV debe tener 13 filas una para los rotulos con la informacion, otra para los terminales \n");
		return ERROR;
	}
	return EXITO;
}

static int Comprobar_Consecutividad_Terminales(const Datos_CSV ***Datos_Restricciones,const int Numero_Terminal_Anterior,
	                                           const int Indice_Terminal) {
	
	int Numero_Terminal = Datos_Restricciones[Indice_Terminal][COLUMNA_CSV_NUMERO_TERMINAL]->data.dbl;

	if (Numero_Terminal - Numero_Terminal_Anterior != 1) {
		printf("ERROR Los terminales han de ser consecutivos en el CSV de las restricciones \n");
		return ERROR;
	}
	return EXITO;
}
 
int Verificar_Restricciones(const Datos_CSV ***Datos_Restricciones, const int Numero_Filas_CSV_Restricciones) {
	//En este subprograma se verifica que las restricciones del sistema son correctas.

	if (Revisar_Potencia_Maxima(Datos_Restricciones, FILA_CSV_MAXIMA_POTENCIA_SISTEMA) == ERROR) {
		return ERROR;
	}

	//Se comprueba que el numero de filas es 13, 12 para los terminales y una para los rotulos
	if (Comprobar_Numero_Filas_Csv_Restricciones(Datos_Restricciones, Numero_Filas_CSV_Restricciones) == ERROR) {
		return ERROR;
	}

	//Se crean variables auxiliares que se utilizan en la comprobacion de la informacion
	int    Numero_Terminal_Anterior = 0;

	for (int Numero_Terminal = 1; Numero_Terminal <= NUMERO_TERMINALES; Numero_Terminal++) {
	//Se itera por los terminales y se comprueba que la informacion es correcta
		
		if (Comprobar_Consecutividad_Terminales(Datos_Restricciones, Numero_Terminal_Anterior, Numero_Terminal) == ERROR) {
			return ERROR;
		}
		
		if (Revisar_Potencia_Maxima(Datos_Restricciones, Numero_Terminal) == ERROR) {
			return ERROR;
		}

		Numero_Terminal_Anterior += 1;
	}
	return EXITO;
}