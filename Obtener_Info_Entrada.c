#pragma warning(disable: 4996)
#include <stdio.h>
#include <string.h>
#include <stdbool.h>
#include <stdlib.h>
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
	Datos_CSV*** data = (Datos_CSV***)malloc(MAX_FILAS * sizeof(Datos_CSV**));
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
		data[fila] = (Datos_CSV**)malloc(MAX_COLUMNAS * sizeof(Datos_CSV*));
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
			data[fila][col] = (Datos_CSV*)malloc(sizeof(Datos_CSV));
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

void freeCSV(Datos_CSV *** data, int filas, int columnas) {
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



















