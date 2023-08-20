#include "osqp.h"
#include <stdbool.h>
#include "Comprobar_Informacion.h"
#include "Tipos_Optimizacion.h"
#include "time.h"
//Se definen en que columnas estan las diferentes informaciones en el CSV que contiene la informacion de los
//vehiculos->
#define COL_TIPO_DISPOSITIVO  1
#define COL_ID_VEHICULO       2
#define COL_ANYO_INI_CARGA    8
#define COL_MES_INI_CARGA     9
#define COL_DIA_INI_CARGA     10
#define COL_HORA_INI_CARGA    11
#define COL_MINUTO_INI_CARGA  12
#define COL_ANYO_FIN_CARGA    13
#define COL_MES_FIN_CARGA     14
#define COL_DIA_FIN_CARGA     15
#define COL_HORA_FIN_CARGA    16
#define COL_MINUTO_FIN_CARGA  17
//Se definen las filas y columnas en las que se encuentran los parametros temporales correspondientes en el CSV 
//correspondiente->
#define FILA_PARAMETROS_TIEMPO 1
#define COLUMNA_ANYO_INICIAL   0 
#define COLUMNA_MES_INICIAL    1
#define COLUMNA_DIA_INICIAL    2
#define COLUMNA_HORA_INICIAL   3
#define COLUMNA_MINUTO_INICIAL 4

//Defino en que columnas del CSV se encuentran los datos temporales del final de la simulacion->
#define COLUMNA_ANYO_FINAL     5
#define COLUMNA_MES_FINAL      6
#define COLUMNA_DIA_FINAL      7
#define COLUMNA_HORA_FINAL     8
#define COLUMNA_MINUTO_FINAL   9
//Defino la columna donde se encuentra almacenado el delta de simulacion entre los diferentes puntos de la simulacion
#define COLUMNA_DELTA_SIMULACION 10
//Funcion para comaparar dos fechas en formato tm devuelve true y false
//si no son iguales->

int Reajustar_Dimension_Array_Puntos(Puntos_Optimizacion** arrayPuntosSimulacion, int nuevaDimensionArray) {
	// Subprograma que se utiliza para reajustar la memoria reservada al array que almacena el número de puntos de simulación
	// Entradas:
	// arrayPuntosSimulacion -> Array que almacena los puntos de simulación y los asocia con una fecha.
	// nuevaDimensionArray  -> El nuevo tamaño que debe tener el array.
	printf("La nueva dimension del array es: \n");
	printf("%d\n", nuevaDimensionArray);

	Puntos_Optimizacion* temp = realloc(*arrayPuntosSimulacion, nuevaDimensionArray * sizeof(Puntos_Optimizacion));
	if (temp == NULL) {
		printf("Reserva de memoria fallida Array puntos simulacion\n");
		return -1; // 
	}
	*arrayPuntosSimulacion = temp;
	return 0;
}

bool Fechas_Iguales(const struct tm Fecha_1, const struct tm Fecha_2) {

	if (Fecha_1.tm_year == Fecha_2.tm_year &&
		Fecha_1.tm_mon == Fecha_2.tm_mon &&
		Fecha_1.tm_mday == Fecha_2.tm_mday &&
		Fecha_1.tm_hour == Fecha_2.tm_hour &&
		Fecha_1.tm_min == Fecha_2.tm_min &&
		Fecha_1.tm_sec == Fecha_2.tm_sec) {
		return true;
	}
	else {
		return false;
	}
}

void Comprobar_Array_Puntos_Adicionales(Puntos_Adicionales* Array_Puntos_Adicionales) {
	FILE* file = fopen("Comprobar_Adicional.csv", "w");
	if (file == NULL) {
		printf("Error Abriendo el archivo \n");
		printf("Here");
		return;
	}
	for (int i = 0; i < 29; i++) {
		fprintf(file, "%d,", Array_Puntos_Adicionales[i].date.tm_year);
		fprintf(file, "%d,", Array_Puntos_Adicionales[i].date.tm_mon);
		fprintf(file, "%d,", Array_Puntos_Adicionales[i].date.tm_mday);
		fprintf(file, "%d,", Array_Puntos_Adicionales[i].date.tm_hour);
		fprintf(file, "%d,", Array_Puntos_Adicionales[i].date.tm_min);
		fprintf(file, "%d,", Array_Puntos_Adicionales[i].date.tm_sec);
		fprintf(file, "\n");
	}
	fclose(file);
}

//Subprograma para cargar la fecha inicial y la fecha final en dos variables de formato tm->
void Cargar_Fecha_Inicial_Final(struct tm* Fecha_Inicial, struct tm* Fecha_Final, const Celda*** Data_Tiempo) {
	//Este subprograma se utiliza para cargar en variables de tipo tm la fecha inicial y final de la simulacion->
	Fecha_Inicial->tm_year = Data_Tiempo[FILA_PARAMETROS_TIEMPO][COLUMNA_ANYO_INICIAL]->data.dbl-1900;
	Fecha_Inicial->tm_mon = Data_Tiempo[FILA_PARAMETROS_TIEMPO][COLUMNA_MES_INICIAL]->data.dbl-1;
	Fecha_Inicial->tm_mday = Data_Tiempo[FILA_PARAMETROS_TIEMPO][COLUMNA_DIA_INICIAL]->data.dbl;
	Fecha_Inicial->tm_hour = Data_Tiempo[FILA_PARAMETROS_TIEMPO][COLUMNA_HORA_INICIAL]->data.dbl;
	Fecha_Inicial->tm_min = Data_Tiempo[FILA_PARAMETROS_TIEMPO][COLUMNA_MINUTO_INICIAL]->data.dbl;
	Fecha_Inicial->tm_sec = 0;
	//Se carga la Fecha final->
	Fecha_Final->tm_year = Data_Tiempo[FILA_PARAMETROS_TIEMPO][COLUMNA_ANYO_FINAL]->data.dbl-1900;
	Fecha_Final->tm_mon = Data_Tiempo[FILA_PARAMETROS_TIEMPO][COLUMNA_MES_FINAL]->data.dbl-1;
	Fecha_Final->tm_mday = Data_Tiempo[FILA_PARAMETROS_TIEMPO][COLUMNA_DIA_FINAL]->data.dbl;
	Fecha_Final->tm_hour = Data_Tiempo[FILA_PARAMETROS_TIEMPO][COLUMNA_HORA_FINAL]->data.dbl;
	Fecha_Final->tm_min = Data_Tiempo[FILA_PARAMETROS_TIEMPO][COLUMNA_MINUTO_FINAL]->data.dbl;
	Fecha_Final->tm_sec = 0;

}
//Este subprograma sirve para eliminar del array que contiene los puntos adicionales que deben añadirse que
//estén repetidos
void Reajustar_Array(Puntos_Adicionales **Array_Puntos_Adicionales, int *Numero_Puntos) {
	int i = 0;
	while (i < *Numero_Puntos-1) {
		//Se carga la fecha de un punto del array y la del siguiente para comprobar si son iguales
		struct tm Fecha1 = (*Array_Puntos_Adicionales)[i].date;
		struct tm Fecha2 = (*Array_Puntos_Adicionales)[i + 1].date;
		
		if (Fechas_Iguales(Fecha1, Fecha2)) {
			memmove(&(*Array_Puntos_Adicionales)[i], &(*Array_Puntos_Adicionales)[i + 1], (*Numero_Puntos - i - 1) * sizeof(Puntos_Adicionales));
			(*Numero_Puntos)--; // Update the total number of points
		}
		else {
			i++;
		}
	}
	*Array_Puntos_Adicionales = (Puntos_Adicionales*)realloc(*Array_Puntos_Adicionales, *Numero_Puntos * sizeof(Puntos_Adicionales));
}


//Subprograma para averiguar si no hay que añadir más puntos de carga.
bool Fin_Puntos_Carga(const int Puntos_Carga_Totales,const int Numero_Punto_Carga) {
//Este subprograma tiene dos entradas:
//Puntos_Carga_Totales: Numero de puntos de simulacion que hay que considerar en la simulacion
//Numero_Punto_Carga:El numero de punto de carga del array de carga del vehiculo que se esta considerando.
	if (Numero_Punto_Carga < Puntos_Carga_Totales) {
		return false;
	}
	else {
		return true;
	}
}
//Subprograma para comprobar si una fecha es superior a otra
bool Fecha_Superior(const struct tm Fecha_1, const struct tm Fecha_2) {
	if (Fecha_1.tm_year > Fecha_2.tm_year) return true;
	if (Fecha_1.tm_year < Fecha_2.tm_year) return false;

	if (Fecha_1.tm_mon > Fecha_2.tm_mon) return true;
	if (Fecha_1.tm_mon < Fecha_2.tm_mon) return false;

	if (Fecha_1.tm_mday > Fecha_2.tm_mday) return true;
	if (Fecha_1.tm_mday < Fecha_2.tm_mday) return false;

	if (Fecha_1.tm_hour > Fecha_2.tm_hour) return true;
	if (Fecha_1.tm_hour < Fecha_2.tm_hour) return false;

	if (Fecha_1.tm_min > Fecha_2.tm_min) return true;
	if (Fecha_1.tm_min < Fecha_2.tm_min) return false;

	if (Fecha_1.tm_sec > Fecha_2.tm_sec) return true;
	if (Fecha_1.tm_sec < Fecha_2.tm_sec) return false;

	return false; // Las fechas son iguales}
}



bool Comprobar_Hora_Punto(const struct tm Punto_Comprobar, const int Delta_Minutos) {
	//Este subprograma se utiliza para comprobar si es necesario añadir un punto de simulacion para ver si hay añadir
	//un punto que coincide con una hora en punto.
	
	//Se carga el minuto del punto que se quiere comprobar
	int Minuto = Punto_Comprobar.tm_min;
	//Se resta el delta minutos al minuto del punto actual, si el minuto es inferior a 0 es necesario añadir un punto
	//que coincida con la hora en punto
	int Minuto_Diferencia = Minuto - Delta_Minutos;
	if (Minuto_Diferencia < 0) {
		return true;
	}
	else {
		return false;
	}
}
//En este archivo están los subprogramas que se encargan de configurar los ajustes de la simulacion, así como 
// de calcular el número de puntos de la simulación y la diferencia temporal entre ellos----->

bool Comprobar_Vehiculo(const char* Dispositivo) {
	
	//Este subprograma sirve para distinguir si en cada terminal se tiene un vehiculo o una bateria conectada
	//si en la celda correspondiente del CSV está escrito "Vehiculo" se tiene un vehiculo y se devuelve true.

	if (strcmp(Dispositivo,"Vehiculo") == 0){
		return true;
		
	}
	else {
		return false;
	}
	
}
int  Calcular_Numero_Vehiculos(const Celda*** Data_Vehiculos, const int *N_Filas_CSV) {
	//Este subprograma se utiliza para obtener el numero de vehiculos que tienen su carga programada->
	//Se va leyendo por todas las filas del excel que contiene la informacion de los vehiculos.

	int N_Vehiculos = 0;

	//Se itera por cada fila del CSV que contiene la informacion de los vehiculos que tienen su 
	//carga programada.
	for (int Numero_Fila = 1; Numero_Fila <*N_Filas_CSV; Numero_Fila++) {
		
		if (Comprobar_Vehiculo(Data_Vehiculos[Numero_Fila][COL_TIPO_DISPOSITIVO]->data.str)) {
			N_Vehiculos++;
		}
	}
	
	return N_Vehiculos;
}
int  Comparar_Puntos(const void* a, const void* b) {
	//Esta es una funcion que se crea con el proposito de comparar dos puntos de la simulacion y decidir cual
	//esta anteriormente cronologicamente en el tiempo->
	Puntos_Adicionales* Punto_A = (Puntos_Adicionales*) a;
	Puntos_Adicionales* Punto_B = (Puntos_Adicionales*)b;

	//Se compara los campos temporales y se obitene la diferencia en el tiempo->
	return difftime(mktime(&Punto_A->date), mktime(&Punto_B->date));
}
//Subprograma que se utiliza para calcular el numero de horas 
int Calcular_Numero_Horas(struct tm Fecha_Inicial, struct tm Fecha_Final) {
	//En este subprograma se calcula el numero de puntos que van a coincidir con una hora en punto, y que han de
	// ser añadidos al array->
	printf("Hora Inicial antes de pasar por el bucle");
	printf("%d \n", Fecha_Inicial.tm_hour);
	int Horas_En_Punto = 0;
	if (Fecha_Inicial.tm_min != 0 || Fecha_Inicial.tm_sec != 0) {
		Fecha_Inicial.tm_min = 0;
		Fecha_Inicial.tm_sec = 0;
		Fecha_Inicial.tm_hour++;
	}
	printf("Hora Inicial despues de pasar por el bucle \n");
	printf("%d \n", Fecha_Inicial.tm_hour);
	//Se normalizan las fechas.
	time_t initial_time = mktime(&Fecha_Inicial);
	time_t final_time = mktime(&Fecha_Final);

	
	//Se pasa a segundos
	double difference_in_seconds = difftime(final_time, initial_time);

	//Se calcula el numero de horas
	if (difference_in_seconds >= 0) {
		Horas_En_Punto = difference_in_seconds / 3600; // 3600 seconds in an hour
	}
	
	printf("Hora Inicial despues de pasar por el subprograma \n");
	printf("%d \n", Fecha_Inicial.tm_hour);
	return Horas_En_Punto;
}
void Añadir_Puntos_Horas_En_Punto(Puntos_Adicionales **Array_Puntos,struct tm Fecha_Inicial,struct tm Fecha_Final,
	                               int Punto_Array) {
	//En este subprograma se añaden al array de puntos adicionales, los puntos que coinciden con una hora en punto
	
	//Si la fecha inicial no coincide con una hora en punto se avanza a la siguiente hora en punto
	
	//Se calcula la primera hora en punto a añadir en el array
	Fecha_Inicial.tm_min = 0;
	Fecha_Inicial.tm_sec = 0;
	Fecha_Inicial.tm_hour++;
	printf("La hora inicial es \n: ");
	printf("%d", Fecha_Inicial.tm_hour);
	mktime(&Fecha_Inicial); //Se normaliza la fecha 
	
	//Se itera por las horas,añadiendo los nuevos puntos---->
	while (difftime(mktime(&Fecha_Final), mktime(&Fecha_Inicial)) > 0) {
		(*Array_Puntos)[Punto_Array].date = Fecha_Inicial;
		printf("Comprobacion Fecha Hora en punto \n");
		printf("%d", (*Array_Puntos)[Punto_Array].date.tm_year);
		printf("%d", (*Array_Puntos)[Punto_Array].date.tm_mon);
		printf("%d", (*Array_Puntos)[Punto_Array].date.tm_mday);
		printf("%d", (*Array_Puntos)[Punto_Array].date.tm_hour);
		printf("%d", (*Array_Puntos)[Punto_Array].date.tm_min);
		printf("%d", (*Array_Puntos)[Punto_Array].date.tm_sec);
		//Se mueve a la siguiente hora y se incrementa el contador
		Fecha_Inicial.tm_hour++;
		mktime(&Fecha_Inicial);
		Punto_Array++;
	}

}
void Incluir_Inicio_Carga(Puntos_Adicionales* Array_Puntos_Adicionales,const Celda *** Data_Vehiculos, const int Numero_Fila,
	                      const int Numero_Punto_Array) {
	//En este subprograma se carga en el array de puntos adicionales la fecha de inicio de carga de un vehiculo.
	Array_Puntos_Adicionales[Numero_Punto_Array].date.tm_year = Data_Vehiculos[Numero_Fila][COL_ANYO_INI_CARGA]->data.dbl - 1900;
	Array_Puntos_Adicionales[Numero_Punto_Array].date.tm_mon = Data_Vehiculos[Numero_Fila][COL_MES_INI_CARGA]->data.dbl - 1;
	Array_Puntos_Adicionales[Numero_Punto_Array].date.tm_mday = Data_Vehiculos[Numero_Fila][COL_DIA_INI_CARGA]->data.dbl;
	Array_Puntos_Adicionales[Numero_Punto_Array].date.tm_hour = Data_Vehiculos[Numero_Fila][COL_HORA_INI_CARGA]->data.dbl;
	Array_Puntos_Adicionales[Numero_Punto_Array].date.tm_min = Data_Vehiculos[Numero_Fila][COL_MINUTO_INI_CARGA]->data.dbl;
	Array_Puntos_Adicionales[Numero_Punto_Array].date.tm_sec = 0;
}

void Incluir_Final_Carga(Puntos_Adicionales* Array_Puntos_Adicionales, const Celda*** Data_Vehiculos, const int Numero_Fila,
	const int Numero_Punto_Array) {
	//En este subprograma se carga en el array de puntos adicionales la fecha de finalizacion de carga de un vehiculo
	Array_Puntos_Adicionales[Numero_Punto_Array].date.tm_year = Data_Vehiculos[Numero_Fila][COL_ANYO_FIN_CARGA]->data.dbl - 1900;
	Array_Puntos_Adicionales[Numero_Punto_Array].date.tm_mon = Data_Vehiculos[Numero_Fila][COL_MES_FIN_CARGA]->data.dbl - 1;
	Array_Puntos_Adicionales[Numero_Punto_Array].date.tm_mday = Data_Vehiculos[Numero_Fila][COL_DIA_FIN_CARGA]->data.dbl;
	Array_Puntos_Adicionales[Numero_Punto_Array].date.tm_hour = Data_Vehiculos[Numero_Fila][COL_HORA_FIN_CARGA]->data.dbl;
	Array_Puntos_Adicionales[Numero_Punto_Array].date.tm_min = Data_Vehiculos[Numero_Fila][COL_MINUTO_FIN_CARGA]->data.dbl;
	Array_Puntos_Adicionales[Numero_Punto_Array].date.tm_sec = 0;

}
void Reordenar_Array(Puntos_Adicionales **Array_Puntos_Adicionales, int *Numero_Puntos) {
	//Subprograma que se utiliza para reordenar el array de puntos adicionales por orden cronologico, y 
	//eliminar aquellos puntos que estan repetidos
	qsort(*Array_Puntos_Adicionales, *Numero_Puntos, sizeof(Puntos_Adicionales), Comparar_Puntos);
	//Comprobar_Array_Puntos_Adicionales(*Array_Puntos_Adicionales);
	Reajustar_Array(Array_Puntos_Adicionales, Numero_Puntos);
	//Comprobar_Array_Puntos_Adicionales(*Array_Puntos_Adicionales);
	qsort(*Array_Puntos_Adicionales, *Numero_Puntos, sizeof(Puntos_Adicionales), Comparar_Puntos);
	printf("El numero de puntos es-> \n");
	printf("%d \n",*Numero_Puntos);
	Comprobar_Array_Puntos_Adicionales(*Array_Puntos_Adicionales);
}
int Calcular_Punto_Final(const struct tm Fecha_Final) {
	//Se utiliza para ver si hay que tener en consideracion que es necesario añadir un punto final si la hora final
	//no es una hora en punto->
	int Punto_Final = 0;
	if (Fecha_Final.tm_min != 0) {
		return Punto_Final = 1;
	}
	else {
		return Punto_Final;
	}
	
}
int Obtener_Array_Puntos_Adicionales(const Celda*** Data_Vehiculos,const Celda ***Data_Tiempo, Puntos_Adicionales**Array_Puntos,
	const int *N_Filas_CSV, int*Numero_Puntos) {
	//En este subprograma se obtiene un array que contiene los puntos de simulacion que deben estar en la simulacion
	//por que coinciden con la fecha de inicio o la fecha de finalizacion de una carga de un vehiculo.

    //Parametros de entrada: El array que contiene los datos de los dispositivos conectados al terminal, y el numero
	//de filas del array.
	
	//Bucle for que itera por las filas del array que contiene la informacion de los vehiculos->
    //Se tiene en cuenta que la primera fila corresponde a los rotulos de informacion indicando que es cada campo 
	//del CSV.
	
	//Variables auxiliares:
	//Fecha_Inicial:Fecha Inicial de ejecucion del algoritmo
	//Fecha_Final:  Fecha Final de ejecucion del algoritmo
	struct tm Fecha_Inicial;
	struct tm Fecha_Final;

	//Cargo la fecha inicial y final en dos variables para calcular el numero de horas->
	Cargar_Fecha_Inicial_Final(&Fecha_Inicial, &Fecha_Final, Data_Tiempo);
	printf("La fecha inicial completa es->\n");
	printf("%d", Fecha_Inicial.tm_mon);
	printf("%d", Fecha_Inicial.tm_mday);
	printf("%d", Fecha_Inicial.tm_hour);
	printf("%d", Fecha_Inicial.tm_min);
	
	//Calculo el numero de vehiculos que se tienen en el CSV de entrada->
	int Numero_Vehiculos=Calcular_Numero_Vehiculos(Data_Vehiculos, N_Filas_CSV);
	
	//Defino la columna en la que se encuentra registrado la informacion de que tipo de dispositivo es
	int Columna_Dispositivo = COL_TIPO_DISPOSITIVO;
	
    //Se calcula el numero de horas durante las cuales se ejecuta el algoritmo.
	int Numero_Horas = Calcular_Numero_Horas(Fecha_Inicial,Fecha_Final);
	
	//Se considera la posibilidad de que la hora final no acabe en una hora  en punto y sea necesario añadir
	//un punto.
	int Punto_Final = Calcular_Punto_Final(Fecha_Final);
	printf("El punto final es->");
	printf("%d", Punto_Final);
	//La fecha final es->
	printf("La Fecha Final es->");
	printf("%d\n", Fecha_Final.tm_hour);
	printf("%d\n", Fecha_Final.tm_min);
	//El array ha de tener una dimension que es igual al doble de vehiculos que tienen su carga programada
	//(se incluye la fecha inicial y final de carga) y el numero de horas (se incluyen las horas en punto)

	int Dimension_Array = 2*Numero_Vehiculos + Numero_Horas + Punto_Final;
	
	//Por cada vehiculo en el array que contiene los puntos de inicio y finalizacion de la simulacion tiene que 
	//haber dos puntos.

	//Se reserva memoria para el array.
	*Array_Puntos = (Puntos_Adicionales*) calloc (Dimension_Array,sizeof(Puntos_Adicionales));
	if (*Array_Puntos == NULL) {
		return -1;
	}
	int Numero_Punto = 0;
	

	for (int Numero_Fila = 1; Numero_Fila < *N_Filas_CSV; Numero_Fila++) {
		
		//Se va iterando por cada fila del CSV y si es un vehiculo se incluye su inicio y finalizacion de carga
		//en el array

		if (Comprobar_Vehiculo(Data_Vehiculos[Numero_Fila][Columna_Dispositivo]->data.str)) {
			//Se carga en la posicion correspondiente del array la fecha inicial de carga del vehiculo
			Incluir_Inicio_Carga(*Array_Puntos, Data_Vehiculos, Numero_Fila, Numero_Punto);
			Numero_Punto++;
			//Se carga en la posicion correspondiende del array la fecha final de carga del vehiculo->
			Incluir_Final_Carga(*Array_Puntos, Data_Vehiculos, Numero_Fila, Numero_Punto);
			Numero_Punto++;
		}
		
	}

	//Una vez que se tiene el array con las fechas de inicio y finalizacion de carga de todos los vehiculos
	//Una vez hecho eso es necesario reordenar el array en orden cronologico 
	
	Añadir_Puntos_Horas_En_Punto(Array_Puntos, Fecha_Inicial, Fecha_Final, Numero_Punto);
	//Se carga la dimension del array.
	*Numero_Puntos = Dimension_Array;
	
	//Se reordena las fechas contenidas en el array en orden cronologico y se eliminan aquellas fechas que 
	//se encuentren repetidas->
	printf("El numero de puntos es-> \n");
	printf("%d \n", *Numero_Puntos);
	
	//Comprobar_Array_Puntos_Adicionales(*Array_Puntos);
	Reordenar_Array(Array_Puntos, Numero_Puntos);

	//Subprograma para testear que todo ha funcionado correctamente.
	//Comprobar_Array_Puntos_Adicionales(*Array_Puntos);
	return 0;
}

int Obtener_Diferencia_Minutos(const struct tm Punto_Uno, const struct tm Punto_Dos) {
	//Se pasa los structs tm a time_t
	time_t Tiempo_Uno = mktime((struct tm*) & Punto_Uno);
	time_t Tiempo_Dos = mktime((struct tm*)&Punto_Dos);
	//Calcula la diferencia en segundos-> (Duda)
	double Diferencia_Segundos = difftime(Tiempo_Uno, Tiempo_Dos);
	//Se convierte  la diferencia en minutos
	int Diferencia_Minutos = Diferencia_Segundos/ 60;
	//Se retorna la diferencia en minutos.
	return Diferencia_Minutos;
}
//Subprograma que se utiliza para añadir al array los puntos en el cual se almacenan los puntos de la simulacion->
void Añadir_Punto_Carga(Puntos_Optimizacion * Array_Puntos,const int Punto_Array,const struct tm Punto_Nuevo, const struct tm Punto_Anterior,const int Punto_Simulacion) {
 //Este subprograma sirve para añadir puntos de simulacion que coinciden con el inicio de la carga o finalizacion
 //de vehiculos.
 
//Se calcula el delta temporal de la simulacion->
	int Delta_Minutos = Obtener_Diferencia_Minutos(Punto_Nuevo,Punto_Anterior);
//Se añade los datos al array de puntos de la simulacion->
	Array_Puntos[Punto_Array].Delta_Simulacion = Delta_Minutos;
	Array_Puntos[Punto_Array].Fecha = Punto_Nuevo;
	Array_Puntos[Punto_Array].Punto_Simulacion = Punto_Simulacion;
}

struct tm Calcular_Siguiente_Punto(const int Delta_Simulacion, const struct tm Punto_Actual) {
	struct tm Punto_Siguiente;
	//Se copia el punto Actual al punto siguiente->
	Punto_Siguiente = Punto_Actual;
	//Se añade el delta en minutos al punto actual->
	printf("Comprobacion Hora \n");
	printf("%d", Punto_Siguiente.tm_hour);
	printf("Comprobacion minuto \n");
	printf("%d", Punto_Siguiente.tm_min);
	printf("El delta es \n");
	printf("%d\n", Delta_Simulacion);
	Punto_Siguiente.tm_min += Delta_Simulacion;
	printf("Comprobacion minuto + delta");
	printf("%d", Punto_Siguiente.tm_min);
	//Se Normaliza el formato de fecha->
	mktime(&Punto_Siguiente);
	printf("Comprobacion Hora \n");
	printf("%d", Punto_Siguiente.tm_hour);
	printf("Comprobacion minuto \n");
	printf("%d", Punto_Siguiente.tm_min);
	return Punto_Siguiente;
}
//Se comprueba si el siguiente punto de simulacion supera o iguala la fecha final de la simulacion
bool Comprobar_Fin_Simulacion(const struct tm Punto_Actual, const struct tm Punto_Final,const int *Punto_Actual_Array_Carga,
	                          const int * Total_Puntos_Array_Carga) {
	//Para comnparar facilmente las fechas se pasa a una estructura de tiempo time_t para poder comparar
	//facilmente las fechas
	time_t Tiempo_Actual = mktime(&Punto_Actual);
	time_t Tiempo_Final  = mktime(&Punto_Final);
	//Si la fecha del punto actual es superior a la fecha de finalizacion, 
	if (Tiempo_Actual < Tiempo_Final) {
		return false;
	}
	if (*Punto_Actual_Array_Carga < *Total_Puntos_Array_Carga ) {
		return false;
	}
	else {
		return true;
	}
}
bool Coincide_Punto_Carga(Puntos_Adicionales *Array_Punto,struct tm Punto_Siguiente,const int Punto_Array) {
	if (Fechas_Iguales(Array_Punto[Punto_Array].date, Punto_Siguiente)) {
		return true;
	}
    else {
		return false;
	}
}
bool Supera_Punto_Carga(Puntos_Adicionales *Array_Puntos,struct tm Punto_Siguiente,const int Punto_Array) {
	//Se comprueba si el siguiente punto supera el inicio o fin de carga de un vehiculo y es necesario
	//añadir un punto.
	if (Fecha_Superior(Punto_Siguiente, Array_Puntos[Punto_Array].date)) {
		return true;
	}
	else {
		return false;
	}
	
}

void Cargar_Fecha_Inicial(Puntos_Optimizacion* Array_Puntos_Simulacion,const Celda***Data_Tiempo) {
	//Se carga el primer punto de la simulacion en el array que relaciona, los puntos de la simulacion con las fechas
	//asociadas a los puntos de la simulacion.
	Array_Puntos_Simulacion[0].Fecha.tm_year = Data_Tiempo[FILA_PARAMETROS_TIEMPO][COLUMNA_ANYO_INICIAL]->data.dbl-1900;
	
	Array_Puntos_Simulacion[0].Fecha.tm_mon =  Data_Tiempo[FILA_PARAMETROS_TIEMPO][COLUMNA_MES_INICIAL]->data.dbl-1;
	
	Array_Puntos_Simulacion[0].Fecha.tm_mday = Data_Tiempo[FILA_PARAMETROS_TIEMPO][COLUMNA_DIA_INICIAL]->data.dbl;
	
	Array_Puntos_Simulacion[0].Fecha.tm_hour = Data_Tiempo[FILA_PARAMETROS_TIEMPO][COLUMNA_HORA_INICIAL]->data.dbl;
	
	Array_Puntos_Simulacion[0].Fecha.tm_min =  Data_Tiempo[FILA_PARAMETROS_TIEMPO][COLUMNA_MINUTO_INICIAL]->data.dbl;
	
	Array_Puntos_Simulacion[0].Fecha.tm_sec = 0;
	Array_Puntos_Simulacion[0].Punto_Simulacion = 0;
	Array_Puntos_Simulacion[0].Delta_Simulacion = 0;

}
int Inicializar_Array_Puntos_Simulacion(Puntos_Optimizacion **Array_Puntos_Simulacion,const Celda***Data_Tiempo) {
	//Este subprograma se utiliza para inicializar el array que contiene los puntos de simulacion, y la fecha 
	//asociada a ellas.

	//Se reserva memoria para el array
	*Array_Puntos_Simulacion = (Puntos_Optimizacion*)malloc(1*sizeof(Puntos_Optimizacion));
	if (*Array_Puntos_Simulacion == NULL) {
		printf("Reserva de memoria fallida \n");
		return -1;
	}
	//Se carga la fecha inicial en el array
	Cargar_Fecha_Inicial(*Array_Puntos_Simulacion, Data_Tiempo);

	//No ha habido problemas en la reserva de memoria
	return 0;
}

int Anyadir_Punto(Puntos_Optimizacion **Array_Puntos_Simulacion,const struct tm *Fecha,
	                         const int Delta_Simulacion,  int*Numero_Puntos) {
       //Subprograma para añadir un punto de simulacion al array que almacena los puntos de simulacion con sus fechas
       //asociadas

      //Variables de entrada:
      //Array_Puntos_Simulacion:Array que almacena los puntos de simulacion que tiene el algoritmo, esta variable
      //almacena asocia cada punto de ejecucion del algoritmo con una fecha, para relacionar de este modo cada punto
      //con una fecha real.
      //Delta_Simulacion: Variable en minutos de 1 a 60 que hay entre los puntos de la simulacion
      //Numero_Puntos: Numero de puntos de la simulacion que tiene el array hasta este momento
      //Fecha: Fecha a asociar al nuevo punto de la simulacion que hay en el array 

      //Se modifica el array añadiendo el nuevo punto con su fecha (Parametro de entrada Fecha), se introduce la 
      //variacion en minutos respecto al ultimo punto, y se actualiza la variable que almacena el numero de puntos
      //de la simulacion.
	
	(*Numero_Puntos)++;
	if (Reajustar_Dimension_Array_Puntos(Array_Puntos_Simulacion,*Numero_Puntos)) {
		printf("No se ha podido anyadir el punto adicional al array\n");
		return -1;
	}
	(*Array_Puntos_Simulacion)[*Numero_Puntos-1].Fecha = *Fecha;
	(*Array_Puntos_Simulacion)[*Numero_Puntos-1].Delta_Simulacion =Delta_Simulacion;
	(*Array_Puntos_Simulacion)[*Numero_Puntos-1].Punto_Simulacion = (*Numero_Puntos)-1;
	return 0;
}
int Incluir_Nuevo_Punto(Puntos_Optimizacion **Array_Puntos_Simulacion, int *Numero_Puntos_Simulacion,Puntos_Adicionales *Array_Puntos_Adicionales,
	                     const int Punto_Array_Carga,struct tm* Fecha_Siguiente) {
	//Este subprograma se utiliza para incluir al array en el que están registrados lo puntos de simulacion con
	//sus fechas asociadas.

	(*Numero_Puntos_Simulacion)++;
	if ((Reajustar_Dimension_Array_Puntos(Array_Puntos_Simulacion, *Numero_Puntos_Simulacion) == -1)) {
		return -1;
	}
	(*Array_Puntos_Simulacion)[*Numero_Puntos_Simulacion-1].Fecha = Array_Puntos_Adicionales[Punto_Array_Carga].date;
	//Cargo la Fecha que se ha calculado en la variable que devuelve ->
	*Fecha_Siguiente =  Array_Puntos_Adicionales[Punto_Array_Carga].date;
	//Se carga el punto de simulacion siguiente (Punto Segundo) y el punto anterior (Punto_Primero)
	struct tm Punto_Primero = (*Array_Puntos_Simulacion)[*Numero_Puntos_Simulacion-1].Fecha;
	struct tm Punto_Segundo = (*Array_Puntos_Simulacion)[*Numero_Puntos_Simulacion - 2].Fecha;

	//Se calcula la diferencia en minutos entre el punto de simulacion siguiente y el anterior.
	int Delta_Variable = Obtener_Diferencia_Minutos(Punto_Primero, Punto_Segundo);

	//Se añade al array el punto en cuestion.
	(*Array_Puntos_Simulacion)[*Numero_Puntos_Simulacion-1].Punto_Simulacion = (*Numero_Puntos_Simulacion)-1;
	(*Array_Puntos_Simulacion)[*Numero_Puntos_Simulacion-1].Delta_Simulacion = Delta_Variable;
	
	printf("El numero de puntos de simulacion es \n");
	printf("%d\n", *Numero_Puntos_Simulacion);
}
int Aumentar_Array_Simulacion     (Puntos_Optimizacion **Array_Puntos_Simulacion,Puntos_Adicionales **Array_Puntos_Adicionales,
	                              const int Delta_Simulacion, int*Numero_Puntos_Simulacion,struct tm *Fecha_Siguiente,
	                              int *Punto_Actual_Array_Carga, const int Puntos_Array_Adicional) {
	
	
	
	if ((Supera_Punto_Carga(*Array_Puntos_Adicionales, *Fecha_Siguiente,*Punto_Actual_Array_Carga)) && (!Fin_Puntos_Carga(Puntos_Array_Adicional,*Punto_Actual_Array_Carga))) {
		//La fecha del punto siguiente supera al punto del array luego es necesario añadir el punto del array
		//a la simulacion manualmente
		printf("Hello_World\n");
		if (Incluir_Nuevo_Punto(Array_Puntos_Simulacion, Numero_Puntos_Simulacion,*Array_Puntos_Adicionales,*Punto_Actual_Array_Carga,Fecha_Siguiente) == -1) {
			return -1;
		}
		(*Punto_Actual_Array_Carga)++;
	}

	

	else if ((Fechas_Iguales(*Fecha_Siguiente, (*Array_Puntos_Adicionales)[*Punto_Actual_Array_Carga].date))) {
		//Si las fecha del siguiente punto a añadir en la simulacion coincide con la fecha de un punto del array de 
		// cargas adicionales ->
		printf("Segundo_Camino\n");
		if (Anyadir_Punto(Array_Puntos_Simulacion, Fecha_Siguiente, Delta_Simulacion, Numero_Puntos_Simulacion)) {
			return -1;
		}
		(*Punto_Actual_Array_Carga)++;
	}
	else {
		// Si el siguiente punto a añadir no coincide con un punto especial (Hora en punto, o inicio o fin de carga)
		// se añade un punto sumandole al punto anterior el delta t.
		printf("Tercero Camino \n");
		if (Anyadir_Punto(Array_Puntos_Simulacion, Fecha_Siguiente, Delta_Simulacion, Numero_Puntos_Simulacion)) {
			return -1;
		}
	}
	//*Fecha_Siguiente = Calcular_Siguiente_Punto(Delta_Simulacion,*Fecha_Siguiente);
	return 0;
}
//Libero la memoria asociada al array que contiene los puntos adicionales

int Incluir_Fecha_Final(Puntos_Optimizacion **Array_Puntos_Simulacion,int *Numero_Puntos_Simulacion,
	                     const struct tm Fecha_Final) {
	//Este subprograma se utiliza para cargar el ultimo punto de la simulacion que coincide con la fecha final de
	// ejecucion del algoritmo al array de los puntos de simulacion.
	//Cargo la fecha del punto del array->
	struct tm Fecha_Anterior = (*Array_Puntos_Simulacion)[*Numero_Puntos_Simulacion-1].Fecha;
	(*Numero_Puntos_Simulacion)++;
	*Array_Puntos_Simulacion = (Puntos_Optimizacion*)realloc(*Array_Puntos_Simulacion, *Numero_Puntos_Simulacion * sizeof(Puntos_Optimizacion));
	//Redimensiono la memoria reservada para el array->
	if (*Array_Puntos_Simulacion == NULL) {
		return -1;
	}
	//Obtengo la diferencia en minutos con el punto anterior->
	printf("El minuto de la fecha anterior es->\n");
	printf("%d \n", Fecha_Anterior.tm_min);
	printf("El minuto de la fecha siguiente es-> \n");
	printf("%d \n", Fecha_Final.tm_min);
	int Delta_Ultimo = Obtener_Diferencia_Minutos(Fecha_Final, Fecha_Anterior);
	//Se carga la fecha de finalizacion de la simulacion en el array->
	(*Array_Puntos_Simulacion)[*Numero_Puntos_Simulacion-1].Fecha = Fecha_Final;
	(*Array_Puntos_Simulacion)[*Numero_Puntos_Simulacion-1].Delta_Simulacion =  Delta_Ultimo;
	(*Array_Puntos_Simulacion)[*Numero_Puntos_Simulacion-1].Punto_Simulacion = (*Numero_Puntos_Simulacion)-1;

	return 0;
}
int Configurar_Puntos_Simulacion(const Celda*** Data_Vehiculos, const Celda*** Data_Tiempo,
	Puntos_Optimizacion** Array_Puntos_Simulacion, const int* Filas_CSV_Vehiculo, int* Numero_Puntos_Simulacion) {
	
	//Este subprgograma crea un array que relaciona cada puntos de la simulacion con una fecha->
    //Se crean una serie de variables para poder crear el array de puntos de simulacion

	*Numero_Puntos_Simulacion = 1;
	struct tm Fecha_Inicial   = { 0 };
	struct tm Fecha_Final     = { 0 };
	struct tm Fecha_Siguiente = { 0 };
	Cargar_Fecha_Inicial_Final(&Fecha_Inicial, &Fecha_Final, Data_Tiempo);
	Puntos_Adicionales* Array_Puntos_Adicionales = NULL;
	int Punto_Actual_Array_Carga = 1;
	int Total_Puntos_Array_Adicional = 0;

	//Se carga en el array que contiene los puntos de simulacion con sus fechas asociadas la fecha
	//inicial.

	if (Inicializar_Array_Puntos_Simulacion(Array_Puntos_Simulacion, Data_Tiempo) == -1) {
		printf("La inicializacion del array de puntos de simulacion no ha sido correcta");
		return -1;
	};


	//Se carga el valor de diferencia en minutos que hay entre puntos de la simulacion--->
	//Se crea una variable para almacenar el delta temporal que es variable->
	const int Delta_Simulacion = Data_Tiempo[FILA_PARAMETROS_TIEMPO][COLUMNA_DELTA_SIMULACION]->data.dbl;
	int Delta_Variable = 0;

	

	//Se obtiene el array en el que viene registrado los inicios y finalizacion de la carga
	if (Obtener_Array_Puntos_Adicionales(Data_Vehiculos, Data_Tiempo, &Array_Puntos_Adicionales, Filas_CSV_Vehiculo, &Total_Puntos_Array_Adicional)) {
		printf("El array de puntos adicionales de simulacion no se ha creado correctamente \n");
		return -1;
	}
	
	//Se calcula la fecha siguiente a la fecha inicial sumandole el delta t
	Fecha_Siguiente = Calcular_Siguiente_Punto(Delta_Simulacion, Array_Puntos_Simulacion[0]->Fecha);

	
	//Se imprime por pantalla la fecha->
	
	//En este bucle mientras no se alcanze la fecha final de ejecucion del algoritmo se van añadiendo los puntos
	//adicionales al array.
	
	while (!Comprobar_Fin_Simulacion(Fecha_Siguiente,Fecha_Final,&Punto_Actual_Array_Carga,&Total_Puntos_Array_Adicional)) {
		//Se calcula el punto siguiente de simulacion que corresponderia sumandole el delta t
		if (Aumentar_Array_Simulacion(Array_Puntos_Simulacion, &Array_Puntos_Adicionales, Delta_Simulacion,
			Numero_Puntos_Simulacion, &Fecha_Siguiente, &Punto_Actual_Array_Carga, Total_Puntos_Array_Adicional) == -1) {
			return -1;
		}
		//Se calcula la fecha siguiente a añadir en el array utilizando el delta de la simulacion y la fecha
		//actual.
		Fecha_Siguiente = Calcular_Siguiente_Punto(Delta_Simulacion, Fecha_Siguiente);
	}

	//Escribir_Array_Puntos_Simulacion(*Array_Puntos_Simulacion, Numero_Puntos_Simulacion);
	//Se incluye en el array de puntos de la simulacion, la fecha final de ejecucion del algoritmo->
	
	if (Incluir_Fecha_Final(Array_Puntos_Simulacion, Numero_Puntos_Simulacion, Fecha_Final) == -1) {
		printf("La fecha final no ha sido añadida correctamente");
		return -1;
	}
	Escribir_Array_Puntos_Simulacion(*Array_Puntos_Simulacion, Numero_Puntos_Simulacion);
	//Libero la memoria asociada al array que contiene los puntos adicionales
	free(Array_Puntos_Adicionales);
	//Se retorna el valor 0 si no ha habido ningun error.
	return 0;
}
void Configurar_Ajustes(OSQPSettings* settings) {
	osqp_set_default_settings(settings);
	// Set your specific settings
	settings->eps_abs = 1e-6; // Absolute convergence tolerance
	settings->eps_rel = 1e-6; // Relative convergence tolerance
	settings->max_iter = 500000; // Maximum number of iterations
	settings->adaptive_rho = 1; // Enable adaptive rho
	settings->adaptive_rho_interval = 60; // Update rho every 60 iterations
}




