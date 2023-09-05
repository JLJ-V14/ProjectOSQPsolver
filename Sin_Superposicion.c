
#include "Definiciones_Globales.h"
#include "Funciones_Auxiliares.h"
#include "Tipos_Optimizacion.h"

//En este archivo se verifica que no hay baterias o vehiculos conectados al mismo terminal
//al mismo tiempo 


static int  Reajustar_Dimension_Array(Ocupacion_Terminales *Terminales,const int Numero_Terminal) {

	//Este subprograma se utiliza para calcular el neuvo tamaño de la dimension del array;
	int Nueva_Dimension_Array = Terminales->Numero_Franjas_Ocupadas + 1 ;

	Franja_Ocupada* temp = realloc(Terminales->Franjas, Nueva_Dimension_Array * sizeof(Franja_Ocupada));
	
	if (temp == NULL) {
		printf("Ha habido un problema con la reserva en memoria para la ocupacion de los terminales \n");
		return ERROR;
	}
	else {
		Terminales->Franjas = temp;
		Terminales->Numero_Franjas_Ocupadas++;
		return EXITO;
	}
}
static void Inicializar_Ocupacion_Terminales(Ocupacion_Terminales *Terminales) {

	//Inicializo la variable que sirve para almacenar la informacion de la ocupacion de los terminales->
	for (int i = 0; i < NUMERO_TERMINALES; i++) {
		Terminales[i].Numero_Franjas_Ocupadas=0;
		Terminales[i].Franjas = NULL;
	}
}

static int Leer_Ocupacion(const Datos_CSV*** Datos_Excel,const int Numero_Filas_Csv_Vehiculos,
	                       Ocupacion_Terminales* Terminales, const int Columna_Anyo_Inicial,
	                       const int Columna_Mes_Inicial,    const int Columna_Dia_Inicial,
	                       const int Columna_Hora_Inicial,   const int Columna_Minuto_Inicial,
	                       const int Columna_Anyo_Final,     const int Columna_Mes_Final,
	                       const int Columna_Dia_Final,      const int Columna_Hora_Final,
	                       const int Columna_Minuto_Final,   const int Columna_Terminal) {

	//Se va iterando por las distintas filas y se va añadiendo los periodos en los que un terminal esta ocupado
	//a la variable que se utiliza para registrar esta informacion
    
	for (int Numero_Fila = 1; Numero_Fila < Numero_Filas_Csv_Vehiculos;Numero_Fila++) {

		int Numero_Terminal = Datos_Excel[Numero_Fila][Columna_Terminal];
		int Indice_Franja = Terminales[Numero_Terminal - 1].Numero_Franjas_Ocupadas;
		//Se cargan las fechas para las cuales el terminal esta ocupado
		if (Reajustar_Dimension_Array(Terminales, Numero_Terminal) == ERROR) {
			printf("Ha habido un problema con la memoria de la ocupacion de los terminales \n");
			return ERROR;
		}



		//Se carga la fecha inicial de la bateria o vehiculo
		Cargar_Fecha(Datos_Excel, &(Terminales[Numero_Terminal-1].Franjas[Indice_Franja].Fecha_Inicial), Columna_Anyo_Inicial,
	    Columna_Mes_Inicial, Columna_Dia_Inicial, Columna_Hora_Inicial,Columna_Minuto_Inicial, Numero_Fila);

		//Se carga la fecha final de la bateria o vehiculo
		Cargar_Fecha(Datos_Excel, &(Terminales[Numero_Terminal-1].Franjas[Indice_Franja].Fecha_Final), Columna_Anyo_Final,
		Columna_Mes_Final, Columna_Dia_Final, Columna_Hora_Final, Columna_Minuto_Final, Numero_Fila);

	}
	return EXITO;
}

static int Revisar_Superposicion(const struct tm *Inicio_A, const struct tm *Fin_A,
	                             const struct tm *Inicio_B, const struct tm *Fin_B) {
	time_t Fecha_Inicial_A  = mktime((struct tm*)Inicio_A);
	time_t Fecha_Final_A    = mktime((struct tm*)Fin_A);
	time_t Fecha_Inicial_B  = mktime((struct tm*)Inicio_B);
	time_t Fecha_Final_B    = mktime((struct tm*)Fin_B);

	//Se revisa si hay alguna superposicion->
	if (Fecha_Final_A < Fecha_Inicial_B || Fecha_Inicial_A > Fecha_Final_B) {
		return EXITO;
	}
	return ERROR;
}


static int Comprobar_Terminal(Franja_Ocupada Franjas [], const int Numero_Franjas_Ocupadas) {
	//En este subprograma se comprueba que las determinadas franjas de un terminal no coinciden en el
	//tiempo
	for (int Numero_Franja = 0; Numero_Franja < Numero_Franjas_Ocupadas-1; Numero_Franja++) {
		
		struct tm Fecha_Inicial_Franja_1 = Franjas[Numero_Franja].Fecha_Inicial;
		struct tm Fecha_Final_Franja_1   = Franjas[Numero_Franja].Fecha_Final;
		
    for (int Numero_Franja_Bucle = Numero_Franja+1; Numero_Franja < Numero_Franjas_Ocupadas; Numero_Franja_Bucle++) {
			
		struct tm Fecha_Inicial_Franja_2 = Franjas[Numero_Franja_Bucle].Fecha_Inicial;
		struct tm Fecha_Final_Franja_2   = Franjas[Numero_Franja_Bucle].Fecha_Final;

		if ((Revisar_Superposicion(&Fecha_Inicial_Franja_1, &Fecha_Final_Franja_1,
		    &Fecha_Inicial_Franja_2, &Fecha_Final_Franja_2)) == ERROR) {
		    return ERROR;
			}

		}
	}
	return EXITO;
}

static int Verificar_Disponibilidad(Ocupacion_Terminales Terminales[]) {
	//En este subprograma se verifica que no hay franjas conflictivas en un terminal->

	for (int Numero_Terminal = 0; Numero_Terminal < NUMERO_TERMINALES; Numero_Terminal++) {
		if (Comprobar_Terminal(Terminales[Numero_Terminal].Franjas, Terminales[Numero_Terminal].Numero_Franjas_Ocupadas)==ERROR) {
			printf("Hay dos vehiculos o baterias que para el mismo terminal estan en simultaneo \n");
			return ERROR;
		}

	}
	return EXITO;
}
int Verificar_Superposicion_Terminales (const Datos_CSV ***Datos_Vehiculos,   const Datos_CSV *** Datos_Baterias,
	                                    const int Numero_Filas_Csv_Vehiculos, const int Numero_Filas_Csv_Baterias) {

	//Defino el tipo para comprobar la informacion en los terminales->
	Ocupacion_Terminales Terminales[NUMERO_TERMINALES];
	Inicializar_Ocupacion_Terminales(&Terminales);
	
	//Se pasa a leer cuando ocupan los terminales los vehiculos.
	if (Leer_Ocupacion(Datos_Vehiculos, Numero_Filas_Csv_Vehiculos, Terminales, COLUMNA_CSV_VEHICULOS_ANYO_INICIAL,
		COLUMNA_CSV_VEHICULOS_MES_INICIAL, COLUMNA_CSV_VEHICULOS_DIA_INICIAL,
		COLUMNA_CSV_VEHICULOS_HORA_INICIAL, COLUMNA_CSV_VEHICULOS_MINUTO_INICIAL,
		COLUMNA_CSV_VEHICULOS_ANYO_FINAL, COLUMNA_CSV_VEHICULOS_MES_FINAL,
		COLUMNA_CSV_VEHICULOS_DIA_FINAL, COLUMNA_CSV_VEHICULOS_HORA_FINAL,
		COLUMNA_CSV_VEHICULOS_MINUTO_FINAL, COLUMNA_CSV_VEHICULOS_NUMERO_TERMINAL) == ERROR) {
		
		return ERROR;
	}
	//Se pasa a leer cuando ocupan los terminales las baterias.
	if (Leer_Ocupacion(Datos_Baterias, Numero_Filas_Csv_Baterias, Terminales, COLUMNA_CSV_BATERIAS_ANYO_INICIAL,
		COLUMNA_CSV_BATERIAS_MES_INICIAL, COLUMNA_CSV_BATERIAS_DIA_INICIAL,
		COLUMNA_CSV_BATERIAS_HORA_INICIAL, COLUMNA_CSV_BATERIAS_MINUTO_INICIAL,
		COLUMNA_CSV_BATERIAS_ANYO_FINAL, COLUMNA_CSV_BATERIAS_MES_FINAL,
		COLUMNA_CSV_BATERIAS_DIA_FINAL, COLUMNA_CSV_BATERIAS_HORA_FINAL,
		COLUMNA_CSV_BATERIAS_MINUTO_FINAL, COLUMNA_CSV_BATERIAS_NUMERO_TERMINAL) == ERROR) {

		return ERROR;
	}

	if ((Verificar_Disponibilidad(Terminales))==ERROR) {
		printf("Hay problemas de superposicion en los terminales \n");
		return ERROR;
	}

	return EXITO;
}