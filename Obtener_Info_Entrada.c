#pragma warning(disable: 4996)
#include <stdio.h>
#include <string.h>
#include <stdbool.h>
#include <stdlib.h>
#include "preparar_arrays.h"
#include "Tipos_Optimizacion.h"
#include "osqp.h"
//Variables que se usan para escribir en el struct, de las variables que indican si se tienen en cuenta los objetivos
//de optimizacion
#define Obj_Minimizar_Coste 0
#define Obj_Cargar_Vehiculo 1
#define Obj_Vida_Bat 2
#define Obj_Referencia_Operador 3
#define Obj_Bateria_Adicional 4
#define Max_Bateria 500    //Se establece la capacidad maxima de la bateria 500 kWh
//Defino una serie de variables, para establecer el tamaño del array
#define MAX_FILAS    40
#define MAX_COLUMNAS 40

Datos_CSV *** Leer_CSV(const char* Nombre_Archivo, int* Filas, int* Columnas) {
	//Subprograma que se utiliza para leer el fichero con extensión csv donde viene incluido la información de 
	//los vehículos que se van a cargar.
	FILE* file = fopen(Nombre_Archivo, "r");
	if (file == NULL) {
		printf("Error abriendo el archivo \n");
		return NULL;
	}
	Celda*** data = (Celda***)malloc(MAX_FILAS * sizeof(Celda**));
	if (data == NULL) {
		printf("Error de reserva de memoria \n");
		fclose(file);
		return NULL;
	}
	char line[1000]; //Asumiendo una linea maxima de 1000 caracteres
	int fila = 0;
	while (fgets(line, sizeof(line), file) != NULL && fila < MAX_FILAS) {
		line[strcspn(line, "\n")] = '\0';
		//Se reserva memoria en el array para almacenar los datos del array.
		data[fila] = (Celda**)malloc(MAX_COLUMNAS * sizeof(Celda*));
		if (data[fila] == NULL) {
			printf("Error de reserva de memoria");
			fclose(file);

			//Se libera memoria reservada previamente si ha habido un error en la reserva de la memoria.
			for (int i = 0; i < fila; i++) {
				for (int j = 0; j < Columnas; j++) {
					if (data[i][j]->Tipo == STRING) {
						free(data[i][j]->data.str);
					}
					free(data[i][j]);
				}
				free(data[i]);

			}
			free(data);
			return NULL;
		}
		//Se crea un puntero llamado token para apuntar al primer caracter de cada celda del CSV.
		char* token;
		int   col = 0;
		token = strtok(line, ",");
		while (token != NULL && col < MAX_COLUMNAS) {
			//Saltar tokens vacios
			if (strlen(token) == 0) {
				token = strtok(NULL, ",");
				continue;
			}
			char* end;
			double value = strtod(token, &end);
			data[fila][col] = (Celda*)malloc(sizeof(Celda));
			//Si el dato es de tipo string->
			if (end == token) {
				data[fila][col]->Tipo = STRING;
				data[fila][col]->data.str = strdup(token);
			}
			else {
				data[fila][col]->Tipo = DOUBLE1;
				data[fila][col]->data.dbl = value;
			}
			token = strtok(NULL, ",");
			col++;
		}
		//Si una fila tiene menos columnas que el resto se tiene esto en cuenta.
	
		//Se actualiza el numero de columnas que se leen par esta red.
		if (fila == 0) {
			*Columnas = col;
		}
		fila++;
	}
	   fclose(file);
	   *Filas = fila;
	   return data;
}

void freeCSV(Celda *** data, int filas, int columnas) {
	//Subprograma que se utiliza para liberar m
	for (int i = 0; i < filas; i++) {
		for (int j = 0; j < columnas; j++) {
			if (data[i][j]->Tipo == STRING) {
				free(data[i][j]->data.str);
				data[i][j]->data.str = NULL;
			}
			free(data[i][j]);
		}
		free(data[i]);
	}
	free(data);
}
static int Leer_Objetivo_Usuario(const char* Mensaje) {
	//Este subprograma lee la variable booleana que indica si cada determinado objetivo se tiene en cuenta.
	int Valor;
	printf("%s", Mensaje);
	if (((scanf("%d", &Valor)) != 1) || (Valor != 0 && Valor != 1)) {
		printf("Lectura de objetivo fallida \n");
		return -1;
	}
	return Valor;
}

static bool Validar_Hora(const int Hora) {
	//Se comprueba que la hora este en un rango entre las 0 y las 23 Horas.s
	if (Hora < 0 || Hora >23) {
		printf("Las Horas de Inicio y finalizacion tienen que estar entre las 0 y 23 Horas \n");
		return false;
	}
	return true;
}
static bool Validar_Minutos(const int Minutos) {
	//Se comprueba que los minutos esten entre los 0 y los 59 minutos
	if ((Minutos < 0) || (Minutos > 59)) {
		return false;
	}
	return true;
}
static int Asignar_Objetivo(Objetivos* Objetivos_Optimizacion, const int Index, const char* Mensaje) {
	//Este subprograma introduce en la variable de los objetivos que objetivos se tienen en cuenta.
	int Input = Leer_Objetivo_Usuario(Mensaje);
	if (Input == -1) return -1;
	switch (Index) {
	case Obj_Minimizar_Coste:
		Objetivos_Optimizacion->Minimizar_Coste = Input;
		break;
	case Obj_Cargar_Vehiculo:
		Objetivos_Optimizacion->Cargar_Vehiculo = Input;
		break;
	case Obj_Vida_Bat:
		Objetivos_Optimizacion->Obj_Vida_Bateria = Input;
		break;
	case Obj_Referencia_Operador:
		Objetivos_Optimizacion->Obj_Referencia_Red = Input;
		break;
	case Obj_Bateria_Adicional:
		Objetivos_Optimizacion->Obj_Cargar_Bateria = Input;
		break;
	default:
		printf("Invalid Index");
		return -1;
	}
	return 0;
}

int Leer_Tipos_Objetivos(Objetivos* Objetivos_Optimizacion) {
	//Se usa este subprograma para que se indique que objetivos se van a tener en cuenta en la optimizacion.
	if (Asignar_Objetivo(Objetivos_Optimizacion, Obj_Minimizar_Coste, "Introduzca si se tiene en cuenta el objetivo del precio") == -1) {
		return -1;
	}
	if (Asignar_Objetivo(Objetivos_Optimizacion, Obj_Cargar_Vehiculo, "Introduzca si se tiene en cuenta el objetivo de carga del vehiculo") == -1) {
		return -1;
	}
	if (Asignar_Objetivo(Objetivos_Optimizacion, Obj_Vida_Bat, "Introduzca si se tiene en cuenta el objetivo de vida de la bateria") == -1) {
		return -1;
	}
	if (Asignar_Objetivo(Objetivos_Optimizacion, Obj_Referencia_Operador, "Introduzca si se tiene en cuenta el objetivo de referencia de la red") == -1) {
		return -1;
	}
	if (Asignar_Objetivo(Objetivos_Optimizacion, Obj_Bateria_Adicional, "Introduzca si se tiene en cuenta el objetivo de carga del Terminal") == -1) {
		return -1;
	}
	return 0;
}



double Obtener_Potencia(const char* Mensaje) {
	double Potencia;
	printf("%s\n", Mensaje);
	if ((scanf("%lf", &Potencia) != 1) || (Potencia < 0)) {
		printf("La restriccion introducida no es valida \n");
		return -1;
	}
	return Potencia;
}
/*
int Obtener_Restricciones_Red_Electrolinera(Restricciones_Electrolinera* Restricciones) {
	//En este subprograma se fijan las restricciones para la potencia que puede intercambiar la red con
	//la electrolinera. 
	//Valores Fijos:
	Restricciones->Minima_Potencia_Entrada_Red_R = 0;
	Restricciones->Minima_Potencia_Entrada_Red_S = 0;
	Restricciones->Minima_Potencia_Entrada_Red_T = 0;
	Restricciones->Minima_Potencia_Salida_Red_R = 0;
	Restricciones->Minima_Potencia_Salida_Red_S = 0;
	Restricciones->Minima_Potencia_Salida_Red_T = 0;
	Restricciones->Minima_Potencia_Red_Entrada = 0;
	Restricciones->Minima_Potencia_Red_Salida = 0;
	//Se proceden a cargar los valores Maximos que puede intercambiar la electrolinera con la red.
	printf("Introduzca la maxima potencia que puede llegar de la red a la electrolinera \n");
	//Almaceno los valores introducidos en dos variables
	double Max_In = Obtener_Potencia("Introduzca la maxima potencia que puede llegar de la red a la electrolinera");
	double Max_Out = Obtener_Potencia("Introduzca la maxima potencia que puede volcar la electrolinera a la red");

	if (Max_In < 0) return -1;
	Restricciones->Maxima_Potencia_Red_Entrada = Max_In;

	if (Max_Out < 0) return -1;
	Restricciones->Maxima_Potencia_Red_Salida = Max_Out;

	//Se asume que el maximo por fase es idéntico.
	Restricciones->Maxima_Potencia_Entrada_Red_R = Max_In / 3;
	Restricciones->Maxima_Potencia_Entrada_Red_S = Max_In / 3;
	Restricciones->Maxima_Potencia_Entrada_Red_T = Max_In / 3;

	Restricciones->Maxima_Potencia_Salida_Red_R = Max_Out / 3;
	Restricciones->Maxima_Potencia_Salida_Red_S = Max_Out / 3;
	Restricciones->Maxima_Potencia_Salida_Red_T = Max_Out / 3;

	Restricciones->Maximo_Potencia_Red_R = Max_In / 3;
	Restricciones->Maximo_Potencia_Red_S = Max_In / 3;
	Restricciones->Maximo_Potencia_Red_T = Max_In / 3;

	Restricciones->Minima_Potencia_Red_R = -Max_Out / 3;
	Restricciones->Minima_Potencia_Red_S = -Max_Out / 3;
	Restricciones->Minima_Potencia_Red_T = -Max_Out / 3;

	//Pongo los valores minimos y maximos de la potencia global que la red puede intercambiar con el sistema
	Restricciones->Maxima_Potencia_Red = Max_In;
	Restricciones->Minima_Potencia_Red = -Max_Out;
	
	return 0;
}
*/
static int Fijar_Tipo_Tarifa(Vehiculo* Coche, const char* str) {
	if (strcmp(str, "Estandar") == 0) {
		Coche->Modo_Carga = Estandar;
		return 0;
	}
	else if (strcmp(str, "Prisa") == 0) {
		Coche->Modo_Carga = Prisa;
		return 0;
	}
	else {
		return -1;
	}
}
int Leer_Parametro_Vehiculo_Doble(double* Parametro_Vehiculo, double min, double max, const char* Mensaje) {
	//Este Subprograma se utiliza 
	printf("%s", Mensaje);
	if ((scanf("%lf", Parametro_Vehiculo) != 1) || (*Parametro_Vehiculo < min) || (*Parametro_Vehiculo > max)) {
		printf("Input erróneo. Por favor introduzca un valor entre %.2f y  %.2f \n", min, max);
		return -1;
	}
	return 0;
}
int Leer_Parametro_Vehiculo_Integer(int* Parametro_Vehiculo, const char* Mensaje) {
	printf("%s", Mensaje);
	if ((scanf("%d", Parametro_Vehiculo) != 1)) {
		printf("Input invalido.Por favor introduzca un integer.\n");
		return -1;
	}
	return 0;
}
int Leer_Parametro_String(char* Parametro_Vehiculo, int Longitud, const char* Mensaje) {
	printf("%s", Mensaje);
	fgets(Parametro_Vehiculo, Longitud, stdin);
	return 0;
}
static int Convertir_A_Minutos(const int Dia, const int Hora, const int Minuto) {
	//Este subprograma convierte el tiempo a minutos
	return Dia * 24 * 60 + Hora * 60 + Minuto;
}
static int Comprobar_Rango(const int Tiempo_Coche, const int Tiempo_Inicio_Simulacion, const int Tiempo_Fin_Simulacion) {
	//Esta Función auxilair sirve para comprobar si el tiempo de carga del vehiculo esta en el rango del tiempo
	//de la simulacion
	return(Tiempo_Coche >= Tiempo_Inicio_Simulacion) && (Tiempo_Coche <= Tiempo_Fin_Simulacion);
}
static int Validar_Tiempo_Vehiculo(const Vehiculo Coche) {
	//Se comprueba que la hora del coche y el minuto tienen sentido logico.
	if ((!Validar_Hora(Coche.Hora_Inicial_Carga)) || !(Validar_Hora(Coche.Hora_Final_Carga))) {
		return -1;
	}
	if ((!(Validar_Minutos(Coche.Minuto_Final_Carga)) || (!(Validar_Minutos(Coche.Minuto_Inicial_Carga))))) {
		return -1;
	}
	return 0;
}
static int Comprobar_Tiempo_Vehiculo(const Caracteristicas_Simulacion Tiempo_Simulacion, const Vehiculo Coche) {
	if (Validar_Tiempo_Vehiculo(Coche)) {
		return -1;
	}
	int Comienzo_Simulacion = Convertir_A_Minutos(Tiempo_Simulacion.Dia_Inicio, Tiempo_Simulacion.Hora_Inicio, Tiempo_Simulacion.Minuto_Inicial);
	int Final_Simulacion = Convertir_A_Minutos(Tiempo_Simulacion.Dia_Final, Tiempo_Simulacion.Hora_Final, Tiempo_Simulacion.Minuto_Final);
	//Convierto la fecha de inicio de carga del Vehiculo a minutos
	int Comienzo_Carga = Convertir_A_Minutos(Coche.Dia_Inicial_Carga, Coche.Hora_Inicial_Carga, Coche.Minuto_Inicial_Carga);
	int Fin_Carga = Convertir_A_Minutos(Coche.Dia_Final_Carga, Coche.Hora_Final_Carga, Coche.Minuto_Final_Carga);
	//Se comprueba que el tiempo de comienzo de la carga del Vehiculo y de finalizacion de la carga del vehiculo 
	//está dentro del rango de tiempo de la simulacion 
	if (!Comprobar_Rango(Comienzo_Carga, Comienzo_Simulacion, Final_Simulacion) || !Comprobar_Rango(Fin_Carga, Comienzo_Simulacion, Final_Simulacion)) {
		return -1;
	}
	return 0;
}

static int Calcular_Puntos_Simulacion(const Caracteristicas_Simulacion Tiempo_Simulacion, Vehiculo* Coche) {
	if (!Comprobar_Tiempo_Vehiculo(Tiempo_Simulacion, *Coche)) {
		return -1;
	}
	//Variables para poder situar los puntos de comienzo y finalizacion de la carga del coche
	int Delta_Index_Primer = Tiempo_Simulacion.Delta_Primer_Punto / (Tiempo_Simulacion.Resolucion_Minutos);
	int Delta_Index_Ultimo = Tiempo_Simulacion.Delta_Ultimo_Punto / (Tiempo_Simulacion.Resolucion_Minutos);
	//Se encuentra el primer punto de simulacion que coincide o excede el comienzo de carga del coche
	for (int i = 0; i < Tiempo_Simulacion.Puntos_Simulacion; i++) {
		if (Tiempo_Simulacion.Puntos_Tiempo_Real[i].Dia > Coche->Dia_Inicial_Carga ||
			(Tiempo_Simulacion.Puntos_Tiempo_Real[i].Dia == Coche->Dia_Inicial_Carga &&
				(Tiempo_Simulacion.Puntos_Tiempo_Real[i].Hora > Coche->Hora_Inicial_Carga ||
					(Tiempo_Simulacion.Puntos_Tiempo_Real[i].Hora == Coche->Hora_Inicial_Carga &&
						Tiempo_Simulacion.Puntos_Tiempo_Real[i].Minuto >= Coche->Minuto_Inicial_Carga)))) {
			//Hay diferencia si coincide con el primer o el último punto de la simulación.

			if (i == 0) {
				Coche->Punto_Inicio_Carga = i + (Coche->Minuto_Inicial_Carga / (Tiempo_Simulacion.Resolucion_Minutos) - Delta_Index_Primer);
			}
			else {
				Coche->Punto_Inicio_Carga = i;
			}
			break;
		}
	}
	//Encuentra el punto de simulacion que coincide con la fecha de finalizacion del coche.
	for (int i = Tiempo_Simulacion.Puntos_Simulacion - 1; i >= 0; i--) {
		if (Tiempo_Simulacion.Puntos_Tiempo_Real[i].Dia < Coche->Dia_Final_Carga ||
			(Tiempo_Simulacion.Puntos_Tiempo_Real[i].Dia == Coche->Dia_Final_Carga &&
				(Tiempo_Simulacion.Puntos_Tiempo_Real[i].Hora < Coche->Hora_Final_Carga ||
					(Tiempo_Simulacion.Puntos_Tiempo_Real[i].Hora == Coche->Hora_Final_Carga &&
						Tiempo_Simulacion.Puntos_Tiempo_Real[i].Minuto <= Coche->Minuto_Final_Carga)))) {

			//Si es el ultimo punto de simulacion ajustar por la diferencia de tiempo.
			if (i == Tiempo_Simulacion.Puntos_Simulacion - 1) {
				Coche->Punto_Final_Carga = i - (Coche->Minuto_Final_Carga / Tiempo_Simulacion.Resolucion_Minutos) + Delta_Index_Ultimo;
			}
			else {
				Coche->Punto_Final_Carga = i;
			}
			break;
		}
	}
	return 0;
}


static int Leer_Caracteristicas_Vehiculo(Vehiculo* Coche, const Caracteristicas_Simulacion Tiempo_Simulacion) {
	//En este subprograma se leen las caracteristicas del Vehiculo.
	if (Leer_Parametro_Vehiculo_Doble(&Coche->Capacidad_Bateria, 0, 500, "Introduzca la capacidad de la bateria \n") < 0) {
		return -1;
	}

	if (Leer_Parametro_Vehiculo_Doble(&Coche->Estado_Inicial_Bateria, 0, 1, "Introduzca el estado inicial de la bateria \n") < 0) {
		return -1;
	}
	if (Leer_Parametro_Vehiculo_Doble(&Coche->Estado_Deseado_Bateria, 0, 1, "Introduzca el estado deseado de la bateria \n") < 0) {
		return -1;
	}
	char Tarifa[11];                             //Buffer para almacenar el tipo de tarifa.
	Leer_Parametro_String(Tarifa, sizeof(Tarifa), "Introduzca el tipo de tarifa Prisa o Estándar");
	if (Fijar_Tipo_Tarifa(Coche, Tarifa) == -1) { //Se Lee el tipo de tarifa.
		return -1;
	}
	//Se Lee el Maximo estado de carga en porcentaje de 0 a 1 que 
	if (Leer_Parametro_Vehiculo_Doble(&Coche->Limite_Bateria, 0, 1, "Introduzca el limite de bateria de 0 a 1") == -1) {
		return -1;
	}
	//Se Lee el limite de la potencia de carga.
	if (Leer_Parametro_Vehiculo_Doble(&Coche->Limite_Potencia_Carga, 0, 350, "Introduzca limite de carga") == -1) {
		return -1;
	}
	//Se Lee los tiempos iniciales y finales de Carga:
	if (Leer_Parametro_Vehiculo_Integer(&Coche->Dia_Inicial_Carga, "Introduzca el Dia inicial de la carga")) {
		return -1;
	}
	if (Leer_Parametro_Vehiculo_Integer(&Coche->Dia_Final_Carga, "Introduzca el Dia final de la carga")) {
		return -1;
	}
	if (Leer_Parametro_Vehiculo_Integer(&Coche->Hora_Inicial_Carga, "Introduzca la hora inicial de la carga")) {
		return -1;
	}
	if (Leer_Parametro_Vehiculo_Integer(&Coche->Hora_Final_Carga, "Introduzca la hora final de la carga")) {
		return -1;
	}
	if (Leer_Parametro_Vehiculo_Integer(&Coche->Minuto_Inicial_Carga, "Introduzca el minuto inicial de carga")) {
		return -1;
	}
	if (Leer_Parametro_Vehiculo_Integer(&Coche->Minuto_Final_Carga, "Introduzca el minuto final de carga")) {
		return -1;
	}
	//Se calcula los puntos de simulacion del vehiculo.
	if (!Calcular_Puntos_Simulacion(Tiempo_Simulacion, Coche)) {
		return -1;
	}
	return 0;
}


static int Leer_Caracteristicas_Estacion(Estacion* Puesto_Carga, const Caracteristicas_Simulacion Tiempo_Simulacion) {
	//En este subprograma se leen las caracteristicas del puesto de carga
	printf("Introduzca el Numero de Vehiculos que tienen programada la carga en la estacion \n");
	if ((scanf("%d", &Puesto_Carga->Num_Vehiculos) != 1) || (Puesto_Carga->Num_Vehiculos) < 0) {
		printf("No se ha introducido el numero de vehiculos \n");
		return -1;
	}
	printf("Introduzca la fase a la que esta conectada la estacion carga R,S,T \n");
	if ((scanf("%c", &Puesto_Carga->Fase_Red) != 1) || (Puesto_Carga->Fase_Red != 'R') || (Puesto_Carga->Fase_Red != 'S') || (Puesto_Carga->Fase_Red != 'T')) {
		printf("La fase que se ha introducido no es correcta \n");
		return -1;
	}
	for (int i = 0; i < Puesto_Carga->Num_Vehiculos; i++) {
		//Se leen las caracteristicas de tiempo del vehiculo.
		if (Leer_Caracteristicas_Vehiculo(&Puesto_Carga->Vehiculos[i], Tiempo_Simulacion)) {
			return -1;
		}
	}
	return 0;
}
int Obtener_Informacion_Electrolinera(Sistema_Carga* Electrolinera, const Caracteristicas_Simulacion Tiempo_Simulacion) {
	//En este subprograma se obtienen los datos de la electrolinera.
	printf("Introduzca el numero de estaciones de carga en la electrolinera");
	if ((scanf("%d", &Electrolinera->Numero_Estaciones) != 1) || (Electrolinera->Numero_Estaciones) < 0) {
		printf("Los Datos del Numero de Estaciones no tienen sentido \n");
		return -1;
	}
	//Se reserva memoria para almacenar la variable
	Electrolinera->Estaciones_Carga = malloc(Electrolinera->Numero_Estaciones * sizeof(Estacion));
	if (Electrolinera->Estaciones_Carga == NULL) {
		printf("Error: No se puede reservar memoria para la Estacion \n");
		return -1;
	}
	for (int i = 0; i < Electrolinera->Numero_Estaciones; i++) {
		Leer_Caracteristicas_Estacion(&Electrolinera->Estaciones_Carga[i], Tiempo_Simulacion);
	}
	return 0;
}



static bool Validar_Tiempo_Simulacion(const Caracteristicas_Simulacion* Tiempo_Simulacion) {
	//En este subprograma se comprueba que los datos introducidos tengan un sentido logico, basicamente
	//que el inicio del algoritmo en el tiempo no esté temporalmente después del punto final. Asi como 
	//que la resolucion en minutos sea lógica.
	if (Tiempo_Simulacion->Dia_Inicio > Tiempo_Simulacion->Dia_Final) { //El Dia de Inicio no puede ser mas tarde que el final.
		printf("El Dia de inicio no puede ser despues del Dia Final.\n");
		return false;
	}
	if (Tiempo_Simulacion->Dia_Inicio == Tiempo_Simulacion->Dia_Final) {
		//Si el Dia de Inicio y el Dia Final de la simulacion son iguales la Hora de inicio debe ser anterior a la Hora final.
		if (Tiempo_Simulacion->Hora_Inicio > Tiempo_Simulacion->Hora_Final) {
			printf("La Hora de inicio no puede ser mas tarde que la hora final\n");
			return false;
		}
		//Si Coinciden tanto el dia Inicial como el Final y tambien la Hora se tiene que cumplir que el minuto Inicial ha de ser anterior al final
		if ((Tiempo_Simulacion->Hora_Inicio) == Tiempo_Simulacion->Hora_Final) {
			if ((Tiempo_Simulacion->Minuto_Final) <= (Tiempo_Simulacion->Minuto_Inicial)) {
				printf("El Minuto inicial no puede ser anterior al punto final \n");
				return false;

			}
		}
	}
	//Se comprueba que las Horas introducidas estan entre las 0 y las 23 horas.
	if ((!Validar_Hora(Tiempo_Simulacion->Hora_Inicio)) || (!Validar_Hora(Tiempo_Simulacion->Hora_Final))) {
		printf("Las Horas introducidas han de estar entre las 0 y las 23 Horas \n");
		return false;
	}

	//Los datos de los minutos de inicio y de salida  han de estar entre 0 y 60 minutos
	if (!Validar_Minutos(Tiempo_Simulacion->Minuto_Inicial) || (!Validar_Minutos(Tiempo_Simulacion->Minuto_Final))) {
		printf("Los Minutos inicial y final han de estar entre los 0 y 60 Minutos\n");
		return false;
	}
	//La resolucion en Minutos ha de ser un divisor de 60 y no superar 60
	if ((!Validar_Minutos(Tiempo_Simulacion->Resolucion_Minutos)) || !(60 % Tiempo_Simulacion->Resolucion_Minutos != 0)) {
		printf("La Resolucion en minutos de la simulacion ha de ser un divisor de 60 \n");
		return false;
	}
	return true;
}
static int Leer_Datos_Tiempo(Caracteristicas_Simulacion* Tiempo_Simulacion) {
	//En este subprograma se leen las caracteristicas temporales de la Simulacion.
	printf("Introduzca el dia de inicio de la simulacion \n");
	if (scanf("%d", &(Tiempo_Simulacion->Dia_Inicio)) != 1) {
		printf("No se ha introducido bien el dia de inicio \n");
		return -1;
	}
	printf("Introduzca el dia de finalizacion de la simulacion \n");
	if (scanf("%d", &(Tiempo_Simulacion->Dia_Final)) != 1) {
		printf("No se ha introducido bien el dia final \n");
		return -1;
	}
	//A continuacion se leen la Hora de inicio de la simulacion
	printf("Introduzca la Hora de inicio de la simulacion \n");
	if (scanf("%d", &(Tiempo_Simulacion->Hora_Inicio)) != 1) {
		printf("La Hora de inicio introducida no es correcta \n");
		return -1;
	}
	//A continuacion se introduce la Hora de finalizacion deL Algoritmo.
	printf("Introduzca la Hora de Finalizacion de la simulacion \n");
	if (scanf("%d", &(Tiempo_Simulacion->Hora_Final)) != 1) {
		printf("La Hora de Finalizacion introducida no es correcta \n");
		return -1;
	}
	//A continuacion se introduce el minuto de Inicio del Algoritmo 
	if (scanf("%d", &(Tiempo_Simulacion->Minuto_Inicial)) != 1) {
		printf("El Dato del minuto inicial del algoritmo no tiene sentido \n");
		return -1;
	}
	//A continuacion se introduce el minuto de Finalizacion del Algoritmo
	if (scanf("%d", &(Tiempo_Simulacion->Minuto_Final)) != 1) {
		printf("El Dato del minuto final del algoritmo no tiene sentido\n");
		return -1;
	}
	//A continuacion se introduce la resolucion en Minutos_del Algoritmo.
	if (scanf("%d", &(Tiempo_Simulacion->Resolucion_Minutos)) != 1) {
		printf("El dato de la resolucion de los minutos no tiene sentido \n");
		return -1;
	}
	//Se comprueba que los datos de tiempo introducidos son correctos
	if (!Validar_Tiempo_Simulacion(Tiempo_Simulacion)) {
		return -1;
	}
	return 0;
}

static int Calcular_Caracteristicas_Tiempo(Caracteristicas_Simulacion* Tiempo_Simulacion) {
	//Este subprograma se encarga de calcular las caracteristicas de tiempo de la simulacion.
	int Total_Minutos_Inicio = (Tiempo_Simulacion->Dia_Inicio * 1440) + (Tiempo_Simulacion->Hora_Inicio * 60) + (Tiempo_Simulacion->Minuto_Inicial);
	int Total_Minutos_Final = (Tiempo_Simulacion->Dia_Final * 1440) + (Tiempo_Simulacion->Hora_Final * 60) + (Tiempo_Simulacion->Minuto_Final);
	int Diferencia_Total_Minutos = Total_Minutos_Final - Total_Minutos_Inicio;
	int Delta_Minutos = Tiempo_Simulacion->Resolucion_Minutos;

	int Numero_Horas = Diferencia_Total_Minutos / 60;
	if (Diferencia_Total_Minutos % 60 != 0) {
		Numero_Horas += 1;
	}
	Tiempo_Simulacion->Numero_Horas = Numero_Horas;
	//Calculo el numero total de puntos de simulacion

	//Calculo el delta de tiempo del instante inicial al primer Punto de simulacion.

	Total_Minutos_Inicio = (Tiempo_Simulacion->Hora_Inicio * 60) + Tiempo_Simulacion->Minuto_Inicial;
	int Primer_Punto_Minutos = ((Total_Minutos_Inicio + Delta_Minutos - 1) / Delta_Minutos) * Delta_Minutos;
	Tiempo_Simulacion->Delta_Primer_Punto = Primer_Punto_Minutos - Total_Minutos_Inicio;

	//Calculo el delta de tiempo del penultimo punto al ultimo.

	int Total_Minutos_Ult_Final = (Tiempo_Simulacion->Hora_Final * 60) + Tiempo_Simulacion->Minuto_Final;
	int Ultimo_Punto_Minutos = (Total_Minutos_Ult_Final / Delta_Minutos) * Delta_Minutos;
	Tiempo_Simulacion->Delta_Ultimo_Punto = Total_Minutos_Ult_Final - Ultimo_Punto_Minutos;

	//Calculo el numero total de puntos de simulacion
	int Puntos_Simulacion = Diferencia_Total_Minutos / Delta_Minutos;
	if (Diferencia_Total_Minutos % Delta_Minutos != 0) {
		Puntos_Simulacion += 1;
	}
	Tiempo_Simulacion->Puntos_Simulacion = Puntos_Simulacion;

	//Calculo el Array que identifica a cada punto de la simulacion con un tiempo real
	Tiempo_Simulacion->Puntos_Tiempo_Real = (Puntos_Tiempo*)calloc(Puntos_Simulacion, sizeof(Puntos_Tiempo));
	if (Tiempo_Simulacion->Puntos_Tiempo_Real == NULL) {
		//Se maneja el caso que la reserva de memoria haya fallado.
		printf("La Reserva de memoria ha fallado \n");
		return -1;
	}
	//Lleno el Array haciendo que cada Punto de simulacion equivalga a un tiempo real, esto sera util, cuando se compare con los datos de los 
	//Vehiculos.
	int Total_Minutos_Puntos = Total_Minutos_Inicio + Tiempo_Simulacion->Delta_Primer_Punto;
	for (int i = 0; i < Tiempo_Simulacion->Puntos_Simulacion; i++) {
		Tiempo_Simulacion->Puntos_Tiempo_Real[i].Dia = Total_Minutos_Puntos / 1440;
		Total_Minutos_Puntos %= 1440;
		Tiempo_Simulacion->Puntos_Tiempo_Real[i].Hora = Total_Minutos_Puntos / 60;
		Tiempo_Simulacion->Puntos_Tiempo_Real[i].Minuto = Total_Minutos_Puntos % 60;
		Tiempo_Simulacion->Puntos_Tiempo_Real[i].Punto = i;
		Total_Minutos_Puntos += Delta_Minutos;
	}
	return 0;
}

int Configurar_Tiempo_Simulacion(Caracteristicas_Simulacion* Tiempo_Simulacion) {
	//Este Subprograma se encarga de leer y calcular los puntos de calculo que realiza el algoritmo, asi como calcular los instantes de tiempo inicial y final
	//asi como la resolucion temporal del Algoritmo esto es, la diferencia en minutos entre los diferentes puntos de la simulacion.
	if (!Leer_Datos_Tiempo(Tiempo_Simulacion)) {
		return -1;
	}
	if (!Calcular_Caracteristicas_Tiempo(Tiempo_Simulacion)) {
		return -1;
	}
	return 0;
}

static int Leer_Hay_Panel_Carga(const char* Mensaje, int* Existe_Panel) {
	//Subprograma que se encarga de leer si hay panel fotovoltaico en cada fase
	printf("%s", Mensaje);
	if (scanf("%d", Existe_Panel) != 1) {
		printf("No se ha introducido un dato de tipo int");
		return -1;
	}
	return 0;
}
static int Leer_Datos_Panel_Carga(double* Dato, int Existe_Panel_Carga, const char* Fase, int Dia_Actual, int Hora_Actual, int Minuto_Actual,
	int Dia_Anterior, int Hora_Anterior, int Minuto_Anterior) {
	if (Existe_Panel_Carga) {
		printf("Potencia del Dia %d Hora %d Minuto %d al Dia %d Hora %d Minuto%d \n", Dia_Actual, Hora_Actual, Minuto_Actual, Dia_Anterior, Hora_Anterior, Minuto_Anterior);
		printf("Fase %s \n", Fase);
		if (scanf("%lf", Dato) != 1) {
			printf("La Lectura del Panel O Carga no ha sido correcta \n");
			return -1;
		}
		if (*Dato < 0) {
			printf("No se pueden introducir valores negativos");
			return -1;
		}
	}
	return 0;
}
//Funcion que sirve para leer si Existe Bateria en cada fase
static int Existe_Bateria(bool* Existe_Bateria_Fase, const char Fase, const char* Mensaje) {
	printf("%s", Mensaje);
	int Temp;
	if (scanf("%d", &(Temp)) != 1) {
		printf("La Lectura de Si hay una bateria o no conectada no es correcta \n");
		return -1;
	}
	*Existe_Bateria_Fase = Temp != 0;
	return 0;
}
static int Leer_Parametro_Bateria(double* Parametro_Bateria, const double min, const double max, const char* Mensaje) {
	//Se leen los parametros de la bateria.
	printf("%s", Mensaje);
	double Temp;
	if ((scanf("%lf", &Temp) != 1) || (Temp < min) || (Temp) > max) {
		printf("El Parametro de la bateria leido no es correcto \n");
		return -1;
	}
	*Parametro_Bateria = Temp != 0;
	return 0;
}

int Leer_Baterias(Bateria* Baterias_Sistema) {
	//En este subprograma se lee los parametros de las baterias conectadas
	//al sistema.
	if ((Existe_Bateria(&Baterias_Sistema->Bateria_R, "R", "Hay Bateria conectada en la fase R \n")) == -1) {
		return -1;
	}
	if ((Existe_Bateria(&Baterias_Sistema->Bateria_S, "S", "Hay Bateria conectada en la fase S \n")) == -1) {
		return -1;
	}
	if ((Existe_Bateria(&Baterias_Sistema->Bateria_T, "T", "Hay Bateria conectada en la fase T \n")) == -1) {
		return -1;
		//Si hay Baterias conectadas en cada fase se leen sus datos. 
		//Se Lee la capacidad de la bateria
		if ((Leer_Parametro_Bateria(&Baterias_Sistema->Capacidad_Bateria_R, 0, 300, "Introduzca la capacidad de la bateria")) == -1) {
			return -1;
		}
		if ((Leer_Parametro_Bateria(&Baterias_Sistema->Capacidad_Bateria_S, 0, 300, "Introduzca la capacidad de la bateria")) == -1) {
			return -1;
		}
		if ((Leer_Parametro_Bateria(&Baterias_Sistema->Capacidad_Bateria_T, 0, 300, "Introduzca la capacidad de la bateria")) == -1) {
			return -1;
		}
		//Se Lee el porcentaje inicial de las baterias conectadas a cada fase.
		if ((Leer_Parametro_Bateria(&Baterias_Sistema->Bateria_Inicial_R, 0, 1, "Introduzca el porcentaje inicial de la bateria conectada a fase R")) == -1) {
			return -1;
		}
		if ((Leer_Parametro_Bateria(&Baterias_Sistema->Bateria_Inicial_S, 0, 1, "Introduzca el porcentaje inicial de la bateria conectada a fase S")) == -1) {
			return -1;
		}
		if ((Leer_Parametro_Bateria(&Baterias_Sistema->Bateria_Inicial_T, 0, 1, "Introduzca el porcentaje inicial de la bateria conectada a fase T")) == -1) {
			return -1;
		}

	}
	return 0;
}

int Obtener_Datos_Panel_Fotovoltaico(Panel_Solar* Paneles_Solares, const Caracteristicas_Simulacion Caracteristicas_Tiempo) {
	//Con este subprograma se obtiene los datos de potencia que se estima que va a generar el panel solar
	const int Puntos_Simulacion = Caracteristicas_Tiempo.Puntos_Simulacion;
	//Reservo Espacio en la memoria para los arrays de los datos de la energia proveniente del panel fotovoltaico
	Paneles_Solares->Panel_R = (double*)(calloc)(Puntos_Simulacion, sizeof(double));
	Paneles_Solares->Panel_S = (double*)(calloc)(Puntos_Simulacion, sizeof(double));
	Paneles_Solares->Panel_T = (double*)(calloc)(Puntos_Simulacion, sizeof(double));
	//Se comprueba si la reserva de memoria ha sido exitosa
	if ((Paneles_Solares->Panel_R == NULL) || (Paneles_Solares->Panel_S == NULL) || (Paneles_Solares->Panel_T == NULL)) {
		return -1;
	}
	//Se lee si se tienen paneles fotovoltaicos conectados a cada fase.
	if (Leer_Hay_Panel_Carga("Hay Paneles fotovoltaicos en la fase R 0 si no hay \n", &(Paneles_Solares->Existe_Panel_R)) == -1)return -1;
	if (Leer_Hay_Panel_Carga("Hay Paneles fotovoltaicos en la fase S 0 si no hay \n", &(Paneles_Solares->Existe_Panel_S)) == -1)return -1;
	if (Leer_Hay_Panel_Carga("Hay Paneles fotovoltaicos en la fase T 0 si no hay \n", &(Paneles_Solares->Existe_Panel_T)) == -1)return -1;
	//Se procede a leer los datos de la estimacion del generacion del panel fotovoltaico.
	//Empieza en 1 porque el primer punto de la simulacion es el punto inicial.
	for (int i = 1; i < Puntos_Simulacion; i++) {
		//Se crean una serie de valores para indicar de que instantes se esta introduciendo datos de paneles fotovoltaicos.
		int Dia_Actual = Caracteristicas_Tiempo.Puntos_Tiempo_Real[i].Dia;
		int Hora_Actual = Caracteristicas_Tiempo.Puntos_Tiempo_Real[i].Hora;
		int Minuto_Actual = Caracteristicas_Tiempo.Puntos_Tiempo_Real[i].Minuto;
		//Escribo aqui los valores de tiempo anterior al valor que se va cargar
		int Dia_Anterior = Caracteristicas_Tiempo.Puntos_Tiempo_Real[i - 1].Dia;
		int Hora_Anterior = Caracteristicas_Tiempo.Puntos_Tiempo_Real[i - 1].Hora;
		int Minuto_Anterior = Caracteristicas_Tiempo.Puntos_Tiempo_Real[i - 1].Minuto;
		if (Leer_Datos_Panel_Carga(&(Paneles_Solares->Panel_R[i]), Paneles_Solares->Existe_Panel_R, "R", Dia_Actual, Hora_Actual, Minuto_Actual, Dia_Anterior, Hora_Anterior, Minuto_Anterior) == -1)return -1;
		if (Leer_Datos_Panel_Carga(&(Paneles_Solares->Panel_S[i]), Paneles_Solares->Existe_Panel_S, "S", Dia_Actual, Hora_Actual, Minuto_Actual, Dia_Anterior, Hora_Anterior, Minuto_Anterior) == -1)return -1;
		if (Leer_Datos_Panel_Carga(&(Paneles_Solares->Panel_T[i]), Paneles_Solares->Existe_Panel_T, "T", Dia_Actual, Hora_Actual, Minuto_Actual, Dia_Anterior, Hora_Anterior, Minuto_Anterior) == -1)return -1;
	}
	printf("Los Datos de fotovoltaica se han leido correctamente \n");
	return 0;
}

int Obtener_Datos_Cargas(Carga_Adicional* Carga, const Caracteristicas_Simulacion Caracteristicas_Tiempo) {
	const int Puntos_Simulacion = Caracteristicas_Tiempo.Puntos_Simulacion;
	//Reservo Espacio en la memoria para almacenar la potencia consumida de la carga
	Carga->Carga_R = (double*)(calloc)(Puntos_Simulacion, sizeof(double));
	Carga->Carga_S = (double*)(calloc)(Puntos_Simulacion, sizeof(double));
	Carga->Carga_T = (double*)(calloc)(Puntos_Simulacion, sizeof(double));
	//Se comprueba si la reserva de memoria ha sido exitosa.
	if (((Carga->Carga_R) == NULL) || ((Carga->Carga_S) == NULL) || ((Carga->Carga_T) == NULL)) {
		printf("La reserva de memoria ha sido erronea");
		return -1;
	}
	//Se comprueba si hay Carga por cada fase:
	if (Leer_Hay_Panel_Carga("Hay Carga en la fase R? 0 si no hay \n", &(Carga->Existe_Carga_R)) == -1)return -1;
	if (Leer_Hay_Panel_Carga("Hay Carga en la fase S? 0 si no hay \n", &(Carga->Existe_Carga_S)) == -1)return -1;
	if (Leer_Hay_Panel_Carga("Hay Carga en la fase T? 0 si no hay \n", &(Carga->Existe_Carga_T)) == -1)return -1;
	for (int i = 1; i < Puntos_Simulacion; i++) {
		//Se crean una serie de valores para indicar de que instantes se esta introduciendo datos de paneles fotovoltaicos.
		int Dia_Actual = Caracteristicas_Tiempo.Puntos_Tiempo_Real[i].Dia;
		int Hora_Actual = Caracteristicas_Tiempo.Puntos_Tiempo_Real[i].Hora;
		int Minuto_Actual = Caracteristicas_Tiempo.Puntos_Tiempo_Real[i].Minuto;
		//Escribo aqui los valores de tiempo anterior al valor que se va cargar
		int Dia_Anterior = Caracteristicas_Tiempo.Puntos_Tiempo_Real[i - 1].Dia;
		int Hora_Anterior = Caracteristicas_Tiempo.Puntos_Tiempo_Real[i - 1].Hora;
		int Minuto_Anterior = Caracteristicas_Tiempo.Puntos_Tiempo_Real[i - 1].Minuto;
		if (Leer_Datos_Panel_Carga(&(Carga->Carga_R[i]), Carga->Existe_Carga_R, "R", Dia_Actual, Hora_Actual, Minuto_Actual, Dia_Anterior, Hora_Anterior, Minuto_Anterior) == -1)return -1;
		if (Leer_Datos_Panel_Carga(&(Carga->Carga_S[i]), Carga->Existe_Carga_S, "S", Dia_Actual, Hora_Actual, Minuto_Actual, Dia_Anterior, Hora_Anterior, Minuto_Anterior) == -1)return -1;
		if (Leer_Datos_Panel_Carga(&(Carga->Carga_T[i]), Carga->Existe_Carga_T, "T", Dia_Actual, Hora_Actual, Minuto_Actual, Dia_Anterior, Hora_Anterior, Minuto_Anterior) == -1)return -1;
	}
	//Se empieza a leer la estimacion del consumo de carga
	printf("Los Datos de Carga se han leido correctamente \n");
	return 0;
}




int Leer_Precios(double** Precio_Compra, double** Precio_Venta, const int Numero_Horas) {
	*Precio_Compra = (double*)calloc(Numero_Horas, sizeof(double));
	*Precio_Venta = (double*)calloc(Numero_Horas, sizeof(double));
	if (*Precio_Compra == NULL || *Precio_Venta == NULL) {
		printf("Error no se ha podido reservar memoria");
		return -1;
	}
	//Inicializo los arrays a cero
	for (int i = 0; i < Numero_Horas; i++) {
		(*Precio_Compra)[i] = 0.0;
		(*Precio_Venta)[i] = 0.0;
	}
	//Pregunto al usuario por los precios de compra del kilovatio-hora de la red:
	for (int i = 0; i < Numero_Horas; i++) {
		printf("Por favor introduzca el precio de compra del kilovatio hora para la hora %d \n", i);
		if (scanf("%lf", &((*Precio_Compra)[i])) != 1) {
			printf("Valor de precio incorrecto \n");
			return -1;
		};
		if ((*Precio_Compra)[i] < 0) {
			printf("Valor de precio incorrecto no puede ser inferior a 0 \n");
			return -1;
		}
		printf("Por favor introduzca el precio de venta del kilovatio hora para la hora %d \n", i);
		if (scanf("%lf", &((*Precio_Venta)[i])) != 1) {
			printf("Valor de precio de venta incorrecto \n");
			return -1;
		}
		if ((*Precio_Venta)[i] < 0) {
			printf("El valor de venta no puede ser negativo \n");
			return -1;
		}
	}
	return 0;
}

static int Leer_Consumo(Operador_Red* Operador, const int Num_Horas) {
	//Leo lo que se ha consumido en la hora actual por cada fase.
	//Fase R
	printf("Introduzca la potencia que se ha consumido hasta el momento en la hora actual en fase R \n");
	if ((scanf("%lf", &Operador->Potencia_Consumida_R) != 1)) {
		return -1;
	}
	//Fase S
	printf("Introduzca la potencia que se ha consumido hasta el momento en la hora actual en fase S \n");
	if ((scanf("%lf", &Operador->Potencia_Consumida_S) != 1)) {
		return -1;
	}
	//Fase T
	printf("Introduzca la potencia que se ha consumido hasta el momento en la hora actual en fase T \n");
	if ((scanf("%lf", &Operador->Potencia_Consumida_T) != 1)) {
		return -1;
	}
	//Leo los datos de lo que el operador de la red desea que se consuma.
	for (int i = 0; i < Operador->Num_Fases; i++) {
		for (int j = 0; j < Num_Horas; j++) {
			printf("Introduzca la referencia de la red para la fase %d en el tramo horario de %d a %d \n", i + 1, j, j + 1);
			if (scanf("%lf", &Operador->Fases[i].Consumo[j]) != 1) {
				printf("Error al leer el consumo \n");
				return -1;
			}
		}
	}
	return 0;
}
int Leer_Datos_Operador(Operador_Red** Operador, const int Num_Horas) {
	//Defino el numero de fases como 3
	const int Num_Fases = 3;
	//Reservo memoria para los datos de la referencia del operador de la red.
	if (Crear_Array_Operador(Operador, Num_Fases, Num_Horas) == -1) {
		return -1;
	}
	//Leo los datos de lo que el operador se desea que se consuma:
	if (Leer_Consumo(*Operador, Num_Horas) == -1) {
		return -1;
	};
	return 0;
}

int Leer_Informacion(Objetivos* Objetivos_Optimizacion, Restricciones_Electrolinera* Restricciones_Potencia,
	Caracteristicas_Simulacion* Tiempo_Algoritmo, Sistema_Carga* Electrolinera,
	Panel_Solar* Datos_Panel_Fotovoltaico, Carga_Adicional* Datos_Carga, c_float* Precio_Compra,
	c_float* Precio_Venta, Operador_Red* Demanda_Operador, Bateria* Baterias_Sistema) {
	
	if (Leer_Tipos_Objetivos(Objetivos_Optimizacion) == -1) {
		return -1;
	};
	//Se Leen las restriciones de los intercambios de potencia entre la electrolinera y la red.
	//if (Obtener_Restricciones_Red_Electrolinera(Restricciones_Potencia) == -1) {
	//	return -1;
	//}
	//if ((!Configurar_Tiempo_Simulacion(Tiempo_Algoritmo)) == -1) {
		//printf("Error al introducir los datos de tiempo \n");
	//	return -1;
	//}
	//ME QUEDO POR AQUI REVISANDO CARGAR DATOS ELECTROLINERA
	//if (Obtener_Informacion_Electrolinera(Electrolinera, *Tiempo_Algoritmo) == -1) {
		//return -1;
	//}

	//Se Leen los datos de potencia solar
	//if (Obtener_Datos_Panel_Fotovoltaico(Datos_Panel_Fotovoltaico, *Tiempo_Algoritmo) == -1) {
		//printf("Error al introducir el panel solar o la carga \n");
		//return -1;
	//}
	//Leo los datos de potencia consumida por la carga
	//if (Obtener_Datos_Cargas(Datos_Carga, *Tiempo_Algoritmo) == -1) {
		//printf("Error al introducir el panel solar o la carga \n");
		//return -1;
	//}
	//Solo se leen los precios si de considera el objetivo de reducir el precio de compra.
	//if (Objetivos_Optimizacion->Minimizar_Coste == 1) {
		//if (Leer_Precios(&Precio_Compra, &Precio_Venta, Tiempo_Algoritmo->Numero_Horas) == -1) {
			//printf("Error al introducir los precios \n");
			//return -1;
		//};
	//}
	//Leo la referencia de lo que el operador de la red se quiere que se consuma por fase:
	//Solo se lee si se considera el objetivo de referencia de la red
	//if (Objetivos_Optimizacion->Obj_Referencia_Red == 1) {
		//if (Leer_Datos_Operador(&Demanda_Operador, Tiempo_Algoritmo->Numero_Horas) == -1) {
			//printf("Error al introducir los datos del operador \n");
			//return -1;
		//}
	//}
	//if ((Leer_Baterias(Baterias_Sistema)) == -1) {
		//printf("Error al introducir la bateria del sistema \n");
		//return -1;
	//}
	return 0;
}