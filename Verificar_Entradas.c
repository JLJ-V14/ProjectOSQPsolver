#include "Definiciones_Globales.h"
#include "Funciones_Auxiliares.h"
#include "time.h"
#include "Tipos_Optimizacion.h"
#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>
#include <sys/types.h>
#include <string.h>

static void Cargar_Fecha_Inicial_Algoritmo(const Datos_CSV*** Datos_Tiempo, struct tm* Fecha_Inicial) {
	//Este subprograma se utiliza para cargar en la variable Fecha_Inicial la fecha inicial 
	Fecha_Inicial->tm_year = Datos_Tiempo[FILA_INFORMACION_TEMPORAL_ALGORITMO][COLUMNA_ANYO_INICIAL_ALGORITMO]->data.dbl - OFFSET_ANYO_tm;
	Fecha_Inicial->tm_mon  = Datos_Tiempo[FILA_INFORMACION_TEMPORAL_ALGORITMO][COLUMNA_MES_INICIAL_ALGORITMO]->data.dbl  - OFFSET_MES_tm;
	Fecha_Inicial->tm_mday = Datos_Tiempo[FILA_INFORMACION_TEMPORAL_ALGORITMO][COLUMNA_DIA_INICIAL_ALGORITMO]->data.dbl;
	Fecha_Inicial->tm_hour = Datos_Tiempo[FILA_INFORMACION_TEMPORAL_ALGORITMO][COLUMNA_HORA_INICIAL_ALGORITMO]->data.dbl;
	Fecha_Inicial->tm_min  = Datos_Tiempo[FILA_INFORMACION_TEMPORAL_ALGORITMO][COLUMNA_MINUTO_INICIAL_ALGORITMO]->data.dbl;
	Fecha_Inicial->tm_sec  = 0;
}

static void Cargar_Fecha_Final_Algoritmo(const Datos_CSV*** Datos_Tiempo, struct tm* Fecha_Final) {
	//Este subprograma se utiliza para cargar la fecha final en la variable fecha final
	Fecha_Final->tm_year = Datos_Tiempo[FILA_INFORMACION_TEMPORAL_ALGORITMO][COLUMNA_ANYO_FINAL_ALGORITMO]->data.dbl- OFFSET_ANYO_tm;
	Fecha_Final->tm_mon  = Datos_Tiempo[FILA_INFORMACION_TEMPORAL_ALGORITMO][COLUMNA_MES_FINAL_ALGORITMO]->data.dbl - OFFSET_MES_tm;
	Fecha_Final->tm_mday = Datos_Tiempo[FILA_INFORMACION_TEMPORAL_ALGORITMO][COLUMNA_DIA_FINAL_ALGORITMO]->data.dbl;
	Fecha_Final->tm_hour = Datos_Tiempo[FILA_INFORMACION_TEMPORAL_ALGORITMO][COLUMNA_HORA_FINAL_ALGORITMO]->data.dbl;
	Fecha_Final->tm_min  = Datos_Tiempo[FILA_INFORMACION_TEMPORAL_ALGORITMO][COLUMNA_MINUTO_FINAL_ALGORITMO]->data.dbl;
	Fecha_Final->tm_sec  = 0;
}

static void Cargar_Fecha_Inicial_Vehiculo(const Datos_CSV*** Datos_Vehiculos, struct tm* Fecha_Inicial_Vehiculo,
	                                      const int Numero_Vehiculo) {
	//Se carga la fecha inicial de carga del vehiculo en la variable Fecha_Inicial_Vehiculo
	Fecha_Inicial_Vehiculo->tm_year = Datos_Vehiculos[OFFSET_FILA_CSV_VEHICULOS  + Numero_Vehiculo][COLUMNA_CSV_VEHICULOS_ANYO_INICIAL]->data.dbl - OFFSET_ANYO_tm;
	Fecha_Inicial_Vehiculo->tm_mon = Datos_Vehiculos [OFFSET_FILA_CSV_VEHICULOS + Numero_Vehiculo][COLUMNA_CSV_VEHICULOS_MES_INICIAL]->data.dbl   - OFFSET_MES_tm;
	Fecha_Inicial_Vehiculo->tm_mday = Datos_Vehiculos[OFFSET_FILA_CSV_VEHICULOS + Numero_Vehiculo][COLUMNA_CSV_VEHICULOS_DIA_INICIAL]->data.dbl;
	Fecha_Inicial_Vehiculo->tm_hour = Datos_Vehiculos[OFFSET_FILA_CSV_VEHICULOS + Numero_Vehiculo][COLUMNA_CSV_VEHICULOS_HORA_INICIAL]->data.dbl;
	Fecha_Inicial_Vehiculo->tm_min = Datos_Vehiculos [OFFSET_FILA_CSV_VEHICULOS + Numero_Vehiculo][COLUMNA_CSV_VEHICULOS_MINUTO_INICIAL]->data.dbl;
	Fecha_Inicial_Vehiculo->tm_sec = 0;
}
static void Cargar_Fecha_Final_Vehiculo(const Datos_CSV*** Datos_Vehiculos, struct tm* Fecha_Final_Vehiculo,
	                                    const int Numero_Vehiculo) {
	//Se carga la fecha final de carga del vehiculo en la variable Fecha_Final_Vehiculo
	Fecha_Final_Vehiculo->tm_year = Datos_Vehiculos[OFFSET_FILA_CSV_VEHICULOS + Numero_Vehiculo][COLUMNA_CSV_VEHICULOS_ANYO_FINAL]->data.dbl - OFFSET_ANYO_tm;
	Fecha_Final_Vehiculo->tm_mon = Datos_Vehiculos [OFFSET_FILA_CSV_VEHICULOS + Numero_Vehiculo][COLUMNA_CSV_VEHICULOS_MES_FINAL]->data.dbl  - OFFSET_MES_tm;
	Fecha_Final_Vehiculo->tm_mday= Datos_Vehiculos [OFFSET_FILA_CSV_VEHICULOS + Numero_Vehiculo][COLUMNA_CSV_VEHICULOS_DIA_FINAL]->data.dbl;
	Fecha_Final_Vehiculo->tm_hour = Datos_Vehiculos[OFFSET_FILA_CSV_VEHICULOS  + Numero_Vehiculo][COLUMNA_CSV_VEHICULOS_HORA_FINAL]->data.dbl;
	Fecha_Final_Vehiculo->tm_min = Datos_Vehiculos [OFFSET_FILA_CSV_VEHICULOS  + Numero_Vehiculo][COLUMNA_CSV_VEHICULOS_MINUTO_FINAL]->data.dbl;
	Fecha_Final_Vehiculo->tm_sec = 0;
}
static int Comprobar_Fecha_Inicial_Vehiculo(struct tm *Fecha_Inicial_Vehiculo,const Datos_CSV ***Datos_Vehiculos,
	                                         const int Numero_Vehiculo,const struct tm Fecha_Inicial_Algoritmo) {
	
	//Este subprograma se utiliza para comprobar que la fecha inicial de carga del vehiculo tiene sentido
	Cargar_Fecha_Inicial_Vehiculo(Datos_Vehiculos, Fecha_Inicial_Vehiculo, Numero_Vehiculo);
	int Resultado_Comparacion;
	
	Resultado_Comparacion = Comprobar_Orden_Fechas(Fecha_Inicial_Algoritmo, *Fecha_Inicial_Vehiculo);
	
	//Si la fecha inicial del algoritmo es posterior a la fecha inicial de carga del vehiculo, la fecha es incorrecta
	if (Resultado_Comparacion > 0) {
		printf("La fecha inicial del vehiculo no puede ser anterior a la fecha inicial del algoritmo %d \n", Numero_Vehiculo);
		return ERROR;
	}
	else {
		return EXITO;
	}
}

static int Comprobar_Fecha_Final_Vehiculo(const Datos_CSV *** Datos_Vehiculos, struct tm*Fecha_Final_Vehiculo,
	                                      const struct tm Fecha_Final_Algoritmo, const int Numero_Vehiculo) {
	//Este subprograma se utiliza para comprobar que la fecha final del vehiculo es correcta->

	Cargar_Fecha_Final_Vehiculo(Datos_Vehiculos, Fecha_Final_Vehiculo, Numero_Vehiculo);
	int Resultado_Comparacion;

	Resultado_Comparacion = Comprobar_Orden_Fechas(Fecha_Final_Algoritmo, *Fecha_Final_Vehiculo);
	
	if (Resultado_Comparacion < 0) {
		printf("Error el algoritmo no puede acabar antes que la carga del vehiculo %d \n", Numero_Vehiculo);
		return ERROR;
	}
	else {
		return EXITO;
	}
}


static int Comprobar_Fecha_Vehiculos(const struct tm Fecha_Inicial_Algoritmo, const struct tm Fecha_Final_Algoritmo,
	                                 const int Numero_Vehiculos, const Datos_CSV ***Datos_Vehiculos) {

	//En este subprograma se comprueba que las fechas de inicio y finalizacion de carga de los vehiculos tienen
	//sentido logico, que la fecha inicial no es poterior a la final y que esta dentro de la ventana temporal
	//en la cual se ejecuta el algoritmo.
	
	//Creo variables Auxiliares
	struct tm Fecha_Inicial_Vehiculo;
	struct tm Fecha_Final_Vehiculo;
	
	for (int Numero_Vehiculo = 0; Numero_Vehiculo < Numero_Vehiculos; Numero_Vehiculo++) {
		
		//Se comprueba si la fecha inicial del vehiculo es igual o posterior a la fecha de inicio del algoritmo
		if (Comprobar_Fecha_Inicial_Vehiculo(&Fecha_Inicial_Vehiculo, Datos_Vehiculos, Numero_Vehiculo, Fecha_Inicial_Algoritmo)==ERROR) {
			printf("Error la fecha inicial del vehiculo %d es incorrecta \n ", Numero_Vehiculo + 1);
			return ERROR;
		}

		//Se comprueba que la fecha final del vehiculo no es posterior a la fecha final del algoritmo
		if (Comprobar_Fecha_Final_Vehiculo(Datos_Vehiculos, &Fecha_Final_Vehiculo, Fecha_Final_Algoritmo, Numero_Vehiculo) == ERROR) {
			printf("Error la fecha final del vehiculo %d es incorrecta \n", Numero_Vehiculo + 1);
			return ERROR;
		}

		//Se comprueba que la fecha inicial no supera a la fecha final
		if (Comprobar_Orden(Fecha_Inicial_Vehiculo, Fecha_Final_Vehiculo) == ERROR) {
			printf("Error la fecha inicial de carga del vehiculo %d no puede ser posterior a la fecha final de carga \n",Numero_Vehiculo+1);
			return ERROR;
		}

	}
	return EXITO;
}
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
bool Comprobar_Validez_Modo_Carga(const char * Modo_Carga) {
	//Se utiliza este subprograma para comprobar que el modo de carga coincide con la palabra rapida o normal
	if (strcmp(Modo_Carga,"RAPIDA")== 0) {
		return true;
	}
	else if (strcmp(Modo_Carga, "NORMAL") == 0) {
		return true;
	}
	else {
		return false;
	}
}
static int Comprobar_Datos_Vehiculos(const Datos_CSV ***Datos_Vehiculos, const int Numero_Vehiculos) {
	//En este subprograma se comprueba que los datos de los diferentes vehiculos tienen sentido.
	double Capacidad_Bateria;
	double Maxima_Potencia;
	double Bateria_Inicial;
	double Bateria_Deseada;
	
	for (int Numero_Vehiculo = 0; Numero_Vehiculo < Numero_Vehiculos; Numero_Vehiculo++) {
		Capacidad_Bateria = Datos_Vehiculos[OFFSET_FILA_CSV_VEHICULOS + Numero_Vehiculo][COLUMNA_CSV_VEHICULOS_CAPACIDAD_BATERIA]->data.dbl;
		Maxima_Potencia = Datos_Vehiculos  [OFFSET_FILA_CSV_VEHICULOS + Numero_Vehiculo][COLUMNA_CSV_VEHICULOS_MAX_POTENCIA]->data.dbl;
		Bateria_Inicial = Datos_Vehiculos  [OFFSET_FILA_CSV_VEHICULOS + Numero_Vehiculo][COLUMNA_CSV_VEHICULOS_BAT_INICIAL]->data.dbl;
		Bateria_Deseada = Datos_Vehiculos  [OFFSET_FILA_CSV_VEHICULOS + Numero_Vehiculo][COLUMNA_CSV_VEHICULOS_BAT_FINAL]->data.dbl;

		if (!Comprobar_Validez_Datos(Capacidad_Bateria, Maxima_Potencia, Bateria_Inicial, Bateria_Deseada)) {
			printf("Los datos del vehiculo %d son incorrectos \n,", Numero_Vehiculo);
			return ERROR;
		}
   }
	return EXITO;
}

static int Comprobar_Precio_Inicial(const Datos_CSV*** Datos_Precio, const int Numero_Filas_Precio,
	                        const struct tm Fecha_Inicial_Algoritmo) {

	//Este subprograma se utiliza para comprobar que la hora inicial del algoritmo no es anterior a la primera 
	//fecha del CSV de los precios. Para asegurar que no hay horar que no tienen precio asignado

	struct tm Fecha_Inicial_Precio = { 0 };

	Fecha_Inicial_Precio.tm_year = Datos_Precio[PRIMERA_FILA_CSV_PRECIO][COLUMNA_CSV_ANYO_PRECIO]-OFFSET_ANYO_tm;
	Fecha_Inicial_Precio.tm_mon  = Datos_Precio[PRIMERA_FILA_CSV_PRECIO][COLUMNA_CSV_MES_PRECIO]-OFFSET_MES_tm;
	Fecha_Inicial_Precio.tm_mday = Datos_Precio[PRIMERA_FILA_CSV_PRECIO][COLUMNA_CSV_DIA_PRECIO];
	Fecha_Inicial_Precio.tm_hour = Datos_Precio[PRIMERA_FILA_CSV_PRECIO][COLUMNA_CSV_HORA_PRECIO];
	Fecha_Inicial_Precio.tm_min  = 0;
	Fecha_Inicial_Precio.tm_sec  = 0;

	int Resultado_Comparacion = Comprobar_Orden_Fechas(Fecha_Inicial_Algoritmo, Fecha_Inicial_Precio);

	if (Resultado_Comparacion < 0) {
		printf("La primera fecha del CSV de los precios no puede ser anterior a la fecha inicial del algoritmo \n");
		return ERROR;
	}

	//Se pasa a comprobar que el precio inicial sea superior a 0
	double Precio = Datos_Precio[PRIMERA_FILA_CSV_PRECIO][COLUMNA_CSV_PRECIO]->data.dbl;

	if (Precio<0) {
		return ERROR;
	}

	return EXITO;
}


static int Comprobar_Precio (const Datos_CSV*** Datos_Precio, const int Numero_Fila_Fecha_Primera,
	                                          const int Numero_Fila_Fecha_Segunda){
	
	//Este subprograma comprueba que la fecha que viene en la fila Numero_Fila_Fecha_Primera del CSV de los precios
	//solo se diferencia de la segunda en una hora, esto se utiliza para comprobar que las fechas del CSV de los precios
	//son consecutivas por horas.

    //Cargo las fechas a comprobar
	struct tm Fecha_1 = { 0 };
	struct tm Fecha_2 = { 0 };

	//Se carga la primera fecha a comprobar
	Fecha_1.tm_year = Datos_Precio[Numero_Fila_Fecha_Primera][COLUMNA_CSV_ANYO_PRECIO]->data.dbl - OFFSET_ANYO_tm;
	Fecha_1.tm_mon  = Datos_Precio[Numero_Fila_Fecha_Primera][COLUMNA_CSV_MES_PRECIO]->data.dbl  - OFFSET_MES_tm;
	Fecha_1.tm_mday = Datos_Precio[Numero_Fila_Fecha_Primera][COLUMNA_CSV_DIA_PRECIO]->data.dbl;
	Fecha_1.tm_hour = Datos_Precio[Numero_Fila_Fecha_Primera][COLUMNA_CSV_HORA_PRECIO]->data.dbl;
	Fecha_1.tm_min = 0;
	Fecha_1.tm_sec = 0;

    //Se carga la segunda fecha a comprobar
	Fecha_2.tm_year = Datos_Precio[Numero_Fila_Fecha_Segunda][COLUMNA_CSV_ANYO_PRECIO]->data.dbl - OFFSET_ANYO_tm;
	Fecha_2.tm_mon  = Datos_Precio[Numero_Fila_Fecha_Segunda][COLUMNA_CSV_MES_PRECIO]->data.dbl  - OFFSET_MES_tm;
	Fecha_2.tm_mday = Datos_Precio[Numero_Fila_Fecha_Segunda][COLUMNA_CSV_DIA_PRECIO]->data.dbl;
	Fecha_2.tm_hour = Datos_Precio[Numero_Fila_Fecha_Segunda][COLUMNA_CSV_HORA_PRECIO]->data.dbl;
	Fecha_2.tm_min  = 0;
	Fecha_2.tm_sec  = 0;

	time_t Fecha_1_Normalizada = mktime(&Fecha_1);
	time_t Fecha_2_Normalizada = mktime(&Fecha_2);
	double Diferencia_Tiempo = difftime(Fecha_1_Normalizada, Fecha_2_Normalizada);
	
	if (Diferencia_Tiempo!=SEGUNDOS_HORA){
		printf("Los precios no son consecutivos \n");
		return ERROR;
	}

	//Se comprueba que el valor del precio no sea negativo->
	double Precio = Datos_Precio[Numero_Fila_Fecha_Segunda][COLUMNA_CSV_PRECIO]->data.dbl;
	if (Precio < 0) {
		printf("No puede haber precios negativos \n");
		return ERROR;
	}

	return EXITO;

}
static int Comprobar_Precios_CSV(const Datos_CSV ***Datos_Precio, const int Numero_Filas_Precio) {
	//Se utiliza este subprograma para comprobar que las fechas de los diferentes precios son consecutivos, 
	//y que los valores de los precios son superiores o igual a 0.

	for (int Numero_Fila_CSV = 1; Numero_Fila_CSV < Numero_Filas_Precio-1; Numero_Fila_CSV++) {
		Comprobar_Precio(Datos_Precio, Numero_Fila_CSV,Numero_Fila_CSV+1);

	}
}

static int Comprobar_Precio_Final(const Datos_CSV*** Datos_Precio, const int Numero_Filas_CSV,
	                              const Datos_CSV*** Datos_Tiempo) {

	//En este subprograma se comprueba que el ultimo precio del CSV o bien coincide con la fecha final del
	//algoritmo o bien es una fecha posterior. Para asegurar que esta cubierto
    //la fecha final de ejecucion del algoritmo en el CSV de lor precios.

	struct tm Fecha_Final_Algoritmo = { 0 };
	Fecha_Final_Algoritmo.tm_year = Datos_Tiempo[FILA_INFORMACION_TEMPORAL_ALGORITMO][COLUMNA_ANYO_INICIAL_ALGORITMO];
	Fecha_Final_Algoritmo.tm_mon  = Datos_Tiempo[FILA_INFORMACION_TEMPORAL_ALGORITMO][COLUMNA_MES_INICIAL_ALGORITMO];
	Fecha_Final_Algoritmo.tm_mday = Datos_Tiempo[FILA_INFORMACION_TEMPORAL_ALGORITMO][COLUMNA_DIA_INICIAL_ALGORITMO];
	Fecha_Final_Algoritmo.tm_hour = Datos_Tiempo[FILA_INFORMACION_TEMPORAL_ALGORITMO][COLUMNA_HORA_INICIAL_ALGORITMO];
	Fecha_Final_Algoritmo.tm_min  = 0;
	Fecha_Final_Algoritmo.tm_sec  = 0;

	struct tm Fecha_Final_Precio = { 0 };
	Fecha_Final_Precio.tm_year   = Datos_Precio[][];
	Fecha_Final_Precio.tm_mon    = Datos_Precio[][];
	Fecha_Final_Precio.tm_mday   = Datos_Precio[][];
	Fecha_Final_Precio.tm_hour   = Datos_Precio[][];

}
static int Comprobar_Precios(const Datos_CSV ***Datos_Precio_Compra, const Datos_CSV ***Datos_Precio_Venta,
	                         const int Numero_Filas_Precio_Compra,const int Numero_Filas_Precio_Venta,
	                         const Datos_CSV ***Datos_Tiempo_Algoritmo) {
	
	//En este subprograma se comprueba que los precios de compra del kilovatio hora y de venta del kilovatio hora
	//son correctos
	
	struct tm Fecha_Inicial_Algoritmo;
	Cargar_Fecha_Inicial_Algoritmo(Datos_Tiempo_Algoritmo, &Fecha_Inicial_Algoritmo);

	if ((Numero_Filas_Precio_Compra <= 1) ||(Numero_Filas_Precio_Venta <=1)) {
		printf("No hay datos suficientes en el CSV que contiene los precios del kilovatio hora");
		return ERROR;
	}
	

	//Se pasa a comprobar si los datos de los CSV de los precios de compra y de venta tienen sentido

	if (Comprobar_Precio_Inicial(Datos_Precio_Compra,Numero_Filas_Precio_Compra,Fecha_Inicial_Algoritmo) ==ERROR) {
		return ERROR;
	}

	if (Comprobar_Precio_Inicial(Datos_Precio_Venta, Numero_Filas_Precio_Venta, Fecha_Inicial_Algoritmo) == ERROR) {
		return ERROR;
	}

	//Se pasa a comprobar que todos los precios tengan fechas consecutivas una hora despues de la otra, y que 
	//no haya precios negativos.
	if (Comprobar_Precios_CSV(Datos_Precio_Compra, Numero_Filas_Precio_Compra) == ERROR) {
		return ERROR;
	}

	if (Comprobar_Precios_CSV(Datos_Precio_Venta, Numero_Filas_Precio_Venta) == ERROR) {
		return ERROR;
	}

	//Se comprueba que en el CSV de los precios esta incluido la fecha final del algoritmo

	return EXITO;
}


static int Verificar_Tiempo_Algoritmo (const Datos_CSV *** Datos_Tiempo) {

	//Este subprograma se utiliza para verificar que los datos del CSV donde vienen los datos temporales de la
	//simulacion correctos, la fecha de inicio del algoritmo, la fecha de finalizacion. Asi como la diferencia en
	//minutos entre los puntos de la simulacion.
	

	//Defino una serie de variables auxiliares 
	struct tm Fecha_Inicial;
	struct tm Fecha_Final;
	//Cargo la fecha inicial y final de la simulacion.
	Cargar_Fecha_Inicial_Algoritmo (Datos_Tiempo, &Fecha_Inicial);
	Cargar_Fecha_Final_Algoritmo   (Datos_Tiempo, &Fecha_Final);
	//Compruebo si la fecha inicial y final tienen sentido
	
	int Resultado_Comparacion = Comprobar_Orden_Fechas(Fecha_Inicial,Fecha_Final);

	if (Resultado_Comparacion > 0) {
		printf("Error La fecha inicial del algoritmo no puede ser posterior a la fecha final \n");
		return ERROR;
	}
	if (Resultado_Comparacion == 0) {
		printf("Error la fecha inicial del algoritmo no puede ser igual a la fecha final \n");
		return ERROR;
	}

	//Se comprueba si la diferencia en minutos entre los puntos de la simulacion es correcto->

	int Delta_Simulacion;

	Delta_Simulacion = Datos_Tiempo[FILA_INFORMACION_TEMPORAL_ALGORITMO][COLUMNA_DELTA_T];

	//La diferencia en minutos entre los puntos de la simulacion ha de ser entre 1 y 60.
	if ((Delta_Simulacion < 1) || (Delta_Simulacion > 60)) {
		printf("Error el delta de simulacion ha de estar entre 1 y 60 minutos \n");
		return ERROR;
	}

	return EXITO;
}

static int Comprobar_Fase(const Datos_CSV*** Datos_Terminales,const int Fila_Terminal) {
	//Se utiliza este subprograma para comprobar la fase del terminal->
	char* Fase = Datos_Terminales[Fila_Terminal][COLUMNA_CSV_FASE_TERMINAL];
	
	if  (strcmp(Fase,"R")      == 0 || strcmp(Fase,"S")      == 0 || strcmp (Fase,"T") ==0 ||
	     strcmp(Fase,"NEUTRO") == 0 || strcmp (Fase, "NADA") == 0){   
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
	if (Numero_Terminal-Terminal_Anterior!=1) {
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
static int Verificar_Datos_Terminales(const Datos_CSV ***Datos_Terminales, const int Numero_Filas_Terminales) {
	//En este subprograma se verifica que los datos del CSV de los terminales son correctos.
	
	if (Numero_Filas_Terminales != 13) {
		printf("El numero de filas debe ser 13, la primera para la informacion las 12 siguientes para los terminales \n");
		return ERROR;
	}

	//Se crean variables auxiliares
	int Terminal_Anterior = 0;
	int Terminal_Actual ;

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

int Verificar_Entradas( const Datos_CSV *** Datos_Tiempo,           const Datos_CSV  *** Datos_Vehiculos,
	                    const Datos_CSV *** Datos_Bateria,          const Datos_CSV *** Datos_Precio_Compra,
                        const Datos_CSV *** Datos_Precio_Venta,     const Datos_CSV *** Datos_Terminales,
	                    const int Numero_Filas_CSV_Tiempo,          const int Numero_Filas_CSV_Vehiculos,       
	                    const int Numero_Filas_CSV_Bateria,
	                    const int Numero_Filas_Precio_Compra,       const int Numero_Filas_Precio_Venta,
	                    const int Numero_Columnas_CSV_Tiempo,       const int Numero_Columnas_CSV_Vehiculos,
	                    const int Numero_Columnas_CSV_Bateria,      const int Numero_Columnas_CSV_Precio_Compra,
	                    const int Numero_Columnas_CSV_Precio_Venta, const int Numero_Filas_Terminales) {



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

		                  
}