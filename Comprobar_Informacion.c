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
	//Ahora se pasa a comprobar las baterias que se han considerado en el sistema->
	printf("El numero de baterias en el sistema es -> \n");
	printf("%d \n", Informacion_Sistema->Numero_Baterias);
	int Numero_Baterias = Informacion_Sistema->Numero_Baterias;
	for (int i = 0; i < Numero_Baterias; i++) {
		printf("Numero de Bateria %d \n", i);
		printf("Punto de Inicio Bat adicional %d \n",                   Informacion_Sistema->Baterias_Sistema[i].Punto_Inicial_Bateria);
		printf("Punto de Finalizacion Bat adicional %d \n",             Informacion_Sistema->Baterias_Sistema[i].Punto_Final_Bateria);
		printf("Punto Objetivo de la bateria adicional %d \n",          Informacion_Sistema->Baterias_Sistema[i].Punto_Objetivo_Bateria);
		printf("La capacidad de la bateria adicional es %lf \n",        Informacion_Sistema->Baterias_Sistema[i].Capacidad_Bateria);
		printf("El porcentaje de bateria inicial adicional es  %lf \n", Informacion_Sistema->Baterias_Sistema[i].Bateria_Inicial);
		printf("El porcentaje de bateria final adicional   es  %lf \n", Informacion_Sistema->Baterias_Sistema[i].Bateria_Final);
		printf("La maxima potencia aceptada por la bateria es  %lf \n", Informacion_Sistema->Baterias_Sistema[i].Maxima_Potencia);
		printf("La fase a la que esta conectada la bateria es %c \n",   Informacion_Sistema->Baterias_Sistema[i].Fase);
		printf("Se considera la carga del objetivo %d \n",              Informacion_Sistema->Baterias_Sistema[i].Considerar_Carga);
	}
	return 0;
}

void Imprimir_Restriccion_Potencia(const char *Descripcion,const double Valor) {
	printf("%s: %lf \n", Descripcion, Valor);
}

void Comprobar_Restricciones_Sistema(const Restricciones_Electrolinera *Restricciones) {
	printf("Comprobacion de las restricciones del sistema->");
	
	Imprimir_Restriccion_Potencia("Maxima Potencia que puede intercambiar la red y la electrolinera %lf \n", Restricciones->Maxima_Potencia_Red);
	Imprimir_Restriccion_Potencia("Minima Potencia que puede intercambiar la red y la electrolinera %lf \n", Restricciones->Minima_Potencia_Red);
	Imprimir_Restriccion_Potencia("Maxima Potencia que puede llegar a la electrolinera %lf \n", Restricciones->Maxima_Potencia_Red_Entrada);
	Imprimir_Restriccion_Potencia("Minima Potencia que puede llegar a la electrolinera %lf \n", Restricciones->Minima_Potencia_Red_Entrada);
	Imprimir_Restriccion_Potencia("Maxima Potencia que puede salir  de la electrolinera  %lf \n", Restricciones->Maxima_Potencia_Red_Salida);
	Imprimir_Restriccion_Potencia("Minima Potencia que puede salir  de la electrolinera %lf \n", Restricciones->Minima_Potencia_Red_Salida);
	
	Imprimir_Restriccion_Potencia("Maxima Potencia que puede intercambiar la red con la electrolinera fase R  %lf\n", Restricciones->Maxima_Potencia_Red_R);
	Imprimir_Restriccion_Potencia("Minima Potencia que puede intercambiar la red con la electrolinera fase R  %lf\n", Restricciones->Minima_Potencia_Red_R);
	Imprimir_Restriccion_Potencia("Maxima Potencia que puede intercambiar la red con la electrolinera fase S %lf \n", Restricciones->Maxima_Potencia_Red_S);
	Imprimir_Restriccion_Potencia("Minima Potencia que puede intercambiar la red con la electrolinera fase S  %lf\n", Restricciones->Minima_Potencia_Red_S);
	Imprimir_Restriccion_Potencia("Maxima Potencia que puede intercambiar la red con la electrolinera fase T %lf\n", Restricciones->Maxima_Potencia_Red_T);
	Imprimir_Restriccion_Potencia("Minima Potencia que puede intercambiar la red con la electrolinera fase T  %lf\n", Restricciones->Minima_Potencia_Red_T);

	Imprimir_Restriccion_Potencia("Maxima Potencia que puede llegar a la electrolinera fase R %lf \n", Restricciones->Maxima_Potencia_Entrada_Red_R);
	Imprimir_Restriccion_Potencia("Minima Potencia que puede llegar a la electrolinera fase R  %lf\n", Restricciones->Minima_Potencia_Entrada_Red_R);
	Imprimir_Restriccion_Potencia("Maxima Potencia que puede llegar a la electrolinera fase S  %lf\n", Restricciones->Maxima_Potencia_Entrada_Red_S);
	Imprimir_Restriccion_Potencia("Minima Potencia que puede llegar a la electrolinera fase S  %lf\n", Restricciones->Minima_Potencia_Entrada_Red_S);
	Imprimir_Restriccion_Potencia("Maxima Potencia que puede llegar a la electrolinera fase T  %lf\n", Restricciones->Maxima_Potencia_Entrada_Red_T);
	Imprimir_Restriccion_Potencia("Minima Potencia que puede llegar a la electrolinera fase T  %lf\n", Restricciones->Minima_Potencia_Entrada_Red_T);


	Imprimir_Restriccion_Potencia("Maxima Potencia que puede salir de la electrolinera fase R %lf\n", Restricciones->Maxima_Potencia_Salida_Red_R);
	Imprimir_Restriccion_Potencia("Minima Potencia que puede salir de la electrolinera fase R %lf\n", Restricciones->Minima_Potencia_Salida_Red_R);
	Imprimir_Restriccion_Potencia("Maxima Potencia que puede salir de la electrolinera fase S %lf\n", Restricciones->Maxima_Potencia_Salida_Red_S);
	Imprimir_Restriccion_Potencia("Minima Potencia que puede salir de la electrolinera fase S %lf\n", Restricciones->Minima_Potencia_Salida_Red_S);
	Imprimir_Restriccion_Potencia("Maxima Potencia que puede salir de la electrolinera fase T %lf\n", Restricciones->Maxima_Potencia_Salida_Red_T);
	Imprimir_Restriccion_Potencia("Minima Potencia que puede salir de la electrolinera fase T %lf\n", Restricciones->Minima_Potencia_Salida_Red_T);

	Imprimir_Restriccion_Potencia("Maxima Potencia que puede intercambiar el terminal 1 %lf\n", Restricciones->Maxima_Potencia_Terminal_1);
	Imprimir_Restriccion_Potencia("Maxima Potencia que puede intercambiar el terminal 2 %lf\n", Restricciones->Maxima_Potencia_Terminal_2);
	Imprimir_Restriccion_Potencia("Maxima Potencia que puede intercambiar el terminal 3 %lf\n", Restricciones->Maxima_Potencia_Terminal_3);
	Imprimir_Restriccion_Potencia("Maxima Potencia que puede intercambiar el terminal 4 %lf\n", Restricciones->Maxima_Potencia_Terminal_4);
	Imprimir_Restriccion_Potencia("Maxima Potencia que puede intercambiar el terminal 5 %lf\n", Restricciones->Maxima_Potencia_Terminal_5);
	Imprimir_Restriccion_Potencia("Maxima Potencia que puede intercambiar el terminal 6 %lf\n", Restricciones->Maxima_Potencia_Terminal_6);
	Imprimir_Restriccion_Potencia("Maxima Potencia que puede intercambiar el terminal 7 %lf\n", Restricciones->Maxima_Potencia_Terminal_7);
	Imprimir_Restriccion_Potencia("Maxima Potencia que puede intercambiar el terminal 8 %lf\n", Restricciones->Maxima_Potencia_Terminal_8);
	Imprimir_Restriccion_Potencia("Maxima Potencia que puede intercambiar el terminal 9 %lf\n", Restricciones->Maxima_Potencia_Terminal_9);
	Imprimir_Restriccion_Potencia("Maxima Potencia que puede intercambiar el terminal 10 %lf\n", Restricciones->Maxima_Potencia_Terminal_10);
	Imprimir_Restriccion_Potencia("Maxima Potencia que puede intercambiar el terminal 11 %lf\n", Restricciones->Maxima_Potencia_Terminal_11);
	Imprimir_Restriccion_Potencia("Maxima Potencia que puede intercambiar el terminal 12 %lf\n", Restricciones->Maxima_Potencia_Terminal_12);


	Imprimir_Restriccion_Potencia("Minima Potencia que puede intercambiar el terminal 1 %lf\n", Restricciones->Minima_Potencia_Terminal_1);
	Imprimir_Restriccion_Potencia("Minima Potencia que puede intercambiar el terminal 2 %lf\n", Restricciones->Minima_Potencia_Terminal_2);
	Imprimir_Restriccion_Potencia("Minima Potencia que puede intercambiar el terminal 3 %lf\n", Restricciones->Minima_Potencia_Terminal_3);
	Imprimir_Restriccion_Potencia("Minima Potencia que puede intercambiar el terminal 4 %lf\n", Restricciones->Minima_Potencia_Terminal_4);
	Imprimir_Restriccion_Potencia("Minima Potencia que puede intercambiar el terminal 5 %lf\n", Restricciones->Minima_Potencia_Terminal_5);
	Imprimir_Restriccion_Potencia("Minima Potencia que puede intercambiar el terminal 6 %lf\n", Restricciones->Minima_Potencia_Terminal_6);
	Imprimir_Restriccion_Potencia("Minima Potencia que puede intercambiar el terminal 7 %lf\n", Restricciones->Minima_Potencia_Terminal_7);
	Imprimir_Restriccion_Potencia("Minima Potencia que puede intercambiar el terminal 8 %lf\n", Restricciones->Minima_Potencia_Terminal_8);
	Imprimir_Restriccion_Potencia("Minima Potencia que puede intercambiar el terminal 9 %lf\n", Restricciones->Minima_Potencia_Terminal_9);
	Imprimir_Restriccion_Potencia("Minima Potencia que puede intercambiar el terminal 10 %lf\n",Restricciones->Minima_Potencia_Terminal_10);
	Imprimir_Restriccion_Potencia("Minima Potencia que puede intercambiar el terminal 11 %lf\n",Restricciones->Minima_Potencia_Terminal_11);
	Imprimir_Restriccion_Potencia("Minima Potencia que puede intercambiar el terminal 12 %lf\n",Restricciones->Minima_Potencia_Terminal_12);
}

void Comprobar_Precios(const Elementos_Electrolinera* Informacion_Sistema, Puntos_Optimizacion *Array_Puntos_Simulacion) {
	for (int i = 0; i < Informacion_Sistema->Numero_Puntos; i++) {
		printf("El punto es %d \n", Array_Puntos_Simulacion[i].Punto_Simulacion);
		printf("La hora es %d \n", Array_Puntos_Simulacion[i].Fecha.tm_hour);
		printf("El precio de compra es %f \n", Array_Puntos_Simulacion[i].Precio_Compra);
		printf("El precio de venta es %f \n", Array_Puntos_Simulacion[i].Precio_Venta);
	}
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