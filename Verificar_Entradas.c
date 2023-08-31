#include "Definiciones_Globales.h"
#include "Funciones_Auxiliares.h"
#include "time.h"
#include "Tipos_Optimizacion.h"
#include <stdlib.h>
#include <stdbool.h>
static void Cargar_Fecha_Inicial_Algoritmo(const Datos_CSV*** Datos_Tiempo, struct tm* Fecha_Inicial) {
	//Este subprograma se utiliza para cargar en la variable Fecha_Inicial la fecha inicial 
	Fecha_Inicial->tm_year = Datos_Tiempo[FILA_INFORMACION_TEMPORAL_ALGORITMO][COLUMNA_ANYO_INICIAL_ALGORITMO]->data.dbl-1900;
	Fecha_Inicial->tm_mon  = Datos_Tiempo[FILA_INFORMACION_TEMPORAL_ALGORITMO][COLUMNA_MES_INICIAL_ALGORITMO]->data.dbl-1;
	Fecha_Inicial->tm_mday = Datos_Tiempo[FILA_INFORMACION_TEMPORAL_ALGORITMO][COLUMNA_DIA_INICIAL_ALGORITMO]->data.dbl;
	Fecha_Inicial->tm_hour = Datos_Tiempo[FILA_INFORMACION_TEMPORAL_ALGORITMO][COLUMNA_HORA_INICIAL_ALGORITMO]->data.dbl;
	Fecha_Inicial->tm_min  = Datos_Tiempo[FILA_INFORMACION_TEMPORAL_ALGORITMO][COLUMNA_MINUTO_INICIAL_ALGORITMO]->data.dbl;
	Fecha_Inicial->tm_sec  = 0;
}

static void Cargar_Fecha_Final_Algoritmo(const Datos_CSV*** Datos_Tiempo, struct tm* Fecha_Final) {
	//Este subprograma se utiliza para cargar la fecha final en la variable fecha final
	Fecha_Final->tm_year = Datos_Tiempo[FILA_INFORMACION_TEMPORAL_ALGORITMO][COLUMNA_ANYO_FINAL_ALGORITMO]->data.dbl-1900;
	Fecha_Final->tm_mon  = Datos_Tiempo[FILA_INFORMACION_TEMPORAL_ALGORITMO][COLUMNA_MES_FINAL_ALGORITMO]->data.dbl-1;
	Fecha_Final->tm_mday = Datos_Tiempo[FILA_INFORMACION_TEMPORAL_ALGORITMO][COLUMNA_DIA_FINAL_ALGORITMO]->data.dbl;
	Fecha_Final->tm_hour = Datos_Tiempo[FILA_INFORMACION_TEMPORAL_ALGORITMO][COLUMNA_HORA_FINAL_ALGORITMO]->data.dbl;
	Fecha_Final->tm_min  = Datos_Tiempo[FILA_INFORMACION_TEMPORAL_ALGORITMO][COLUMNA_MINUTO_FINAL_ALGORITMO]->data.dbl;
	Fecha_Final->tm_sec  = 0;
}

static void Cargar_Fecha_Inicial_Vehiculo(const Datos_CSV*** Datos_Vehiculos, struct tm* Fecha_Inicial_Vehiculo,
	                                      const int Numero_Vehiculo) {
	//Se carga la fecha inicial de carga del vehiculo en la variable Fecha_Inicial_Vehiculo
	Fecha_Inicial_Vehiculo->tm_year = Datos_Vehiculos[OFSET_CSV_VEHICULOS  + Numero_Vehiculo][COLUMNA_CSV_VEHICULOS_ANYO_INICIAL]->data.dbl - 1900;
	Fecha_Inicial_Vehiculo->tm_mon = Datos_Vehiculos [OFSET_CSV_VEHICULOS  + Numero_Vehiculo][COLUMNA_CSV_VEHICULOS_MES_INICIAL]->data.dbl - 1;
	Fecha_Inicial_Vehiculo->tm_mday = Datos_Vehiculos[OFSET_CSV_VEHICULOS  + Numero_Vehiculo][COLUMNA_CSV_VEHICULOS_DIA_INICIAL]->data.dbl;
	Fecha_Inicial_Vehiculo->tm_hour = Datos_Vehiculos[OFSET_CSV_VEHICULOS  + Numero_Vehiculo][COLUMNA_CSV_VEHICULOS_HORA_INICIAL]->data.dbl;
	Fecha_Inicial_Vehiculo->tm_min = Datos_Vehiculos [OFSET_CSV_VEHICULOS  + Numero_Vehiculo][COLUMNA_CSV_VEHICULOS_MINUTO_INICIAL]->data.dbl;
	Fecha_Inicial_Vehiculo->tm_sec = 0;
}
static void Cargar_Fecha_Final_Vehiculo(const Datos_CSV*** Datos_Vehiculos, struct tm* Fecha_Final_Vehiculo,
	                                    const int Numero_Vehiculo) {
	//Se carga la fecha final de carga del vehiculo en la variable Fecha_Final_Vehiculo
	Fecha_Final_Vehiculo->tm_year = Datos_Vehiculos[OFSET_CSV_VEHICULOS  + Numero_Vehiculo][COLUMNA_CSV_VEHICULOS_ANYO_FINAL]->data.dbl - 1900;
	Fecha_Final_Vehiculo->tm_mon = Datos_Vehiculos [OFSET_CSV_VEHICULOS  + Numero_Vehiculo][COLUMNA_CSV_VEHICULOS_MES_FINAL]->data.dbl - 1;
	Fecha_Final_Vehiculo->tm_mday= Datos_Vehiculos [OFSET_CSV_VEHICULOS  + Numero_Vehiculo][COLUMNA_CSV_VEHICULOS_DIA_FINAL]->data.dbl;
	Fecha_Final_Vehiculo->tm_hour = Datos_Vehiculos[OFSET_CSV_VEHICULOS  + Numero_Vehiculo][COLUMNA_CSV_VEHICULOS_HORA_FINAL]->data.dbl;
	Fecha_Final_Vehiculo->tm_min = Datos_Vehiculos [OFSET_CSV_VEHICULOS  + Numero_Vehiculo][COLUMNA_CSV_VEHICULOS_MINUTO_FINAL]->data.dbl;
	Fecha_Final_Vehiculo->tm_sec = 0;
}

static int Comprobar_Fecha_Vehiculos(const struct tm Fecha_Inicial_Algoritmo, const struct tm Fecha_Final_Algoritmo,
	                                  const int Numero_Vehiculos, const Datos_CSV ***Datos_Vehiculos) {
	//En este subprograma se comprueba que las fechas de inicio y finalizacion de carga de los vehiculos tienen
	//sentido logico, que la fecha inicial no es poterior a la final y que esta dentro de la ventana temporal
	//en la cual se ejecuta el algoritmo.
	//Creo variables Auxiliares
	struct tm Fecha_Inicial_Vehiculo;
	struct tm Fecha_Final_Vehiculo;
	int       Variable_Control;
	for (int Numero_Vehiculo = 0; Numero_Vehiculo < Numero_Vehiculos; Numero_Vehiculo++) {
		//Se comprueba si la fecha inicial del vehiculo es igual o posterior a la fecha de inicio del algoritmo
		Cargar_Fecha_Inicial_Vehiculo(Datos_Vehiculos, &Fecha_Inicial_Vehiculo, Numero_Vehiculo);
		Variable_Control = Comprobar_Orden_Fechas(Fecha_Inicial_Algoritmo, Fecha_Inicial_Vehiculo);
		if (Variable_Control > 0) {
			printf("La fecha inicial del vehiculo no puede ser anterior a la fecha inicial del algoritmo %d \n",Numero_Vehiculo);
			return ERROR;
		}
		//Se comprueba que la fecha final del vehiculo no es posterior a la fecha final del algoritmo
		Cargar_Fecha_Final_Vehiculo(Datos_Vehiculos,&Fecha_Final_Vehiculo, Numero_Vehiculo);
		Variable_Control = Comprobar_Orden_Fechas(Fecha_Final_Algoritmo,Fecha_Final_Vehiculo);
		if (Variable_Control < 0) {
			printf("Error el algoritmo no puede acabar antes que la carga del vehiculo %d \n",Numero_Vehiculo);
			return ERROR;
		}
		//Se comprueba que la fecha inicial no supera a la fecha final
		Variable_Control = Comprobar_Orden_Fechas(Fecha_Inicial_Vehiculo, Fecha_Final_Vehiculo);
		if (Variable_Control<0) {
			printf("Error la fecha inicial de carga del vehiculo no puede ser posterior a la fecha final de carga %d \n",Numero_Vehiculo);
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
static int Comprobar_Fecha_Baterias(const struct tm Fecha_Inicial_Algoritmo,const struct tm Fecha_Final_Algoritmo,
	                               const int Numero_Baterias, const Datos_CSV ***Datos_Baterias) {
	
	//En este subprograma se comprueba que las fechas de inicio y finalizacion de las baterias tienen sentido
	//Creo variables Auxiliares
	struct tm Fecha_Inicial_Bateria;
	struct tm Fecha_Final_Bateria;
	struct tm Fecha_Objetivo_Bateria;
	int       Variable_Control;
	for (int i = 0; i < Numero_Baterias; i++) {
		//Se carga la fecha inicial.
		Fecha_Inicial_Bateria.tm_year = Datos_Baterias[OFSET_CSV_VEHICULOS + i][COLUMNA_CSV_BATERIAS_ANYO_INICIAL]->data.dbl-1900;
		Fecha_Inicial_Bateria.tm_mon =  Datos_Baterias[OFSET_CSV_VEHICULOS + i][COLUMNA_CSV_BATERIAS_MES_INICIAL]->data.dbl-1;
		Fecha_Inicial_Bateria.tm_mday = Datos_Baterias[OFSET_CSV_VEHICULOS + i][COLUMNA_CSV_BATERIAS_DIA_INICIAL]->data.dbl;
		Fecha_Inicial_Bateria.tm_hour = Datos_Baterias[OFSET_CSV_VEHICULOS + i][COLUMNA_CSV_BATERIAS_HORA_INICIAL]->data.dbl;
		Fecha_Inicial_Bateria.tm_min =  Datos_Baterias[OFSET_CSV_VEHICULOS + i][COLUMNA_CSV_BATERIAS_MINUTO_INICIAL]->data.dbl;
		Fecha_Inicial_Bateria.tm_sec = 0;
		Variable_Control = Comprobar_Orden_Fechas(Fecha_Inicial_Algoritmo, Fecha_Inicial_Bateria);
		if (Variable_Control > 0) {
			printf("La fecha inicial del vehiculo no puede ser anterior a la fecha inicial del algoritmo \n");
			return -1;
		}
		//Se comprueba si la fecha inicial del vehiculo es igual o posterior a la fecha de inicio del algoritmo
		Fecha_Final_Bateria.tm_year = Datos_Baterias[OFSET_CSV_VEHICULOS + i][COLUMNA_CSV_BATERIAS_ANYO_FINAL]->data.dbl-1900;
		Fecha_Final_Bateria.tm_mon =  Datos_Baterias[OFSET_CSV_VEHICULOS + i][COLUMNA_CSV_BATERIAS_MES_FINAL]->data.dbl-1;
		Fecha_Final_Bateria.tm_mday = Datos_Baterias[OFSET_CSV_VEHICULOS + i][COLUMNA_CSV_BATERIAS_DIA_FINAL]->data.dbl;
		Fecha_Final_Bateria.tm_hour = Datos_Baterias[OFSET_CSV_VEHICULOS + i][COLUMNA_CSV_BATERIAS_HORA_FINAL]->data.dbl;
		Fecha_Final_Bateria.tm_min =  Datos_Baterias[OFSET_CSV_VEHICULOS + i][COLUMNA_CSV_BATERIAS_MINUTO_FINAL]->data.dbl;
		Fecha_Final_Bateria.tm_sec = 0;
		Variable_Control = Comprobar_Orden_Fechas(Fecha_Final_Algoritmo, Fecha_Final_Bateria);
		if (Variable_Control < 0) {
			printf("Error el algoritmo no puede acabar antes que la carga de la bateria \n");
			return -1;
		}
		//Se carga la fecha final
		Variable_Control = Comprobar_Orden_Fechas(Fecha_Inicial_Bateria, Fecha_Final_Bateria);
		if (Variable_Control < 0) {
			printf("Error la fecha inicial de carga de la bateria no puede ser posterior a la fecha final de carga \n");
			return -1;
		}
		//Si se considera que para una determinada hora se desea que la bateria este cargada a un cierto nivel

		if (Considerar_Objetivo_Bateria(Datos_Baterias[OFSET_CSV_VEHICULOS+i][COLUMNA_CSV_BATERIAS_OBJETIVO_CARGA]->data.str)) {
		//Se comprueba si la fecha objetivo tiene sentido->
			Fecha_Final_Bateria.tm_year = Datos_Baterias[OFSET_CSV_VEHICULOS + i][COLUMNA_CSV_BATERIAS_ANYO_FINAL]->data.dbl - 1900;
			Fecha_Final_Bateria.tm_mon = Datos_Baterias[OFSET_CSV_VEHICULOS + i][COLUMNA_CSV_BATERIAS_MES_FINAL]->data.dbl - 1;
			Fecha_Final_Bateria.tm_mday = Datos_Baterias[OFSET_CSV_VEHICULOS + i][COLUMNA_CSV_BATERIAS_DIA_FINAL]->data.dbl;
			Fecha_Final_Bateria.tm_hour = Datos_Baterias[OFSET_CSV_VEHICULOS + i][COLUMNA_CSV_BATERIAS_HORA_FINAL]->data.dbl;
			Fecha_Final_Bateria.tm_min = Datos_Baterias[OFSET_CSV_VEHICULOS + i][COLUMNA_CSV_BATERIAS_MINUTO_FINAL]->data.dbl;
			Fecha_Final_Bateria.tm_sec = 0;
		}

	}
	return 0;
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
	if (strcmp(Modo_Carga,"rapida")== 0) {
		return true;
	}
	else if (strcmp(Modo_Carga, "normal") == 0) {
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
	
	for (int i = 0; i < Numero_Vehiculos; i++) {
		Capacidad_Bateria = Datos_Vehiculos[OFSET_CSV_VEHICULOS + i][COLUMNA_CSV_VEHICULOS_CAPACIDAD_BATERIA]->data.dbl;
		Maxima_Potencia = Datos_Vehiculos[OFSET_CSV_VEHICULOS + i][COLUMNA_CSV_VEHICULOS_MAX_POTENCIA]->data.dbl;
		Bateria_Inicial = Datos_Vehiculos[OFSET_CSV_VEHICULOS + i][COLUMNA_CSV_VEHICULOS_BAT_INICIAL]->data.dbl;
		Bateria_Deseada = Datos_Vehiculos[OFSET_CSV_VEHICULOS + i][COLUMNA_CSV_VEHICULOS_BAT_FINAL]->data.dbl;

		if (!Comprobar_Validez_Datos(Capacidad_Bateria, Maxima_Potencia, Bateria_Inicial, Bateria_Deseada)) {
			printf("Los datos del vehiculo %d son incorrectos \n,", i);
			return -1;
		}
   }
	return 0;
}
static int Verificar_Tiempo_Simulacion(const Datos_CSV *** Datos_Tiempo,   const Datos_CSV ***Datos_Vehiculos,
	                                   const Datos_CSV *** Datos_Baterias, const Datos_CSV ***Datos_Precio_Compra,
	                                   const Datos_CSV *** Datos_Precio_Venta) {

	//Este subprograma se utiliza para verificar que los datos temporales de los datos de entrada son correctos
	//es decir cosas como que la fecha inicial sea antes o igual a la final, que las fechas de vehiculos y baterias
	//esten dentro del intervalo temporal de calculo asi como los precios.
	

	//Defino una serie de variables auxiliares 
	struct tm Fecha_Inicial;
	struct tm Fecha_Final;
	//Cargo la fecha inicial y final de la simulacion.
	Cargar_Fecha_Inicial(Datos_Tiempo, &Fecha_Inicial);
	Cargar_Fecha_Final(Datos_Tiempo,   &Fecha_Final);
	//Compruebo si la fecha inicial y final tienen sentido
	int Variable_Control = Comprobar_Orden_Fechas(Fecha_Inicial,Fecha_Final);

	if (Variable_Control > 0) {
		printf("Error La fecha inicial del algoritmo no puede ser posterior a la fecha final \n");
		return -1;
	}
	if (Variable_Control == 0) {
		printf("Error la fecha inicial del algoritmo no puede ser posterior a la fecha final \n");
		return -1;
	}
	return 0;
}

void Verificar_Informacion_Entrada() {
	
}