#include "Definiciones_Globales.h"
#include "Tipos_Optimizacion.h"


static int Comprobar_Fase(const Datos_CSV*** Datos_Terminales, const int Fila_Terminal) {
	//Se utiliza este subprograma para comprobar la fase del terminal->
	char* Fase = Datos_Terminales[Fila_Terminal][COLUMNA_CSV_FASE_TERMINAL];

	if (strcmp(Fase, "R") == 0 || strcmp(Fase, "S") == 0 || strcmp(Fase, "T") == 0 ||
		strcmp(Fase, "NEUTRO") == 0 || strcmp(Fase, "NADA") == 0) {
		return EXITO;
	}
	else {
		return ERROR;
	}
}
static int Comprobar_Informacion_Terminal(const Datos_CSV*** Datos_Terminales, const int Fila_Terminal,
	const int Terminal_Anterior) {
	//Este subprograma acepta dos entradas el array donde esta contenida 
	//la informacion de los terminales, la fila del csv que se esta 
	// comprobando y el terminal en la fila anterior del CSV.

	//Se Lee el numero de terminal
	int Numero_Terminal = Datos_Terminales[Fila_Terminal][COLUMNA_CSV_NUMERO_TERMINAL];

	//Se comprueba que los terminales son consecutivos->
	if (Numero_Terminal - Terminal_Anterior != 1) {
		printf("Los terminales han de estar en orden consecutivo \n");
		return ERROR;
	}

	//Se comprueba que la fase a la que estan conectadas los diferentes terminales
	//son la fase R la S la T,Neutro o ninguna.
	if (Comprobar_Fase(Datos_Terminales, Fila_Terminal) == ERROR) {
		printf("La fase asociada al terminal %d es incorrecta", Fila_Terminal);
		return ERROR;
	}
	return EXITO;
}
 int Verificar_Datos_Terminales(const Datos_CSV*** Datos_Terminales, const int Numero_Filas_Terminales) {
	//En este subprograma se verifica que los datos del CSV de los terminales son correctos.

	if (Numero_Filas_Terminales != 13) {
		printf("El numero de filas debe ser 13, la primera para la informacion las 12 siguientes para los terminales \n");
		return ERROR;
	}

	//Se crean variables auxiliares
	int Terminal_Anterior = 0;
	int Terminal_Actual;

	//Se comprobará que:
	//Los terminales están en orden
	//Los terminales estan conectados a fase R,S,T,Neutro o Ninguna.

	for (int Numero_Terminal = 1; Numero_Terminal < 13; Numero_Terminal++) {
		if (Comprobar_Informacion_Terminal(Datos_Terminales, Numero_Terminal, Terminal_Anterior) == ERROR) {
			return ERROR;
		}
		Terminal_Anterior = Terminal_Anterior + 1;
	}
	return EXITO;
}