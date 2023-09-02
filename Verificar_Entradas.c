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


static bool Comprobar_Objetivo_Bateria(const char* Objetivo) {
	//Este subprograma se utiliza para comprobar si el termino de considerar la carga de la bateria es correcto
	if (strcmp("si", Objetivo) == 0) {
		return true;
	}
	else if (strcmp("no", Objetivo) == 0) {
		return true;
	}
	else {
		printf("En el campo de considerar la carga debe introducirse un si o un no \n");
		return false;
	}
}
bool Considerar_Objetivo_Bateria(const char *Objetivo) {
	if (strcmp("si", Objetivo) == 0) {
		return true;
	}
	else {
		return false;
	}
}

static void Cargar_Fecha_Inicial_Bateria(struct tm *Fecha_Inicial_Bateria,const Datos_CSV ***Datos_Baterias,
	                                     const int Numero_Bateria) {
	//Este subprograma se utiliza para cargar la fecha inicial de la bateria en una variable->
	Fecha_Inicial_Bateria->tm_year = Datos_Baterias[OFFSET_FILA_CSV_BATERIAS  + Numero_Bateria][COLUMNA_CSV_BATERIAS_ANYO_INICIAL]->data.dbl - OFFSET_ANYO_tm;
	Fecha_Inicial_Bateria->tm_mon = Datos_Baterias [OFFSET_FILA_CSV_BATERIAS + Numero_Bateria][COLUMNA_CSV_BATERIAS_MES_INICIAL]->data.dbl   - OFFSET_MES_tm;
	Fecha_Inicial_Bateria->tm_mday = Datos_Baterias[OFFSET_FILA_CSV_BATERIAS + Numero_Bateria][COLUMNA_CSV_BATERIAS_DIA_INICIAL]->data.dbl;
	Fecha_Inicial_Bateria->tm_hour = Datos_Baterias[OFFSET_FILA_CSV_BATERIAS + Numero_Bateria][COLUMNA_CSV_BATERIAS_HORA_INICIAL]->data.dbl;
	Fecha_Inicial_Bateria->tm_min = Datos_Baterias [OFFSET_FILA_CSV_BATERIAS + Numero_Bateria][COLUMNA_CSV_BATERIAS_MINUTO_INICIAL]->data.dbl;
	Fecha_Inicial_Bateria->tm_sec = 0;
}

static void Cargar_Fecha_Final_Bateria(struct tm* Fecha_Final_Bateria, const Datos_CSV*** Datos_Baterias,
	                                   const int Numero_Bateria) {
	//Este subprograma se utiliza para cargar la fecha final de carga de la bateria en una variable
	Fecha_Final_Bateria->tm_year = Datos_Baterias [OFFSET_FILA_CSV_BATERIAS + Numero_Bateria][COLUMNA_CSV_BATERIAS_ANYO_FINAL]->data.dbl - OFFSET_ANYO_tm;
	Fecha_Final_Bateria->tm_mon =  Datos_Baterias [OFFSET_FILA_CSV_BATERIAS + Numero_Bateria][COLUMNA_CSV_BATERIAS_MES_FINAL]->data.dbl  - OFFSET_MES_tm;
	Fecha_Final_Bateria->tm_mday = Datos_Baterias [OFFSET_FILA_CSV_BATERIAS + Numero_Bateria][COLUMNA_CSV_BATERIAS_DIA_FINAL]->data.dbl;
	Fecha_Final_Bateria->tm_hour = Datos_Baterias [OFFSET_FILA_CSV_BATERIAS + Numero_Bateria][COLUMNA_CSV_BATERIAS_HORA_FINAL]->data.dbl;
	Fecha_Final_Bateria->tm_min =  Datos_Baterias [OFFSET_FILA_CSV_BATERIAS + Numero_Bateria][COLUMNA_CSV_BATERIAS_MINUTO_FINAL]->data.dbl;
	Fecha_Final_Bateria->tm_sec = 0;
}

static void Cargar_Fecha_Objetivo_Bateria(struct tm *Fecha_Objetivo_Bateria, const Datos_CSV ***Datos_Baterias,
	const int Numero_Bateria){
	Fecha_Objetivo_Bateria->tm_year = Datos_Baterias[OFFSET_FILA_CSV_BATERIAS + Numero_Bateria][COLUMNA_CSV_BATERIAS_ANYO_FINAL]->data.dbl - OFFSET_ANYO_tm;
	Fecha_Objetivo_Bateria->tm_mon = Datos_Baterias [OFFSET_FILA_CSV_BATERIAS + Numero_Bateria][COLUMNA_CSV_BATERIAS_MES_FINAL]->data.dbl  - OFFSET_MES_tm;
	Fecha_Objetivo_Bateria->tm_mday = Datos_Baterias[OFFSET_FILA_CSV_BATERIAS + Numero_Bateria][COLUMNA_CSV_BATERIAS_DIA_FINAL]->data.dbl;
	Fecha_Objetivo_Bateria->tm_hour = Datos_Baterias[OFFSET_FILA_CSV_BATERIAS + Numero_Bateria][COLUMNA_CSV_BATERIAS_HORA_FINAL]->data.dbl;
	Fecha_Objetivo_Bateria->tm_min = Datos_Baterias [OFFSET_FILA_CSV_BATERIAS + Numero_Bateria][COLUMNA_CSV_BATERIAS_MINUTO_FINAL]->data.dbl;
	Fecha_Objetivo_Bateria->tm_sec = 0;
}
static int Comprobar_Fecha_Inicial_Bateria(struct tm* Fecha_Inicial_Bateria, const Datos_CSV*** Datos_Baterias,
	                                       const int Numero_Bateria, const struct tm Fecha_Inicial_Algoritmo) {

	//Se comprueba si la fecha inicial de conexion de la bateria es posterior a la fecha de inicio del algoritmo.
	int Resultado_Comparacion;
	Cargar_Fecha_Inicial_Bateria(&Fecha_Inicial_Bateria, Datos_Baterias, Numero_Bateria);
	Resultado_Comparacion = Comprobar_Orden_Fechas(Fecha_Inicial_Algoritmo,*Fecha_Inicial_Bateria);
	
	if (Resultado_Comparacion > 0) {
		printf("La fecha inicial del vehiculo no puede ser anterior a la fecha inicial del algoritmo \n");
		return ERROR;
	}
	return EXITO;
}

static int Comprobar_Fecha_Final_Bateria(struct tm* Fecha_Final_Bateria, const Datos_CSV*** Datos_Baterias,
	const int Numero_Bateria, const struct tm Fecha_Final_Algoritmo) {
	
	//Se comprueba si la fecha de desconexion de la bateria no es posterior a la finalizacion del algoritmo.
	int Resultado_Comparacion;
	Cargar_Fecha_Final_Bateria(&Fecha_Final_Bateria, Datos_Baterias, Numero_Bateria);
	Resultado_Comparacion = Comprobar_Orden_Fechas(Fecha_Final_Algoritmo,*Fecha_Final_Bateria);
	if (Resultado_Comparacion < 0) {
		printf("Error el algoritmo no puede acabar antes que la carga de la bateria \n");
		return ERROR;
	}
}
static int Comprobar_Orden(const struct tm Fecha_Inicial, const struct tm Fecha_Final) {
	//Este subprograma se utiliza para comprobar que la fecha inicial de conexion de la bateria o vehiculo sea anterior
	//a la fecha final de conexion de la bateria

	int Resultado_Comparacion = Comprobar_Orden_Fechas(Fecha_Inicial, Fecha_Final);
	if (Resultado_Comparacion > 0) {
		return ERROR;
	}
	return EXITO;
}
static int Comprobar_Fecha_Objetivo_Baterias(const Datos_CSV*** Datos_Baterias, const int Numero_Bateria,
	                                         const struct tm Fecha_Inicial_Bateria, const struct tm Fecha_Final_Bateria) {
	//Este subprograma se utiliza apra comprobar que la fecha objetivo de la bateria es correcta.

	struct tm Fecha_Objetivo_Bateria;
	int       Resultado_Comparacion;
	
	Cargar_Fecha_Objetivo_Bateria(&Fecha_Objetivo_Bateria, Datos_Baterias, Numero_Bateria);
	Resultado_Comparacion = Comprobar_Orden_Fechas(Fecha_Inicial_Bateria, Fecha_Objetivo_Bateria);
	if (Resultado_Comparacion > 0) {
		printf("Error el objetivo de carga es anterior a la fecha inicial de la bateria, para la bateria %d \n ", Numero_Bateria);
		return ERROR;
	}
	Resultado_Comparacion = Comprobar_Orden_Fechas(Fecha_Final_Bateria, Fecha_Objetivo_Bateria);
	if (Resultado_Comparacion < 0) {
		printf("La fecha de desconexion de la bateria no puede ser antes que el objetivo de carga par ala bateria %d \n", Numero_Bateria);
		return ERROR;
	}
	return EXITO;
}
static int Comprobar_Fecha_Baterias(const struct tm Fecha_Inicial_Algoritmo, const struct tm Fecha_Final_Algoritmo,
	const int Numero_Baterias, const Datos_CSV*** Datos_Baterias) {

	//Este subprograma se utiliza con el objetivo de comprobar que las fechas de inicio y finalizacion
	//de carga de las diversas baterias tienen sentido.

	struct tm Fecha_Inicial_Bateria;
	struct tm Fecha_Final_Bateria;
	struct tm Fecha_Objetivo_Bateria;
	int       Variable_Control;

	for (int Numero_Bateria = 0; Numero_Bateria < Numero_Baterias; Numero_Bateria++) {
	
		//Se comprueba si la fecha inicial de la bateria es correcta
		if (Comprobar_Fecha_Inicial_Bateria(&Fecha_Inicial_Bateria, Datos_Baterias, Numero_Bateria, Fecha_Inicial_Algoritmo) == ERROR) {
			printf("La fecha Inicial de conexion de la bateria %d no es correcta", Numero_Bateria);
			return ERROR;
		}
		
		//Se comprueba si la fecha final de la bateria es correcta
		if (Comprobar_Fecha_Final_Bateria(&Fecha_Final_Bateria, Datos_Baterias,Numero_Bateria,Fecha_Final_Algoritmo)==ERROR) {
			printf("La fecha final de conexion de la bateria %d no es correcta", Numero_Bateria);
			return ERROR;
		}

		//Se comprueba que la fecha inicial de la bateria es anterior a la de finalizacion.
		if (Comprobar_Orden(Fecha_Inicial_Bateria,Fecha_Final_Bateria)==ERROR) {
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
		
	}
	return EXITO;
}

static bool Comprobar_Validez_Datos (const double Capacidad_Bateria, const double Maxima_Potencia, const double Bateria_Inicial,
	                                 const double Bateria_Deseada) {
	//Este subprograma se utiliza para comprobar si:
	//El dato de la capacidad de la bateria tiene sentido
	//El dato de la potencia maxima que se puede aceptar tiene sentido
    //El dato de la bateria inicial tiene sentido
	//El dato de la bateria deseada tiene sentido
	if (Capacidad_Bateria <= 0) {
		printf("La capacidad de la bateria no puede ser 0 o inferior a 0 \n");
		return false;
	}
	else if (Maxima_Potencia < 0) {
		printf("La potencia maxima no puede ser inferior a 0 \n");
		return false;
	}
	else if ((Bateria_Inicial < 0) || (Bateria_Inicial > 1)) {
		printf("La bateria Inicial ha de estar entre 0 o 1 (0% y 100% \n");
		return false;
	}
	else if ((Bateria_Deseada < 0 )   || (Bateria_Deseada >1)) {
		printf("La bateria deseada ha de estar entre 0 o 1 (0% y 100% \n");
		return false;
	}
}





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
		printf("Los Datos del CSV que contiene las restricciones del sistema son incorrectas");
		return ERROR;
	}

	if (Verificar_Informacion_Vehiculos(Datos_Vehiculos,Numero_Filas_CSV_Vehiculos,Datos_Vehiculos)) {
		printf("Los Datos del CSV que contiene la informacion de los vehiculos son incorrectos")
		return ERROR;
	}
		                  
}