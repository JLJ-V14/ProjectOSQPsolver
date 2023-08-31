
// En este archivo se define los tipos de variables propias utilizadas para el codigo de optimizacion-----//
#ifndef TIPOS_OPTIMIZACION_H
#define TIPOS_OPTIMIZACION_H
#include "Definiciones_Globales.h"
#include <stdbool.h>
#include <time.h>
#include "osqp.h"
#ifdef __cplusplus
extern "C" {
#endif


	//-------------------Variable que sirve para indicar el modo de carga del vehiculo-------------------///
	typedef enum {
		CARGA_RAPIDA = 0,
		CARGA_NORMAL = 1
	}Tipo_Carga;


	//-------------Tipo de variable que sirve para almacenar la informacion correspondiente del vehiculo------//
	typedef struct {
		int           Punto_Inicial_Carga;  //Punto inicial de la carga del vehiculo->
		int           Punto_Final_Carga;    //Punto Final de la carga del vehiculo  ->
		double        Capacidad_Bateria;    //Capacidad de la bateria del vehiculo  ->
		char          Fase;                 //Fase a la que esta conectada el vehiculo ->
		double        Bateria_Inicial;      //Porcentaje de la bateria inicial a la que esta conectada el vehiculo.
		double        Bateria_Final;        //Porcentaje de bateria final que se desea.
		double        Maxima_Potencia;      //Potencia maxima que acepta el vehiculo
		Tipo_Carga    Modo_Carga;           //Variable que almacena el modo de carga del vehiculo 0 rapida 1 lenta.
	}Vehiculos;

	//-------------Tipo de variable que sirve para almacenar la informacion correspondiente de la bateria------//
	typedef struct {
		int    Punto_Inicial_Bateria;    //Punto inicial de la presencia de la bateria en el sistema
		int    Punto_Final_Bateria;      //Punto final de la presencia de la bateria en el sistema
		int    Punto_Objetivo_Bateria;   //Punto objetivo de la carga de la bateria en el sistema
		double Capacidad_Bateria;        //Capacidad de la Bateria del vehiculo
		double Bateria_Inicial;          //Porcentaje de bateria inicial 
		double Bateria_Final;            //Porcentaje de bateria final 
		double Maxima_Potencia;          //Maxima Potencia que puede intercambiar la bateria
		bool   Considerar_Carga;         //Variable booleana que sirve para identificar si se carga la bateria.
	}Baterias;


	//--Tipo de variable que sirve para almacenar la informacion importante del terminal.
	typedef struct {
		int        Numero_Terminal;        //Numero de terminal en cuestion.
		int        Numero_Baterias;        //Numero de baterias de vehiculos o baterias simplemente conectadas al terminal
		int        Numero_Vehiculos;       //Numero de vehiculos que tienen su carga programada en el terminal
		char       Fase;                   //Fase a la que esta conectada el terminal
		Vehiculos* Vehiculos_Terminal;     //Vehiculos que tienen su carga programada en el terminal
		Baterias * Baterias_Terminal;      //Baterias  que tienen su carga programada en el terminal
	}Informacion_Terminal;


	


	//-------Variable que sirve para almacenar la informacion de todos los elementos que tiene la electrolinera---//
	typedef struct {
		int Numero_Puntos;                                   //Numero de puntos de simulacion que se han considerado en el sistema.
		int Numero_Vehiculos;                                //Numero de Vehiculos que tiene conectado el sistema
		int Numero_Baterias;                                 //Numero de Baterias que tiene conectado el sistema
		Informacion_Terminal Terminales[NUMERO_TERMINALES];  //Informacion de los elementos conectados en cada terminal;
	}Elementos_Electrolinera;

	//---Se crea una variable que es un array de las fechas adicionales que deben considerarse---------------//
	//al coincidir con la carga de un vehiculo, o con una hora en punto, o con la partida de un vehiculos-//

	typedef struct {
		struct tm date;
	}Puntos_Adicionales;

	
	//-----------------------Se crea una variable para relacionar los puntos de simulacion con una fecha -------///
	typedef struct {
		int       Punto_Simulacion;        //Numero de punto de calculo del algoritmo
		int       Delta_Simulacion;        //Diferencia en minutos entre un punto del algoritmo y el anterior
		struct tm Fecha;                   //Fecha Asociada al punto de la simulacion
		double    Precio_Compra;           //Precio de compra del kW/h asociado al punto.
		double    Precio_Venta;            //Precio de venta  del kW/h asociado al punto.
	}Puntos_Optimizacion;



	//--------------Variable con el proposito de alamacenar la informacion proveniente del CSV---------------//
	typedef enum {STRING,DOUBLE1}TipoDato;
	typedef struct {
		    TipoDato Tipo;
			union {
			char* str;      //Si el dato es de tipo string
			c_float dbl;    //Si el dato es numerico
		}data;
	}Datos_CSV;
 
	//----------------------------------------Tipo que define las restricciones del sistema------------------//
	//-------------------------------------------------------------------------------------------------------//
	typedef struct {
		double  Maxima_Potencia_Red;                          //Maxima potencia que puede intercambiar la electrolinera con la red
		double  Minima_Potencia_Red;                          //Minima potencia que puede intercambiar la electrolinera con la red
		double	Maxima_Potencia_Red_Entrada;                  //Maxima potencia que puede llegar de la red a la electrolinera
		double  Minima_Potencia_Red_Entrada;                  //Minima potencia que puede llegar de la red a la electrolinera
		double  Maxima_Potencia_Red_Salida;                   //Maxima potencia que puede salir de la electrolinera a la red.
		double  Minima_Potencia_Red_Salida;                   //Minima potencia que puede salir de la electrolinera a la red.
		double  Maxima_Potencia_Red_R;                        //Maxima Potencia que puede intercambiar la electrolinera y red en fase R
		double  Maxima_Potencia_Red_S;                        //Maxima Potencia que puede intercambiar la electrolinera y red en fase S
		double  Maxima_Potencia_Red_T;                        //Maxima Potencia que puede intercambiar la electrolinera y red en fase T
		double  Minima_Potencia_Red_R;                        //Minima Potencia que puede intercambiar la electrolinera y red en fase R
		double  Minima_Potencia_Red_S;                        //Minima Potencia que puede intercambiar la electrolinera y red en fase S
		double  Minima_Potencia_Red_T;                        //Minima Potencia que puede intercambiar la electrolinera y red en fase T
		double  Maxima_Potencia_Entrada_Red_R;                //Maxima Potencia que puede llegar de la red en fase R
		double  Minima_Potencia_Entrada_Red_R;                //Minima Potencia que puede llegar de la red en fase R
		double  Maxima_Potencia_Entrada_Red_S;                //Maxima Potencia que puede llegar de la red en fase S
		double  Minima_Potencia_Entrada_Red_S;                //Minima Potencia que puede llegar de la red en fase S
		double  Maxima_Potencia_Entrada_Red_T;                //Minima Potencia que puede llegar de la red en fase T
		double  Minima_Potencia_Entrada_Red_T;                //Minima Potencia que puede llegar de la red  en fase T
		double  Maxima_Potencia_Salida_Red_R;                 //Maxima Potencia que puede salir  a  la red  en fase R
		double  Minima_Potencia_Salida_Red_R;                 //Minima Potencia que puede salir  a  la red  en fase R
		double  Maxima_Potencia_Salida_Red_S;                 //Maxima Potencia que puede salir  a  la red  en fase S
		double  Minima_Potencia_Salida_Red_S;                 //Minima Potencia que puede salir  a  la red  en fase S
		double  Maxima_Potencia_Salida_Red_T;                 //Maxima Potencia que puede salir  a la red en fase T
		double  Minima_Potencia_Salida_Red_T;                 //Minima Potencia que puede salir  a la red en fase T
		double  Maxima_Potencia_Terminal[NUMERO_TERMINALES];  //Maxima Potencia que pueden intercambiar los terminales
		double  Minima_Potencia_Terminal[NUMERO_TERMINALES];  //Minima Potenica que pueden intercambiar los terminales
	}Restricciones_Electrolinera;                            

#ifdef __cplusplus
}
#endif
#endif