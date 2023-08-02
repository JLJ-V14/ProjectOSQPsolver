// Juan López Jódar Últimos Cambios : 1 / 08 / 2023
//Incluyo una serie de archivos
#include <stdio.h>
#include <stdbool.h>
#include "main.h" 
#include "Obtener_Info_Entrada.h"      //Header file que incluye las funciones de leer la entrada.
#include "Optimizacion_Cuadratica.h"   //Header file que incluye la funcion principal de optimizacion
#include "osqp.h"                      //Header file que incluye las funciones del algoritmo de optimizacion
#include "Preparar_Arrays.h"           //Header file en el que van incluidas las funciones para reservar memorias para los arrays.s
#include "Tipos_Optimizacion.h"        //Header file que incluye los tipos propios definidos para el algoritmo de optimizacion 
#define  Num_Objetivos 5               //Numero de objetivos = 5


void main_optimizacionC(void) {
	//FUNCIONAMIENTO DEL CODIGO:
	//PRIMERO SE LEE LA INFORMACION DEL SISTEMA, POSTERIORMENTE SE CONFIGURAN LAS MATRICES, Y FINALMENTE SE EJECUTA LA FUNCION PRINCIPAL DE OPTIMIZACION.
	//POSTERIORMENTE SE EXTRAEN LOS RESULTADOS
	//Sistema_Carga                                Electrolinera;                            //Variable que almacena los datos de la electrolinera
	//Operador_Red* Demanda_Operador = NULL;                                                 //Inicializo la demanda del operador
	//Caracteristicas_Simulacion                   Tiempo_Algoritmo;                         //Variable que almacena las caracteristicas Temporales de la simulacion
	//Panel_Solar                                  Datos_Panel_Fotovoltaico;                 //Variable que almacena los valores generados por los paneles fotovoltaicos
	//Carga_Adicional                              Datos_Carga;                              //Variable que almacena los valores consumidos por la carga adicional.
	//Restricciones_Electrolinera                  Restricciones_Potencia;                   //Variable que almacena los valores que pueden
	//Bateria                                      Baterias_Sistema;                         //Variable que sirve para indicar las baterias que hay en el sistema
	//const int                                    Numero_Estaciones = 12;                   //Se considera que hay 12 estaciones.
	//c_float                                      Precio_Compra;                            //Variable que  indica los precios de compra de la electricidad
	///c_float                                      Precio_Venta;                             //Variable que  indica los precios de vender electricidad al operador de la red
	//Objetivos                                    Objetivos_Optimizacion;                   //Array booleano que indica si se estan considerando una serie de objetivos, donde cada casilla representa un objetivo.


	//Se Leen la informacion de todo el Sistema.
	//if (Leer_Informacion(&Objetivos_Optimizacion, &Restricciones_Potencia, &Tiempo_Algoritmo, &Electrolinera,
	//	&Datos_Panel_Fotovoltaico, &Datos_Carga, &Precio_Compra, &Precio_Venta, &Demanda_Operador,
	//	&Baterias_Sistema) == -1) {
	//	Liberar_Toda_Memoria(&Datos_Panel_Fotovoltaico, &Datos_Carga, Demanda_Operador, &Tiempo_Algoritmo, &Electrolinera);
	//	return;
	//}
	//Optimizacion_Cuadratica(&Electrolinera, &Tiempo_Algoritmo, &Precio_Compra, &Precio_Venta,
    //&Demanda_Operador, &Objetivos_Optimizacion, &Baterias_Sistema, &Datos_Panel_Fotovoltaico,
	//	&Datos_Carga, &Restricciones_Potencia);
	//Liberar_Toda_Memoria(&Datos_Panel_Fotovoltaico, &Datos_Carga, Demanda_Operador, &Tiempo_Algoritmo, &Electrolinera);

}

int main(int argc, char** argv) {
	//(void)argc;
	//(void)argv;
	//main_optimizacionC();   //Main de la optimizacion
	//return 0;
}
//Lista de Índices de las variables:
//De 0 a Numero de coches *2
// 