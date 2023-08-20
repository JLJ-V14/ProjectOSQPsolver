#pragma once

#ifndef TIPOS_OPTIMIZACION_H
#define TIPOS_OPTIMIZACION_H
#include <stdbool.h>
#include "Tipos_Optimizacion.h"
#include <time.h>
#include "osqp.h"
#ifdef __cplusplus
extern "C" {
#endif

	typedef enum {
		CARGA_RAPIDA = 0,       //0 significa carga rapida
	    CARGA_NORMAL = 1        //1 significa carga normal
	}Tipo_Carga;

	typedef struct {
		int           Punto_Inicial_Carga;  //Punto inicial de la carga del vehiculo->
		int           Punto_Final_Carga;    //Punto Final de la carga del vehiculo  ->
		double        Capacidad_Bateria;   //Capacidad de la bateria del vehiculo  ->
		char          Fase;                //Fase a la que esta conectada el vehiculo ->
		double        Bateria_Inicial;    //Porcentaje de la bateria inicial a la que esta conectada el vehiculo.
		double        Bateria_Final;      //Porcentaje de bateria final que se desea.
		double        Maxima_Potencia;    //Potencia maxima que acepta el vehiculo
		Tipo_Carga    Modo_Carga;         //Variable que almacena el modo de carga del vehiculo 0 rapida 1 lenta.
	}Vehiculos;

	typedef struct {
		int Numero_Puntos;
		int Numero_Vehiculos;
		Vehiculos* Vehiculos_Sistema;
	}Elementos_Electrolinera;
	
	//Tipo propio para almacenar la informacion importante de cada vehiculo.
	
	
	
	
	//Creo un tipo para almacenar, puntos de simulacion que deben estar ya que coinciden con la fecha de inicio
	// de carga o de finalizacion de carga de vehiculos, o bien con una hora en punto.
	typedef struct {
		struct tm date;
	}Puntos_Adicionales;
	//Creo un tipo para asociar cada punto de la simulacion a una fecha.
	typedef struct {
		int Punto_Simulacion;
		int Delta_Simulacion;
		struct tm Fecha;
	}Puntos_Optimizacion;

	//Se crea una variable para poder almacenar los datos del CSV de
	typedef enum {STRING,DOUBLE1}TipoDato;
	//Se define este tipo para poder almacenar la informacion que se lee del CSV donde esta contenida la informacion
	//de los vehículos que tienen su carga programada. Esta informacion puede ser de tipo double.
	typedef struct {
		TipoDato Tipo;
			union {
			char* str;
			c_float dbl;
		}data;
	}Celda;

	//-------------------------------------------------------------------------------------------------------//
	typedef struct {
		double	Maxima_Potencia_Red_Entrada;            //Maxima potencia que puede llegar de la red a la electrolinera
		double  Minima_Potencia_Red_Entrada;            //Minima potencia que puede llegar de la red a la electrolinera
		double  Maxima_Potencia_Red_Salida;             //Maxima potencia que puede salir de la electrolinera a la red.
		double  Minima_Potencia_Red_Salida;             //Minima potencia que puede salir de la electrolinera a la red.
		double  Maxima_Potencia_Red;                    //Maxima potencia que puede intercambiar la electrolinera con la red
		double  Minima_Potencia_Red;                    //Minima potencia que puede intercambiar la electrolinera con la red
		double  Maximo_Potencia_Red_R;                  //Maxima Potencia que puede intercambiar la electrolinera con la red en fase R
		double  Maximo_Potencia_Red_S;                  //Maxima Potencia que puede intercambiar la electrolinera con la red en fase S
		double  Maximo_Potencia_Red_T;                  //Maxima Potencia que puede intercambiar la electrolinera con la red en fase T
		double  Minima_Potencia_Red_R;                  //Minima Potencia que puede intercambiar la electrolinera con la red en fase R
		double  Minima_Potencia_Red_S;                  //Minima Potencia que puede intercambiar la electrolinera con la red en fase S
		double  Minima_Potencia_Red_T;                  //Minima Potencia que puede intercambiar la electrolinera con la red en fase T
		double  Maxima_Potencia_Entrada_Red_R;          //Maxima Potencia que puede llegar de la red a la electrolinera en fase R
		double  Minima_Potencia_Entrada_Red_R;          //Minima Potencia que puede llegar de la red a la electrolinera en fase R
		double  Maxima_Potencia_Entrada_Red_S;          //Maxima Potencia que puede llegar de la red a la electrolinera en fase S
		double  Minima_Potencia_Entrada_Red_S;          //Minima Potencia que puede llegar de la red a la electrolinera en fase S
		double  Maxima_Potencia_Entrada_Red_T;          //Minima Potencia que puede llegar de la red a la electrolinera en fase T
		double  Minima_Potencia_Entrada_Red_T;          //Minima Potencia que puede llegar de la red a la electrolinera en fase T
		double  Maxima_Potencia_Salida_Red_R;           //Maxima Potencia que puede salir  de la electrolinera a la red en fase R
		double  Minima_Potencia_Salida_Red_R;           //Minima Potencia que puede salir  de la electrolinera a la red en fase R
		double  Maxima_Potencia_Salida_Red_S;           //Maxima Potencia que puede salir  de la electrolinera a la red en fase S
		double  Minima_Potencia_Salida_Red_S;           //Minima Potencia que puede salir  de la electrolinera a la red en fase S
		double  Maxima_Potencia_Salida_Red_T;           //Maxima Potencia que puede salir  de la electrolinera a la red en fase T
		double  Minima_Potencia_Salida_Red_T;           //Minima Potencia que puede salir  de la electrolinera a la red en fase T
	}Restricciones_Electrolinera;                   //Variable que sirve para almacenar los limitaciones de la red con la electrolienra.

	typedef struct {
		double* Panel_R;                                 //Valores que dan los paneles fotovoltaicos conectados a fase R
		double* Panel_S;                                 //Valores que dan los paneles fotovoltaicos conectados a fase S
		double* Panel_T;                                 //Valores que dan los paneles fotovoltaicos conectados a fase T
		bool Existe_Panel_R;                            //Variable integer que informa si hay Panel fotovoltaico conectado a fase R
		bool Existe_Panel_S;                            //Variable integer que informa si hay Panel fotovoltaico conectado a fase S
		bool Existe_Panel_T;                            //Variable integer que informa si hay Panel fotovoltaico conectado a fase T
	}Panel_Solar;

	typedef struct {
		double* Carga_R;                                 //Valores consumidos por las cargas adicionales en fase R
		double* Carga_S;                                 //Valores consumidos por las cargas adicionales en fase S
		double* Carga_T;                                 //Valores consumidos por las cargas adicionales en fase T
		bool   Existe_Carga_R;                         //Variable booleana que informa si hay carga adicional conectada a fase R
		bool   Existe_Carga_S;                          //Variable booleana que informa si hay carga adicional conectada a fase S
		bool   Existe_Carga_T;                          //Variable booleana que informa si hay carga adicional conectada a fase T
	}Carga_Adicional;

	typedef enum {
		Estandar,
		Prisa
	}Tipo_Tarifa;
	
	typedef struct {
		double          Capacidad_Bateria;                  //Capacidad de la bateria del vehiculo 
		double          Estado_Inicial_Bateria;             //Estado inicial de la bateria del vehiculo
		double          Estado_Deseado_Bateria;             //Objetivo de carga
		Tipo_Tarifa     Modo_Carga;                         //Variable que indica el tipo de contrato del vehiculo
		double          Limite_Bateria;                     //Variable que indica el limite de la bateria
		double          Limite_Potencia_Carga;              //Limite de potencia que es capaz de aceptar el vehiculo
		int             Dia_Inicial_Carga;                  //Dia_Inicial de la carga
		int             Dia_Final_Carga;                    //Dia_Final de la carga
		int             Hora_Inicial_Carga;                 //Hora Inicial de la carga
		int             Minuto_Inicial_Carga;               //Minuto Inicial de la carga
		int             Hora_Final_Carga;                   //Hora Final de la carga
		int             Minuto_Final_Carga;                 //Minuto Final de la carga
		int             Punto_Inicio_Carga;                 //Variable que indica el punto de inicio de la carga
		int             Punto_Final_Carga;                  //Variable que indica el punto de finalizacion de la carga.	
	} Vehiculo;
	
	typedef struct {
		Vehiculo    *Vehiculos;                          //Array de tipo vehiculos, cada vehiculo contiene la informacion del mismo 
		int         Num_Vehiculos;                      //Numero de vehiculos que tienen la carga programada en el terminal
		char        Fase_Red;                           //Fase a la que esta conectada la estacion de carga.
	}Estacion;                                          //Tipo que define a la estacion de carga 

	typedef struct {
		int        Numero_Estaciones;                   //El Numero de estaciones que se tienen en el sistema de carga
		Estacion*  Estaciones_Carga;
	}Sistema_Carga;

	typedef struct {
		double* Consumo;                                //Array dinamico para almacenar los valores que el operador de la red desean que se consuman
		int     Num_Horas;                              //Para almacenar el numero de horas.
	}Fase;                                              //Valor de fase 


	typedef struct {
		Fase*  Fases;                                   //Array dinamico para almacenar los datos de las fases
		int    Num_Fases;                               //Almacena el numero de fases.
		double Potencia_Consumida_R;                    //Se almacena lo que se ha consumido en la electrolinera hasta el momento  en la hora actual.
		double Potencia_Consumida_S;                    //Se almacena lo que se ha conusmido en la electrolinera hasta el momento  en la hora actual.
		double Potencia_Consumida_T;                    //Se almacena lo que se ha consumido en la electrolinera hasta el momento  en la hora actual
	}Operador_Red;                                      //Tipo que almacena lo que el operador de la red desea que se consuma por horas:

	typedef struct {                                    //Este es un tipo que sirve para identificar cada punto de la simulacion con un punto en el tiempo          
		int  Dia;
		int  Hora;
		int  Minuto;
		int  Punto;
	}Puntos_Tiempo;
	typedef struct {
		int Dia_Inicio;                              //Dia Inicial de la simulacion
		int Dia_Final;                               //Dia Final de la simulacion
		int Hora_Inicio;                             //Hora inicial de ejecucion de la simulacion
		int Minuto_Inicial;                          //Minuto inicial de ejecucion de la simulacion
		int Hora_Final;                              //Hora final   de ejecucion de la simulacion
		int Minuto_Final;                            //Minuto final de ejecucion de la simulacion
		int Resolucion_Minutos;                      //Resolucion en minutos de la simulacion.
		int Numero_Horas;                            //Numero de Horas que se van a calcular en la simulacion.
		int Puntos_Simulacion;                       //Numero de Puntos de simulacion.
		int Delta_Primer_Punto;                      //Delta de tiempo del primer punto del algoritmo al segundo
		int Delta_Ultimo_Punto;                      //Delta de tiempo del penultimo punto del algoritmo al ultimo.
		Puntos_Tiempo* Puntos_Tiempo_Real;            //Struct que identifica cada punto de simulacion con un tiempo real
	}Caracteristicas_Simulacion;

	typedef struct {                                      //Enumeracion que contiene los objetivos a cumplir
		int  Minimizar_Coste;                             //Objetivo de Minimizar coste
		int  Cargar_Vehiculo;                             //Objetivo de cargar vehiculo
		int  Obj_Vida_Bateria;                            //Objetivo de perservar la vida de la bateria
		int  Obj_Referencia_Red;                          //Objetivo de la referencia de la red
		int  Obj_Cargar_Bateria;                          //Objetivo de la bateria del sistema
	}Objetivos;

	typedef struct {
		bool   Bateria_R;                           //Variable booleana que indica si hay bateria conectada a fase R
		bool   Bateria_S;                          //Variable booleana que indica si hay bateria conectada a fase S
		bool   Bateria_T;                          //Variable booleana que indica si hay bateria conectada a fase T
		double Capacidad_Bateria_R;                //Variable que indica la capacidad de la bateria en kWh  conectada a fase R
		double Capacidad_Bateria_S;                //Variable que indica la capacidad de la bateria en kWh  conectada a fase S
		double Capacidad_Bateria_T;                //Variable que indica la capacidad de la bateria en KWh  conectada a fase T
		double Bateria_Inicial_R;                  //Variable que indica el porcentaje de la bateria inicial conectada a fase R
		double Bateria_Inicial_S;                  //Variable que indica el porcentaje de la bateria inicial conectada a fase S
		double Bateria_Inicial_T;                  //Variable que indica el porcentaje de la bateria inicial conectada a fase T
	}Bateria;

	
#ifdef __cplusplus
}
#endif
#endif