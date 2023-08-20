#include <stdbool.h>
#include "osqp.h"
//#include "Comprobar_Informacion.h"
#include <stdio.h>
#include "Tipos_Optimizacion.h"
void Escribir_CSV(const char* Nombre_Archivo, Celda*** data, int Filas, int Columnas) {
	FILE* file = fopen(Nombre_Archivo, "w");
	if (file == NULL) {
		printf("Error Abriendo el archivo \n");
		return;
	}
	//Escribo los encabezados
	//fprintf(file, "Numero de Terminal, Tipo Dispositivo, Numero Vehiculo, Modo Carga, Capacidad Batería %,"\
		"% Bateria Inicial, Potencia carga maxima, Fase, Dia Inicial, Hora Inicial, Minuto Inicial,"\
		"Dia Final, Hora Final, Minuto Final\n");
	//Se itera por cada fila 
	for (int i = 0; i < Filas; i++) {
		//Se itera por cada columna->
		for (int j = 0; j < Columnas; j++) {
			//Se revisa que tipo de dato es y se escribe en un archivo->
			if (data[i][j]->Tipo == STRING) {
				fprintf(file, "%s,", data[i][j]->data.str);
			}
			else if (data[i][j]->Tipo == DOUBLE1) {
				fprintf(file, "%.2f,", data[i][j]->data.dbl);
			}
		}
		fprintf(file, "\n");
	}
	fclose(file);
}
void Escribir_Array_Puntos_Simulacion(Puntos_Optimizacion * Array_Puntos_Simulacion, const int*Numero_Puntos) {
	//Este subprograma se utiliza para comprobar que los datos del array son correctos
	FILE* file = fopen("Comprobar_Array_Puntos.csv", "w");
		if (file == NULL) {
			printf("Error Abriendo el archivo \n");
			return;
		}
		for (int i = 0; i < *Numero_Puntos; i++) {
			fprintf(file, "%d,", Array_Puntos_Simulacion[i].Punto_Simulacion);
			fprintf(file, "%d,", Array_Puntos_Simulacion[i].Delta_Simulacion);
			fprintf(file, "%d,", Array_Puntos_Simulacion[i].Fecha.tm_year);
			fprintf(file, "%d,", Array_Puntos_Simulacion[i].Fecha.tm_mon);
			fprintf(file, "%d,", Array_Puntos_Simulacion[i].Fecha.tm_mday);
			fprintf(file, "%d,", Array_Puntos_Simulacion[i].Fecha.tm_hour);
			fprintf(file, "%d,", Array_Puntos_Simulacion[i].Fecha.tm_min);
			fprintf(file, "%d,", Array_Puntos_Simulacion[i].Fecha.tm_sec);
			fprintf(file, "\n");
		}
		fclose(file);
}

void Escribir_Puntos_Adicionales(Puntos_Adicionales* Array_Puntos_Simulacion, const int* Numero_Puntos) {
	//Este subprograma se utiliza para comprobar que los datos del array son correctos
	FILE* file = fopen("Comprobar_Array_Puntos.csv", "w");
	if (file == NULL) {
		printf("Error Abriendo el archivo \n");
		return;
	}
	for (int i = 0; i < *Numero_Puntos; i++) {
		fprintf(file, "%d,", Array_Puntos_Simulacion[i].date.tm_year);
		fprintf(file, "%d,", Array_Puntos_Simulacion[i].date.tm_mon);
		fprintf(file, "%d,", Array_Puntos_Simulacion[i].date.tm_mday);
		fprintf(file, "%d,", Array_Puntos_Simulacion[i].date.tm_hour);
		fprintf(file, "%d,", Array_Puntos_Simulacion[i].date.tm_min);
		fprintf(file, "%d,", Array_Puntos_Simulacion[i].date.tm_sec);
		fprintf(file, "\n");
	}
	fclose(file);
}

void Comprobar_Elementos_Electrolinera(Elementos_Electrolinera* Informacion_Sistema) {
	//Utilizo este subprograma para comprobar que la informacion de los elementos conectados a la electrolinera
	//se ha cargado correctamente.
	printf("El numero de vehiculos en el sistema es -> \n");
	printf("%d \n", Informacion_Sistema->Numero_Vehiculos);
	int Numero_Vehiculos = Informacion_Sistema->Numero_Vehiculos;
	printf("El numero de puntos de simulacion es -> \n");
	printf("%d \n", Informacion_Sistema->Numero_Puntos);
	for (int i = 0; i < Numero_Vehiculos; i++) {
		printf("Numero de Vehiculo %d\n", i);
		printf("Punto de Inicio %d\n", Informacion_Sistema->Vehiculos_Sistema[i].Punto_Inicial_Carga);
		printf("Punto de Finalizacion %d \n", Informacion_Sistema->Vehiculos_Sistema[i].Punto_Final_Carga);
		printf("La capacidad de la bateria es  %lf \n", Informacion_Sistema->Vehiculos_Sistema[i].Capacidad_Bateria);
		printf("El porcentaje de bateria inicial es  %lf \n", Informacion_Sistema->Vehiculos_Sistema[i].Bateria_Inicial);
		printf("El porcentaje de bateria final   es  %lf \n", Informacion_Sistema->Vehiculos_Sistema[i].Bateria_Final);
		printf("La maxima potencia aceptada por el coche es  %lf \n", Informacion_Sistema->Vehiculos_Sistema[i].Maxima_Potencia);
		printf("La fase a la que esta conectada es %c \n", Informacion_Sistema->Vehiculos_Sistema[i].Fase);
		printf("El modo de carga del vehiculo es %d \n", Informacion_Sistema->Vehiculos_Sistema[i].Modo_Carga);
	}
	return 0;
}
/*
void Comprobar_Array_Puntos_Adicionales(Puntos_Adicionales * Array_Puntos_Adicionales) {
	FILE* file = fopen("Comprobar_Adicional.csv", "w");
	if (file == NULL) {
		printf("Error Abriendo el archivo \n");
		return;
	}
	for (int i = 0; i < 5; i++) {
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
*/
/*
void Escribir_CSV_Vehiculos(const char *Nombre_Archivo,Celda ***data,int Filas, int* Columnas ) {
	FILE* file = fopen(Nombre_Archivo, "w");
	if (file == NULL) {
		printf("Error Abriendo el archivo \n");
		return;
	}
	//Escribo los encabezados
	//fprintf(file, "Numero de Terminal, Tipo Dispositivo, Numero Vehiculo, Modo Carga, Capacidad Batería %,"\
		"% Bateria Inicial, Potencia carga maxima, Fase, Dia Inicial, Hora Inicial, Minuto Inicial,"\
		"Dia Final, Hora Final, Minuto Final\n");
	//Se itera por cada fila 
	for (int i = 0; i <Filas ; i++) {
		//Se itera por cada columna->
		for (int j = 0; j < Columnas[i]; j++) {
			//Se revisa que tipo de dato es y se escribe en un archivo->
			if (data[i][j]->Tipo == STRING) {
				fprintf(file, "%s,", data[i][j]->data.str);
			}
			else if (data[i][j]->Tipo == DOUBLE1) {
				fprintf(file, "%.2f,", data[i][j]->data.dbl);
			}
		}
		fprintf(file, "\n");
	}
	fclose(file);
}
void Escribir_CSV_Terminal(const char *Nombre_Archivo,Celda ***data,int Filas,int *Columnas) {
	FILE* file = fopen(Nombre_Archivo, "w");
	if (file == NULL) {
		printf("Error Abriendo el archivo \n");
		return;
	}
	//Escribo los encabezados
	//fprintf(file, "Numero de Terminal, Dia Final, Hora Final, Minuto Final\n");
	//Se itera por cada fila 
	for (int i = 0; i < Filas; i++) {
		//Se itera por cada columna->
		for (int j = 0; j < Columnas[i]; j++) {
			//Se revisa que tipo de dato es y se escribe en un archivo->
			if (data[i][j]->Tipo == STRING) {
				fprintf(file, "%s,", data[i][j]->data.str);
			}
			else if (data[i][j]->Tipo == DOUBLE1) {
				fprintf(file, "%.2f,", data[i][j]->data.dbl);
			}
		}
		fprintf(file, "\n");
	}
	fclose(file);
}
void Escribir_CSV_Tiempo_Simulacion(const char* Nombre_Archivo, Celda*** data, int Filas, int* Columnas) {
	FILE* file = fopen(Nombre_Archivo, "w");
	if (file == NULL) {
		printf("Error Abriendo el archivo \n");
		return;
	}
	//Escribo los encabezados
	//fprintf(file, "Mes Inicio,Dia Inicio,Hora Inicio,Minuto Inicio,Mes Final,Dia Final,Hora Final,Minuto Final,Delta Simulacion \n");
	//Se itera por cada fila 
	for (int i = 0; i < Filas; i++) {
		//Se itera por cada columna->
		for (int j = 0; j < Columnas[i]; j++) {
			//Se revisa que tipo de dato es y se escribe en un archivo->
			if (data[i][j]->Tipo == STRING) {
				fprintf(file, "%s,", data[i][j]->data.str);
			}
			else if (data[i][j]->Tipo == DOUBLE1) {
				fprintf(file, "%.2f,", data[i][j]->data.dbl);
			}
		}
		fprintf(file, "\n");
	}
	fclose(file);
}
*/