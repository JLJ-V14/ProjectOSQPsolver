#include <stdio.h>
#include "Tipos_Optimizacion.h"
#include "Definiciones_Globales.h"

int Verificar_Restricciones(const Datos_CSV ***Datos_Restricciones, const int Numero_Filas_CSV_Restricciones) {
	//En este subprograma se verifica que las restricciones del sistema son correctas.

	double Maxima_Potencia_Sistema = Datos_Restricciones[0][COLUMNA_CSV_VALOR_RESTRICCION]->data.dbl;

	if (Maxima_Potencia_Sistema < 0) {
		printf("La potencia maxima que puede intercambiar la electrolinera con la red no puede ser negativa \n");
		return;
	}

	//Se comprueba que el numero de filas es 13, 12 para los terminales y una para los rotulos

	if (Numero_Filas_CSV_Restricciones != 13) {
		printf("El CSV debe tener 13 filas una para los rotulos con la informacion, otra para los terminales \n");
		return;
	}

	//Se crean variables auxiliares que se utilizan en la comprobacion de la informacion
	double Maxima_Potencia_Terminal;
	int    Numero_Terminal;
	int    Numero_Terminal_Anterior = 0;

	for (int NUMERO_TERMINAL = 0; NUMERO_TERMINAL < NUMERO_TERMINALES; NUMERO_TERMINAL++) {
	//Se itera por los terminales y se comprueba que la informacion es correcta

		Maxima_Potencia_Terminal = Datos_Restricciones[NUMERO_TERMINAL + 1][COLUMNA_CSV_VALOR_RESTRICCION]->data.dbl;
		Numero_Terminal          = Datos_Restricciones[NUMERO_TERMINAL + 1][COLUMNA_CSV_NUMERO_TERMINAL]->data.dbl;

		if (Numero_Terminal - Numero_Terminal_Anterior != 1) {
			printf("ERROR Los terminales han de ser consecutivos en el CSV de las restricciones \n");
			return ERROR;
		}
		if ((Maxima_Potencia_Terminal < 0)) {
			printf("Error la potencia maxima para el terminal %d no puede ser inferior a 0 \n", NUMERO_TERMINAL + 1);
			return ERROR;
		}
		
		Numero_Terminal_Anterior += 1;
	}
	return EXITO;
}