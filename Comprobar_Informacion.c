
#include "Comprobar_Informacion.h"
#include <stdio.h>
#include "Tipos_Optimizacion.h"
void Escribir_CSV_Vehiculos(const char* Nombre_Archivo, Celda*** data, int Filas, int* Columnas) {
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