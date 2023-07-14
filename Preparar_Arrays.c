#include "Tipos_Optimizacion.h"
#include <stdlib.h>
#include <stdio.h>
//En este archivo hay funciones relacionadas con la creacion de los arrays, y programas relacionados con
//calcular el indice correcto para posicionarlo en el array
void Liberar_Memoria_Panel_Solar(Panel_Solar* Panel) {
	if (Panel->Panel_R != NULL) {
		free(Panel->Panel_R);
		Panel->Panel_R = NULL;
	}

	if (Panel->Panel_S != NULL) {
		free(Panel->Panel_S);
		Panel->Panel_S = NULL;
	}

	if (Panel->Panel_T != NULL) {
		free(Panel->Panel_T);
		Panel->Panel_T = NULL;
	}
}

void Liberar_Memoria_Carga(Carga_Adicional* Carga) {
	if (Carga->Carga_R != NULL) {
		free(Carga->Carga_R);
		Carga->Carga_R = NULL;
	}

	if (Carga->Carga_S != NULL) {
		free(Carga->Carga_S);
		Carga->Carga_S = NULL;
	}

	if (Carga->Carga_T != NULL) {
		free(Carga->Carga_T);
		Carga->Carga_T = NULL;
	}
}

void Liberar_Memoria_Referencia_Operador(Operador_Red* Operador) {
	if (Operador->Fases != NULL) {
		for (int i = 0; i < Operador->Num_Fases; ++i) {
			if (Operador->Fases[i].Consumo != NULL) {
				free(Operador->Fases[i].Consumo);
				Operador->Fases[i].Consumo = NULL;
			}
		}
		free(Operador->Fases);
		Operador->Fases = NULL;
	}
}
void Liberar_Memoria_Tiempo_Simulacion(Caracteristicas_Simulacion* Tiempo_Simulacion) {
	//Se Libera memoria del tiempo de simulacion.
	if (Tiempo_Simulacion->Puntos_Tiempo_Real != NULL) {
		free(Tiempo_Simulacion->Puntos_Tiempo_Real);
		Tiempo_Simulacion->Puntos_Tiempo_Real = NULL;
	}
}

void Liberar_Memoria_Estaciones(Sistema_Carga* Sistema) {
	for (int i = 0; i < Sistema->Numero_Estaciones; ++i) {
		if (Sistema->Estaciones_Carga[i].Vehiculos != NULL) {
			free(Sistema->Estaciones_Carga[i].Vehiculos);
			Sistema->Estaciones_Carga[i].Vehiculos = NULL;
		}
	}

	if (Sistema->Estaciones_Carga != NULL) {
		free(Sistema->Estaciones_Carga);
		Sistema->Estaciones_Carga = NULL;
	}
}
 void Calcular_Hora_Actual(const Caracteristicas_Simulacion* Tiempo_Simulacion, int* Hora_Actual,
	int* Index_Hora, const int Punto_Simulacion) {
	if (Hora_Actual != Tiempo_Simulacion->Puntos_Tiempo_Real[Punto_Simulacion].Hora) {
		Hora_Actual += 1;
		Index_Hora += 1;
	}
}
void Liberar_Toda_Memoria(Panel_Solar* Panel, Carga_Adicional* Carga, Operador_Red* Operador,
	Caracteristicas_Simulacion* Tiempo_Simulacion, Sistema_Carga* Sistema) {
	Liberar_Memoria_Estaciones(Sistema);
	Liberar_Memoria_Tiempo_Simulacion(Tiempo_Simulacion);
	Liberar_Memoria_Referencia_Operador(Operador);
	Liberar_Memoria_Carga(Carga);
	Liberar_Memoria_Panel_Solar(Panel);
}
int Crear_Array_Operador(Operador_Red** Referencia_Operador, const int Num_Fases, const int Num_Horas) {
	//Reservo memoria para la estructura de los datos del operador de red
	//*Referencia_Operador = (Operador_Red*)calloc(sizeof(Operador_Red));
	//Si la reserva de memoria ha tenido exito sigo reservando memoria para los diferentes campos del tipo
	if (*Referencia_Operador == NULL) {                                                   //Devuelve un -1 si la reserva de memoria fracasa
		printf("Reserva de memoria fallida para los datos del operador\n");
		return -1;
	}

	//Reservo memoria para el campo fases
	(*Referencia_Operador)->Fases = (Fase*)calloc(Num_Fases, sizeof(Fase));
	if ((*Referencia_Operador)->Fases == NULL) {
		printf("La Reserva de memoria fallida para las fases de la red.\n");               //La reserva de memoria ha sido fallida
		return -1;
	}

	(*Referencia_Operador)->Num_Fases = Num_Fases; //Se rellena el campo con el numero de fases.
	//Reservo memoria para los campos fases dentro del struct.
	for (int i = 0; i < Num_Fases; i++) {
		(*Referencia_Operador)->Fases[i].Consumo = (double*)calloc(Num_Horas, sizeof(double)); //Se reserva el campo con la memoria.
		(*Referencia_Operador)->Fases[i].Num_Horas = Num_Horas;                                 //Se rellena el campo con el numero de horas
		if ((*Referencia_Operador)->Fases[i].Consumo == NULL) {                                 //Se comprueba que la reserva de mamoria se haya 
			printf("Reserva de memoria fallida \n");
			return -1;
		}
	}
	return 0;
}