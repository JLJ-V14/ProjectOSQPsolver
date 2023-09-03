
#include "Funciones_Auxiliares.h"
#include "Tipos_Optimizacion.h"
#include <stdio.h>
//En este archivo .c vienen definidos los subprogramas que se encargan de comprobar que la informacion del CSV de las baterias
//son correctas


//En este archivo se encuentran los subprogramas que se encargan de verificar
//que la informacion de la bateria es correcta->


//----Revisado-----------------------------------------//

static int Comprobar_Datos_Bateria(const Datos_CSV*** Datos_Bateria, const int Numero_Bateria) {

	//Este subprograma se utiliza para comprobar si:
	//El terminal al que esta conectado es valido.
	//El dato de la capacidad de la bateria tiene sentido
	//El dato de la potencia maxima que se puede aceptar tiene sentido
	//El dato de la bateria inicial tiene sentido
	//El dato de la bateria deseada tiene sentido
	
	int    Numero_Terminal   = Datos_Bateria[Numero_Bateria][COLUMNA_CSV_BATERIAS_NUMERO_TERMINAL]->data.dbl;
	double Capacidad_Bateria = Datos_Bateria[Numero_Bateria][COLUMNA_CSV_BATERIAS_CAPACIDAD_BATERIA]->data.dbl;
	double Maxima_Potencia   = Datos_Bateria[Numero_Bateria][COLUMNA_CSV_BATERIAS_MAXIMA_POTENCIA]->data.dbl;
    double Bateria_Inicial   = Datos_Bateria[Numero_Bateria][COLUMNA_CSV_BATERIAS_INICIAL]->data.dbl;
	double Bateria_Deseada   = Datos_Bateria[Numero_Bateria][COLUMNA_CSV_BATERIAS_FINAL]->data.dbl;

	if ((Numero_Terminal < 0) || (Numero_Terminal > 12)) {
		printf("El numero de terminal al que esta conectado la bateria %d  \n", Numero_Bateria);
	}
	if (Capacidad_Bateria <= 0) {
		printf("La capacidad de la bateria %d no puede ser 0 o inferior a 0 \n",Numero_Bateria);
		return ERROR;
	}
	else if (Maxima_Potencia < 0) {
		printf("La potencia maxima de la bateria %d no puede ser inferior a 0 \n",Numero_Bateria);
		return ERROR;
	}
	else if ((Bateria_Inicial < 0) || (Bateria_Inicial > 1)) {
		printf("El porcentaje inicial de la bateria %d ha de estar entre 0 o 1 (0 % y 100 % \n",Numero_Bateria);
		return ERROR;
	}
	else if ((Bateria_Deseada < 0) || (Bateria_Deseada > 1)) {
		printf("El porcentaje de bateria deseada %d ha de estar entre 0 o 1 (0% y 100% \n",Numero_Bateria);
		return ERROR;
	}
	return EXITO;
}

static int Comprobar_Objetivo_Bateria(const char* Objetivo) {
	//Este subprograma se utiliza para comprobar si el termino de considerar la carga de la bateria es correcto
	if (strcmp("si", Objetivo) == 0) {
		return EXITO;
	}
	else if (strcmp("no", Objetivo) == 0) {
		return EXITO;
	}
	else {
		printf("En el campo de considerar la carga debe introducirse un si o un no \n");
		return ERROR;
	}
}



static int Comprobar_Fecha_Inicial_Bateria(struct tm* Fecha_Inicial_Bateria, const Datos_CSV*** Datos_Baterias,
	const int Numero_Bateria, const Datos_CSV ***Datos_Algoritmo) {

	//Cargo la fecha inicial del algoritmo->
	struct tm Fecha_Inicial_Algoritmo = { 0 };
	Cargar_Fecha(Datos_Algoritmo, &Fecha_Inicial_Algoritmo, COLUMNA_ANYO_INICIAL_ALGORITMO,
		         COLUMNA_MES_INICIAL_ALGORITMO, COLUMNA_DIA_INICIAL_ALGORITMO, COLUMNA_HORA_INICIAL_ALGORITMO,
		         COLUMNA_MINUTO_INICIAL_ALGORITMO, FILA_INFORMACION_ALGORITMO);


	//Se comprueba si la fecha inicial de conexion de la bateria es posterior a la fecha de inicio del algoritmo.
	Cargar_Fecha(Datos_Baterias, Fecha_Inicial_Bateria, COLUMNA_CSV_BATERIAS_ANYO_INICIAL,
		         COLUMNA_CSV_BATERIAS_MES_INICIAL, COLUMNA_CSV_BATERIAS_DIA_INICIAL,
		         COLUMNA_CSV_BATERIAS_HORA_INICIAL, COLUMNA_CSV_BATERIAS_MINUTO_INICIAL,
		         Numero_Bateria);



	if (Verificar_Orden_Fechas(Fecha_Inicial_Algoritmo, *Fecha_Inicial_Bateria) == ERROR) {
		printf("La fecha final de la bateria no puede ser posterior a la del algoritmo \n");
		return ERROR;
	}


	return EXITO;
}

static int Comprobar_Fecha_Final_Bateria(struct tm* Fecha_Final_Bateria, const Datos_CSV*** Datos_Baterias,
	                 const int Numero_Bateria, const Datos_CSV ***Datos_Algoritmo) {
	
	//Se comprueba que la fecha final de la bateria es correcta->

	struct tm Fecha_Final_Algoritmo = { 0 };
	Cargar_Fecha(Datos_Algoritmo, &Fecha_Final_Algoritmo, COLUMNA_ANYO_FINAL_ALGORITMO,
		         COLUMNA_MES_FINAL_ALGORITMO, COLUMNA_DIA_FINAL_ALGORITMO, COLUMNA_HORA_FINAL_ALGORITMO,
		         COLUMNA_MINUTO_FINAL_ALGORITMO, FILA_INFORMACION_ALGORITMO);

	//Se comprueba si la fecha de desconexion de la bateria no es posterior a la finalizacion del algoritmo.
	int Resultado_Comparacion;

	Cargar_Fecha(Datos_Baterias, Fecha_Final_Bateria, COLUMNA_CSV_BATERIAS_ANYO_FINAL,
		         COLUMNA_CSV_BATERIAS_MES_FINAL,    COLUMNA_CSV_BATERIAS_DIA_FINAL,
		         COLUMNA_CSV_BATERIAS_HORA_FINAL,   COLUMNA_CSV_BATERIAS_MINUTO_FINAL,
		         Numero_Bateria);


	if (Verificar_Orden_Fechas(Fecha_Final_Algoritmo, *Fecha_Final_Bateria) == ERROR) {
		printf("La fecha final de la bateria no puede ser posterior a la del algoritmo \n");
		return ERROR;
	}

	return EXITO;
}

bool Considerar_Objetivo_Bateria(const char* Objetivo) {
	if (strcmp("si", Objetivo) == 0) {
		return true;
	}
	else {
		return false;
	}
}

static int Comprobar_Fecha_Objetivo_Baterias(const Datos_CSV*** Datos_Baterias, const int Numero_Bateria,
	const struct tm Fecha_Inicial_Bateria, const struct tm Fecha_Final_Bateria) {
	//Este subprograma se utiliza apra comprobar que la fecha objetivo de la bateria es correcta.

	struct tm Fecha_Objetivo_Bateria;
	int       Resultado_Comparacion;

	Cargar_Fecha(Datos_Baterias,&Fecha_Objetivo_Bateria, COLUMNA_CSV_BATERIAS_ANYO_OBJETIVO,
		         COLUMNA_CSV_BATERIAS_MES_OBJETIVO, COLUMNA_CSV_BATERIAS_DIA_OBJETIVO,
		         COLUMNA_CSV_BATERIAS_HORA_OBJETIVO, COLUMNA_CSV_BATERIAS_MINUTO_OBJETIVO,
		         Numero_Bateria);

	if (Verificar_Orden_Fechas(Fecha_Objetivo_Bateria, Fecha_Inicial_Bateria) == ERROR) {
		printf("La fecha final de la bateria no puede ser posterior a la del algoritmo \n");
		return ERROR;
	}
	if (Verificar_Orden_Fechas(Fecha_Final_Bateria, Fecha_Objetivo_Bateria) == ERROR) {
		printf("La fecha final de la bateria no puede ser posterior a la del algoritmo \n");
		return ERROR;
	}


	return EXITO;
}

static int Comprobar_Fecha_Baterias(const int Numero_Bateria, const Datos_CSV*** Datos_Baterias,
	                                const Datos_CSV *** Datos_Algoritmo) {

	//Este subprograma se utiliza con el objetivo de comprobar que las fechas de inicio y finalizacion
	//de carga de las diversas baterias tienen sentido.

	struct tm Fecha_Inicial_Bateria;
	struct tm Fecha_Final_Bateria;
	struct tm Fecha_Objetivo_Bateria;
	int       Variable_Control;

	

		//Se comprueba si la fecha inicial de la bateria es correcta
		if (Comprobar_Fecha_Inicial_Bateria(&Fecha_Inicial_Bateria, Datos_Baterias, Numero_Bateria, Datos_Algoritmo) == ERROR) {
			printf("La fecha Inicial de conexion de la bateria %d no es correcta", Numero_Bateria);
			return ERROR;
		}

		//Se comprueba si la fecha final de la bateria es correcta
		if (Comprobar_Fecha_Final_Bateria(&Fecha_Final_Bateria, Datos_Baterias, Numero_Bateria, Datos_Algoritmo) == ERROR) {
			printf("La fecha final de conexion de la bateria %d no es correcta", Numero_Bateria);
			return ERROR;
		}

		//Se comprueba que la fecha inicial de la bateria es anterior a la de finalizacion.
		if (Verificar_Orden_Fechas(Fecha_Final_Bateria,Fecha_Inicial_Bateria) == ERROR) {
			printf("Error la fecha inicial de carga de la bateria no puede ser posterior a la fecha final de carga \n");
			return ERROR;
		}

		//Se revisa si para la bateria en cuestion se desea si se desea que tenga un cierto nivel de carga para alguna
		//fecha en particular.
		if (Considerar_Objetivo_Bateria(Datos_Baterias[OFFSET_FILA_CSV_VEHICULOS + Numero_Bateria][COLUMNA_CSV_BATERIAS_OBJETIVO_CARGA]->data.str)) {

			//Se comprueba que la fecha objetivo de carga de la bateria sea correcta.
			if (Comprobar_Fecha_Objetivo_Baterias(Datos_Baterias, Numero_Bateria, Fecha_Inicial_Bateria, Fecha_Final_Bateria) == ERROR) {
				return ERROR;
			}
		

	}
	return EXITO;
}




int Verificar_Baterias(const Datos_CSV *** Datos_CSV_Baterias, const Datos_CSV *** Datos_CSV_Algoritmo,
	                    const int Numero_Filas_CSV_Baterias){
	
	//En este subprograma se va comprobando que los datos del CSV que contiene la informacion de 
	//las baterias es correcto
	for (int Numero_Bateria = 1; Numero_Bateria < Numero_Filas_CSV_Baterias; Numero_Bateria++) {

		//Se comprueba que las fechas de conexion y desconexion de las baterias son correctas.
		if (Comprobar_Fecha_Baterias(Numero_Bateria, Datos_CSV_Baterias, Datos_CSV_Algoritmo) == ERROR) {
			return ERROR;
		}

		//Se comprueba que los datos de la bateria son correctos.
		if (Comprobar_Datos_Bateria(Datos_CSV_Baterias, Numero_Bateria) == ERROR) {
			return ERROR;
		}
	}
	return EXITO;
}