
#include "Comprobar_Informacion.h"
#include "Configurar_Ajustes_Problema.h"
#include "osqp.h"
#include "string.h"
#include <stdbool.h>
#include <stdlib.h>
#include "Tipos_Optimizacion.h"
#include "time.h"

//Defino en que columnas del CSV se encuentran los parametros de informacion de los vehiculos
#define  COL_NUMERO_TERMINAL   0
#define  COL_TIPO_DISPOSITIVO  1
#define  COL_MODO_CARGA        3
#define  COL_CAPACIDAD_BATERIA 4
#define  COL_BATERIA_INICIAL   5
#define  COL_BATERIA_FINAL     6
#define  COL_MAXIMA_POTENCIA   7
#define  COL_ANYO_INI_CARGA    8
#define  COL_MES_INI_CARGA     9
#define  COL_DIA_INI_CARGA     10
#define  COL_HORA_INI_CARGA    11
#define  COL_MINUTO_INI_CARGA  12
#define  COL_ANYO_FIN_CARGA    13
#define  COL_MES_FIN_CARGA     14
#define  COL_DIA_FIN_CARGA     15
#define  COL_HORA_FIN_CARGA    16
#define  COL_MINUTO_FIN_CARGA  17
//Defino las columnas donde se encuentra la informacion de los terminales
#define  COL_FASE              1


//Utilizo este subprograma para poder reservar memoria para la variable que almacena la informacion de los elementos presentes en la electrolinera.
int Inicializar_Cargas_Electrolinera(Elementos_Electrolinera **Informacion_Sistema,const int Numero_Puntos) {
	* Informacion_Sistema = (Elementos_Electrolinera*) malloc (sizeof(Elementos_Electrolinera));
	
	if (*Informacion_Sistema == NULL) {
		return -1;
	}
	(*Informacion_Sistema)->Numero_Puntos = Numero_Puntos;
	(*Informacion_Sistema)->Numero_Vehiculos = 0;
	(*Informacion_Sistema)->Numero_Baterias = 0;
	(*Informacion_Sistema)->Vehiculos_Sistema = NULL;
	(*Informacion_Sistema)->Baterias_Sistema = NULL;
	return 0;
}


//Este subprograma se utiliza para determinar si una fecha es anterior o posterior a otra
//En funcion de si el numero que devuelve es superior a 0 inferior a 0 o igual a 0

int Comparar_Fechas(struct tm Fecha1, struct tm Fecha2) {
	

	

	time_t t1 = mktime(&Fecha1);
	time_t t2 = mktime(&Fecha2);
	if (t1 < t2) return -1;    //Fecha 2 superior
	if (t1 > t2) return 1;    //Fecha 1 superior
	return 0;                  //Fechas iguales
} 

//Este subprograma se utiliza para encontrar la posicion en el array que relaciona los puntos de la 
//simulacion con la fecha. Para poder asi hallar con precision cual es el punto de inicio y el punto de
//finalizacion de la simulacion->

int Busqueda_Binaria(Puntos_Optimizacion *Array_Puntos_Simulacion, const int size,const struct tm Fecha_Objetivo) {
	//En este programa de busqueda se va comprobando si la fecha que se va recorriendo del array está por
	//encima o por debajo de la fecha objetivo->


	int Index_Bajo = 0;
	int Index_Alto = size - 1;
	while (Index_Bajo<=Index_Alto) {
		int Index_Medio = (Index_Bajo + Index_Alto) / 2;
		int Comparacion = Comparar_Fechas(Fecha_Objetivo, Array_Puntos_Simulacion[Index_Medio].Fecha);

		if (Comparacion == 0) {
			
			return Index_Medio;
		}
		else if (Comparacion < 0) {
			Index_Alto = Index_Medio - 1;
		}
		else {
			Index_Bajo = Index_Medio + 1;
		}
	}
	return -1; //No se ha encontrado la fecha objetivo.
}

//Este subprograma se utiliza para crear el array que contiene los datos de los diferentes vehiculos, presentes
//en la electrolinera->
int Reservar_Memoria_Vehiculos(Elementos_Electrolinera *Informacion_Sistema) {
	int Numero_Vehiculos = Informacion_Sistema->Numero_Vehiculos;
	Informacion_Sistema->Vehiculos_Sistema = (Vehiculos*)malloc(Numero_Vehiculos * sizeof(Vehiculos));
	if (!Informacion_Sistema->Vehiculos_Sistema) {
		printf("La reserva de memoria ha fallado \n");
		return -1;
	}
	return 0;
}

Tipo_Carga Obtener_Modo_Carga(const char *Modo_Carga) {
	if ((strcmp(Modo_Carga, "rapida") == 0)||(strcmp(Modo_Carga,"Rapida")==0)) {

		return CARGA_RAPIDA;
	}
	else if ((strcmp(Modo_Carga, "normal") == 0)||(strcmp(Modo_Carga,"Normal")==0)) {
		
		return CARGA_NORMAL;
	}

}


int   Obtener_Numero_Terminal(const Celda ***Data_CSV, const int N_Fila_CSV) {
	//Este subprogama se utiliza para identificar a que numero de terminal estan conectados los vehiculos.
	int Numero_Terminal;
	return  Numero_Terminal = Data_CSV[N_Fila_CSV][COL_NUMERO_TERMINAL]->data.dbl;
}


char Obtener_Fase_Electrica (const Celda ***Data_Terminales, const int Numero_Terminal) {
	char fase;
	
	
	return fase = *(Data_Terminales[Numero_Terminal][COL_FASE]->data.str);
}


void  Obtener_Fase_Vehiculo(Elementos_Electrolinera *Informacion_Sistema, const Celda ***Data_Vehiculos,
	                        const Celda *** Data_Terminales, const int Numero_Vehiculo, const int N_Fila_CSV) {
	//Este subprorgama se utiliza para identificar a que fase de la red esta conectado el vehiculo en cuestion->
	// R S o T
	//Primero es necesario identificar a que numero de terminal esta conectado el vehiculo->
	int Numero_Terminal = Obtener_Numero_Terminal(Data_Vehiculos,N_Fila_CSV);
	//Imprimo por pantalla el numero de terminal->
	printf("El numero de terminal en cuestion es \n");
	printf("%d", Numero_Terminal);
	Informacion_Sistema->Vehiculos_Sistema[Numero_Vehiculo].Numero_Terminal=Numero_Terminal;
	//Segundo es necesario identificar la a fase a la que esta conectada dicho terminal->
	char fase = Obtener_Fase_Electrica(Data_Terminales, Numero_Terminal);
	printf("La fase leida es -> \n");
	printf("%c", fase);
	//Incluyo la fase en la que esta conectada en el vehiculo correspondiente
	Informacion_Sistema->Vehiculos_Sistema[Numero_Vehiculo].Fase = fase;
}


void Incluir_Modo_Carga(Elementos_Electrolinera *Informacion_Sistema, const int Numero_Vehiculo, const int Numero_Fila_CSV,
	                    const Celda ***Data_Vehiculos) {
	//Este subprograma se utiliza para identificar el modo de carga a la que esta sometida el vehiculo, carga rapida
	//o normal->
	Tipo_Carga Tipo = Obtener_Modo_Carga(Data_Vehiculos[Numero_Fila_CSV][COL_MODO_CARGA]->data.str);
	//Se almacena un 0 en el campo correspondiente si la carga es rapida
	//Se almacena un 1 en el campo correspondiente si la carga es normal
	Informacion_Sistema->Vehiculos_Sistema[Numero_Vehiculo].Modo_Carga = Tipo;
	
}


void Incluir_Punto_Inicial_Final(Elementos_Electrolinera *Informacion_Sistema,Puntos_Optimizacion *Array_Puntos_Simulacion,
	                             const int Numero_Vehiculo,const Celda ***Data_Vehiculos,const int Numero_Fila_CSV,
	                             const int Numero_Puntos) {
	//Este subprograma se utiliza para calcular el punto inicial y final de los vehiculos de la simulacion
	//Se definen variables para almacenar el inicio y finalizacion de la carga->
	struct tm Fecha_Inicio_Vehiculo;
	struct tm Fecha_Fin_Vehiculo;
	//Cargo las fechas de inicio y finalizacion de la carga de los vehiculos->
	Fecha_Inicio_Vehiculo.tm_year = Data_Vehiculos[Numero_Fila_CSV][COL_ANYO_INI_CARGA]->data.dbl-1900;
	Fecha_Inicio_Vehiculo.tm_mon =  Data_Vehiculos[Numero_Fila_CSV][COL_MES_INI_CARGA]->data.dbl-1;
	Fecha_Inicio_Vehiculo.tm_mday = Data_Vehiculos[Numero_Fila_CSV][COL_DIA_INI_CARGA]->data.dbl;
	Fecha_Inicio_Vehiculo.tm_hour = Data_Vehiculos[Numero_Fila_CSV][COL_HORA_INI_CARGA]->data.dbl;
	Fecha_Inicio_Vehiculo.tm_min =  Data_Vehiculos[Numero_Fila_CSV][COL_MINUTO_INI_CARGA]->data.dbl;
	Fecha_Inicio_Vehiculo.tm_sec =  0;
	//Cargo la fecha de finalizacion de la carga->
	Fecha_Fin_Vehiculo.tm_year = Data_Vehiculos[Numero_Fila_CSV][COL_ANYO_FIN_CARGA]->data.dbl-1900;
	Fecha_Fin_Vehiculo.tm_mon =  Data_Vehiculos[Numero_Fila_CSV][COL_MES_FIN_CARGA]->data.dbl-1;
	Fecha_Fin_Vehiculo.tm_mday = Data_Vehiculos[Numero_Fila_CSV][COL_DIA_FIN_CARGA]->data.dbl;
	Fecha_Fin_Vehiculo.tm_hour = Data_Vehiculos[Numero_Fila_CSV][COL_HORA_FIN_CARGA]->data.dbl;
	Fecha_Fin_Vehiculo.tm_min =  Data_Vehiculos[Numero_Fila_CSV][COL_MINUTO_FIN_CARGA]->data.dbl;
	Fecha_Fin_Vehiculo.tm_sec = 0;


	
	int Index_Fecha_Inicio = Busqueda_Binaria(Array_Puntos_Simulacion, Numero_Puntos, Fecha_Inicio_Vehiculo);
	
	if (Index_Fecha_Inicio != -1) {
		Informacion_Sistema->Vehiculos_Sistema[Numero_Vehiculo].Punto_Inicial_Carga = Index_Fecha_Inicio;
	}
	int Index_Fecha_Fin    = Busqueda_Binaria(Array_Puntos_Simulacion, Numero_Puntos, Fecha_Fin_Vehiculo);
	if (Index_Fecha_Fin != -1) {
		Informacion_Sistema->Vehiculos_Sistema[Numero_Vehiculo].Punto_Final_Carga = Index_Fecha_Fin;
	}
	
}


void Incluir_Datos_CSV   (Elementos_Electrolinera *Informacion_Sistema, const Celda*** Data_Vehiculos, 
	                     const int Numero_Fila, const int  Numero_Vehiculo) {
	//Este subprograma se utiliza para incluir los datos del vehiculo que se pueden leer facilemente del CSV.
	Informacion_Sistema->Vehiculos_Sistema[Numero_Vehiculo].Capacidad_Bateria = Data_Vehiculos[Numero_Fila][COL_CAPACIDAD_BATERIA]->data.dbl;
	Informacion_Sistema->Vehiculos_Sistema[Numero_Vehiculo].Bateria_Inicial =   Data_Vehiculos[Numero_Fila][COL_BATERIA_INICIAL]->data.dbl;
	Informacion_Sistema->Vehiculos_Sistema[Numero_Vehiculo].Bateria_Final   =   Data_Vehiculos[Numero_Fila][COL_BATERIA_FINAL]->data.dbl;
	Informacion_Sistema->Vehiculos_Sistema[Numero_Vehiculo].Maxima_Potencia =   Data_Vehiculos[Numero_Fila][COL_MAXIMA_POTENCIA]->data.dbl;
}


//Subprograma que se encarga de incluir la informacion del vehiculo.
void Incluir_Vehiculo(const int Numero_Fila_CSV, Elementos_Electrolinera* Informacion_Sistema, const int Numero_Vehiculo,
	                  const Celda *** Data_Vehiculos, Puntos_Optimizacion *Array_Puntos_Simulacion,
	                  const int Numero_Puntos, const Celda ***Data_Terminales) {
	//Se va añadiendo la informacion del vehiculo correspondiente->
	Incluir_Datos_CSV(Informacion_Sistema, Data_Vehiculos, Numero_Fila_CSV, Numero_Vehiculo);
	Incluir_Punto_Inicial_Final(Informacion_Sistema, Array_Puntos_Simulacion, Numero_Vehiculo, Data_Vehiculos, 
		Numero_Fila_CSV,Numero_Puntos);
	Incluir_Modo_Carga(Informacion_Sistema, Numero_Vehiculo, Numero_Fila_CSV, Data_Vehiculos);
	Obtener_Fase_Vehiculo(Informacion_Sistema, Data_Vehiculos, Data_Terminales, Numero_Vehiculo, Numero_Fila_CSV);

}


//Voy rellenando la informacion de los diferentes vehiculos conectados al sistema.
void Asignar_Informacion_Vehiculos(Elementos_Electrolinera* Informacion_Sistema,const Celda *** Data_Vehiculos,
	                               const Celda *** Data_Terminales, const int Filas_CSV_Vehiculos,
	                               Puntos_Optimizacion *Array_Puntos_Simulacion) {
	//Declaro una variable que sirve para ir indicando por que numero de vehiculo se estan actualizando los datos
	int Vehiculo_Actual = 0;
	
	//Cargo el numero de puntos que contiene la simulacion->
	int Numero_Puntos = Informacion_Sistema->Numero_Puntos;
	//Un bucle for que itera por las filas del CSV de los vehiculos->
	for (int Fila_CSV = 1; Fila_CSV < Filas_CSV_Vehiculos; Fila_CSV++) {
	//Si la fila actual del CSV contiene un vehiculo->
		if (Comprobar_Vehiculo(Data_Vehiculos[Fila_CSV][COL_TIPO_DISPOSITIVO]->data.str)) {
			printf("Entra bucle \n");
			//Se incluye el vehiculo correspondiente en la informacion del sistema->
			Incluir_Vehiculo(Fila_CSV, Informacion_Sistema, Vehiculo_Actual, Data_Vehiculos, Array_Puntos_Simulacion,
				Numero_Puntos, Data_Terminales);
			Vehiculo_Actual++;
	}
		
	//Si la fila actual del CSV contiene una bateria->
	}
}

// Este subprograma se utiliza para añadir a la variable Informacion_Sistema que contiene la informacion de los 
// elementos conectados a la electrolinera los datos de los diferentes vehiculos del sistema


int Identificar_Vehiculos(Elementos_Electrolinera * Informacion_Sistema,const Datos_CSV *** Data_Vehiculos_Baterias,const int *Filas_CSV_Vehiculos,
	                      const Datos_CSV *** Data_Terminales, Puntos_Optimizacion *Array_Puntos_Simulacion) {
	//Calculo el numero de vehiculos que se tienen en el CSV de entrada->
	int Numero_Vehiculos = Calcular_Numero_Vehiculos(Data_Vehiculos_Baterias,Filas_CSV_Vehiculos);
	//Guardo el numero de vehiculos que se tienen en el sistema->
	Informacion_Sistema->Numero_Vehiculos = Numero_Vehiculos;
	//Reservo memoria para el array que va a contener los datos de los vehiculos
	if (Reservar_Memoria_Vehiculos(Informacion_Sistema) == -1) {
		return -1;
	}
	Asignar_Informacion_Vehiculos(Informacion_Sistema, Data_Vehiculos_Baterias, Data_Terminales,*Filas_CSV_Vehiculos,
		                          Array_Puntos_Simulacion);
	return 0;
}

