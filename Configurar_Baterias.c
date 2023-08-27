#include "Configurar_Vehiculos.h"
#include "time.h"
#include "Tipos_Optimizacion.h"
#include <stdbool.h>
#include <stdlib.h>

//Defino las columnas donde se encuentran la informacion en el CSV.
#define COL_CAPACIDAD_BATERIA      1
#define COL_BATERIA_INICIAL        2
#define COL_BATERIA_FINAL          3
#define COL_MAXIMA_POTENCIA        4
#define COL_ANYO_INI_CARGA         5
#define  COL_MES_INI_CARGA         6
#define  COL_DIA_INI_CARGA         7
#define  COL_HORA_INI_CARGA        8
#define  COL_MINUTO_INI_CARGA      9
#define  COL_ANYO_FIN_CARGA        10
#define  COL_MES_FIN_CARGA         11
#define  COL_DIA_FIN_CARGA         12
#define  COL_HORA_FIN_CARGA        13
#define  COL_MINUTO_FIN_CARGA      14
#define  COL_OBJETIVO_CARGA        15
#define  COL_ANYO_OBJETIVO_CARGA   16
#define  COL_MES_OBJETIVO_CARGA    17
#define  COL_DIA_OBJETIVO_CARGA    18
#define  COL_HORA_OBJETIVO_CARGA   19
#define  COL_MINUTO_OBJETIVO_CARGA 20

bool Comprobar_Bateria(const char* Dispositivo) {

	//Este subprograma sirve para distinguir si en cada terminal se tiene un vehiculo o una bateria conectada
	//si en la celda correspondiente del CSV está escrito "Vehiculo" se tiene un vehiculo y se devuelve true.

	if ((strcmp(Dispositivo, "Bateria") == 0)||((strcmp(Dispositivo,"bateria"))==0)) {
		return true;

	}
	else {
		return false;
	}

}



int Reservar_Memoria_Baterias (Elementos_Electrolinera* Informacion_Sistema) {
	int Numero_Baterias = Informacion_Sistema->Numero_Baterias;
	Informacion_Sistema->Baterias_Sistema = (Baterias*)malloc(Numero_Baterias * sizeof(Baterias));
	if (!Informacion_Sistema->Baterias_Sistema) {
		printf("La reserva de memoria ha fallado \n");
		return -1;
	}
	return 0;
}



void Incluir_Datos_CSV_Bateria(Elementos_Electrolinera* Informacion_Sistema, const Celda*** Data_Bateria,
	const int Numero_Fila, const int  Numero_Bateria) {
	//Este subprograma se utiliza para incluir los datos del vehiculo que se pueden leer facilemente del CSV.
	Informacion_Sistema->Baterias_Sistema[Numero_Bateria].Capacidad_Bateria = Data_Bateria[Numero_Fila][COL_CAPACIDAD_BATERIA]->data.dbl;
	Informacion_Sistema->Baterias_Sistema[Numero_Bateria].Bateria_Inicial = Data_Bateria[Numero_Fila][COL_BATERIA_INICIAL]->data.dbl;
	Informacion_Sistema->Baterias_Sistema[Numero_Bateria].Bateria_Final = Data_Bateria[Numero_Fila][COL_BATERIA_FINAL]->data.dbl;
	Informacion_Sistema->Baterias_Sistema[Numero_Bateria].Maxima_Potencia = Data_Bateria[Numero_Fila][COL_MAXIMA_POTENCIA]->data.dbl;
}

void Incluir_Punto_Inicial_Final_Bateria(Elementos_Electrolinera* Informacion_Sistema, Puntos_Optimizacion* Array_Puntos_Simulacion,
	const int Numero_Bateria, const Celda*** Data_Baterias, const int Numero_Fila_CSV,
	const int Numero_Puntos) {
	//Este subprograma se utiliza para calcular el punto inicial y final de los vehiculos de la simulacion
	//Se definen variables para almacenar el inicio y finalizacion de la carga->
	struct tm Fecha_Inicio_Bateria;
	struct tm Fecha_Fin_Bateria;
	//Cargo las fechas de inicio y finalizacion de la carga de los vehiculos->
	Fecha_Inicio_Bateria.tm_year = Data_Baterias[Numero_Fila_CSV][COL_ANYO_INI_CARGA]->data.dbl - 1900;
	Fecha_Inicio_Bateria.tm_mon = Data_Baterias[Numero_Fila_CSV][COL_MES_INI_CARGA]->data.dbl - 1;
	Fecha_Inicio_Bateria.tm_mday = Data_Baterias[Numero_Fila_CSV][COL_DIA_INI_CARGA]->data.dbl;
	Fecha_Inicio_Bateria.tm_hour = Data_Baterias[Numero_Fila_CSV][COL_HORA_INI_CARGA]->data.dbl;
	Fecha_Inicio_Bateria.tm_min = Data_Baterias[Numero_Fila_CSV][COL_MINUTO_INI_CARGA]->data.dbl;
	Fecha_Inicio_Bateria.tm_sec = 0;
	//Cargo la fecha de finalizacion de la carga->
	Fecha_Fin_Bateria.tm_year = Data_Baterias[Numero_Fila_CSV][COL_ANYO_FIN_CARGA]->data.dbl - 1900;
	Fecha_Fin_Bateria.tm_mon = Data_Baterias[Numero_Fila_CSV][COL_MES_FIN_CARGA]->data.dbl - 1;
	Fecha_Fin_Bateria.tm_mday = Data_Baterias[Numero_Fila_CSV][COL_DIA_FIN_CARGA]->data.dbl;
	Fecha_Fin_Bateria.tm_hour = Data_Baterias[Numero_Fila_CSV][COL_HORA_FIN_CARGA]->data.dbl;
	Fecha_Fin_Bateria.tm_min = Data_Baterias[Numero_Fila_CSV][COL_MINUTO_FIN_CARGA]->data.dbl;
	Fecha_Fin_Bateria.tm_sec = 0;



	int Index_Fecha_Inicio = Busqueda_Binaria(Array_Puntos_Simulacion, Numero_Puntos, Fecha_Inicio_Bateria);
	if (Index_Fecha_Inicio != -1) {
		Informacion_Sistema->Baterias_Sistema[Numero_Bateria].Punto_Inicial_Bateria = Index_Fecha_Inicio;
	}
	int Index_Fecha_Fin = Busqueda_Binaria(Array_Puntos_Simulacion, Numero_Puntos, Fecha_Fin_Bateria);
	if (Index_Fecha_Fin != -1) {
		Informacion_Sistema->Baterias_Sistema[Numero_Bateria].Punto_Final_Bateria = Index_Fecha_Fin;
	}
}

void  Obtener_Fase_Bateria(Elementos_Electrolinera* Informacion_Sistema, const Celda*** Data_Baterias,
	const Celda*** Data_Terminales, const int Numero_Bateria, const int N_Fila_CSV) {
	//Este subprorgama se utiliza para identificar a que fase de la red esta conectado el vehiculo en cuestion->
	// R S o T
	//Primero es necesario identificar a que numero de terminal esta conectado el vehiculo->
	int Numero_Terminal = Obtener_Numero_Terminal(Data_Baterias, N_Fila_CSV);
	
	Informacion_Sistema->Baterias_Sistema[Numero_Bateria].Numero_Terminal =Numero_Terminal;
	

	//Segundo es necesario identificar la a fase a la que esta conectada dicho terminal->
	char fase = Obtener_Fase_Electrica(Data_Terminales, Numero_Terminal);
	
	//Incluyo la fase en la que esta conectada en el vehiculo correspondiente
	Informacion_Sistema->Baterias_Sistema[Numero_Bateria].Fase = fase;
}
bool Comprobar_Objetivo_Carga(const char* Objetivo) {
	//Funcion booleana que devuelve true si hay que considerar el objetivo de carga de la bateria
	//o falso si no hay que considerarlo.
	
	if ((strcmp(Objetivo, "Si") == 0) || (strcmp(Objetivo, "si") == 0) || (strcmp(Objetivo, "sí") == 0) || strcmp(Objetivo, "Sí")==0) {
		
		return true;
	}
	else {
		return false;
	}
}
void Considerar_Carga_Bateria(Elementos_Electrolinera *Informacion_Sistema, const Celda ***Data_Baterias,
	                          const int Numero_Fila, const int Numero_Bateria) {
	
	//Este subprograma se utiliza para almacenar en la variable que contiene la informacion sobre la electrolinera
	//si se desean que las baterias se carguen para la fecha objetivo o no.
	
		if ((Comprobar_Objetivo_Carga(Data_Baterias[Numero_Fila][COL_OBJETIVO_CARGA]->data.str)==true)) {
			
			Informacion_Sistema->Baterias_Sistema[Numero_Bateria].Considerar_Carga = true;
		}
		else {
			
			Informacion_Sistema->Baterias_Sistema[Numero_Bateria].Considerar_Carga = false;
			
		}

}
void Obtener_Punto_Objetivo(Elementos_Electrolinera *Informacion_Sistema, Puntos_Optimizacion *Array_Puntos_Simulacion,
	                        const int Numero_Bateria, const Celda ***Data_Baterias, const int Numero_Fila_CSV,const int Numero_Puntos) {
	//Este subprograma se utiliza para calcular el punto de la simulacion para el cual se desea satisfacer, un
	//objetivo de carga de la bateria.

	//Se comprueba si para la bateria en cuestion se desea que este cargada a un determinado porcentaje para una fecha
	//en concreto
	if (Informacion_Sistema->Baterias_Sistema[Numero_Bateria].Considerar_Carga == true) {
		struct tm Fecha_Objetivo_Bateria;
		//Cargo la fecha Objetivo de la bateria
		Fecha_Objetivo_Bateria.tm_year = Data_Baterias[Numero_Fila_CSV][COL_ANYO_OBJETIVO_CARGA]->data.dbl - 1900;
		Fecha_Objetivo_Bateria.tm_mon =  Data_Baterias[Numero_Fila_CSV][COL_MES_OBJETIVO_CARGA]->data.dbl - 1;
		Fecha_Objetivo_Bateria.tm_mday = Data_Baterias[Numero_Fila_CSV][COL_DIA_OBJETIVO_CARGA]->data.dbl;
		Fecha_Objetivo_Bateria.tm_hour = Data_Baterias[Numero_Fila_CSV][COL_HORA_OBJETIVO_CARGA]->data.dbl;
		Fecha_Objetivo_Bateria.tm_min =  Data_Baterias[Numero_Fila_CSV][COL_MINUTO_OBJETIVO_CARGA]->data.dbl;
		Fecha_Objetivo_Bateria.tm_sec =  0;

		int Index_Fecha_Objetivo = Busqueda_Binaria(Array_Puntos_Simulacion, Numero_Puntos, Fecha_Objetivo_Bateria);

		if (Index_Fecha_Objetivo != -1) {
			Informacion_Sistema->Baterias_Sistema[Numero_Bateria].Punto_Objetivo_Bateria = Index_Fecha_Objetivo;
		}
	}
	else {
		Informacion_Sistema->Baterias_Sistema[Numero_Bateria].Punto_Objetivo_Bateria = 0;
    //Si no se considera ningun objetivo de carga para la bateria en cuestion no se hace nada.
	}
	
}
void Incluir_Bateria(const int Numero_Fila_CSV, Elementos_Electrolinera* Informacion_Sistema, const int Numero_Bateria,
	const Celda*** Data_Baterias, Puntos_Optimizacion* Array_Puntos_Simulacion, const int Numero_Puntos, const Celda*** Data_Terminales) {
	//Se va añadiendo la informacion del vehiculo correspondiente->
	Incluir_Datos_CSV_Bateria(Informacion_Sistema, Data_Baterias, Numero_Fila_CSV, Numero_Bateria);
	Incluir_Punto_Inicial_Final_Bateria(Informacion_Sistema, Array_Puntos_Simulacion, Numero_Bateria, Data_Baterias,
		Numero_Fila_CSV, Numero_Puntos);
	Obtener_Fase_Bateria(Informacion_Sistema, Data_Baterias, Data_Terminales, Numero_Bateria, Numero_Fila_CSV);
	Considerar_Carga_Bateria(Informacion_Sistema, Data_Baterias, Numero_Fila_CSV,Numero_Bateria);
	
	Obtener_Punto_Objetivo(Informacion_Sistema, Array_Puntos_Simulacion, Numero_Bateria, Data_Baterias, Numero_Fila_CSV, Numero_Puntos);
	
}


void Asignar_Informacion_Baterias(Elementos_Electrolinera* Informacion_Sistema, const Celda*** Data_Baterias,
	const Celda*** Data_Terminales, const int Filas_CSV_Baterias,
	Puntos_Optimizacion* Array_Puntos_Simulacion) {
	//Declaro una variable que sirve para ir indicando por que numero de vehiculo se estan actualizando los datos
	int Bateria_Actual = 0;
	//Cargo el numero de puntos que contiene la simulacion->
	int Numero_Puntos = Informacion_Sistema->Numero_Puntos;
	//Un bucle for que itera por las filas del CSV de los vehiculos->
	for (int Fila_CSV = 1; Fila_CSV < Filas_CSV_Baterias; Fila_CSV++) {
		    //Si la fila actual del CSV contiene un vehiculo->
			
			//Se incluye el vehiculo correspondiente en la informacion del sistema->
			Incluir_Bateria(Fila_CSV, Informacion_Sistema, Bateria_Actual, Data_Baterias, Array_Puntos_Simulacion,
				Numero_Puntos, Data_Terminales);
			Bateria_Actual++;
		

		//Si la fila actual del CSV contiene una bateria->
	}
	
}

int Identificar_Baterias(Elementos_Electrolinera* Informacion_Sistema, const Celda*** Data_Baterias,
	const int Filas_CSV_Baterias, const Celda*** Data_Terminales, Puntos_Optimizacion* Array_Puntos_Simulacion) {
	//Calculo el numero de baterias que se tienen en el CSV de entrada->
	int Numero_Baterias = Filas_CSV_Baterias - 1;
	//Guardo el numero de Baterias que se tienen en el sistema->
	Informacion_Sistema->Numero_Baterias = Numero_Baterias;
	//Reservo memoria para el array que va a contener los datos de los vehiculos
	if (Reservar_Memoria_Baterias(Informacion_Sistema) == -1) {
		return -1;
	}
	
	Asignar_Informacion_Baterias(Informacion_Sistema, Data_Baterias, Data_Terminales, Filas_CSV_Baterias,
		Array_Puntos_Simulacion);
	return 0;
}
