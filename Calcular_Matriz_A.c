#include "osqp.h"
#include "Tipos_Optimizacion.h"

#define NUMERO_VARIABLES 36
#define TERMINOS_FIJOS   33
#define TERMINOS_EC_VARIABLE 3


int Calcular_Dimension_Ap(const Elementos_Electrolinera* Informacion_Sistema) {
	//Calcular la dimension de este vector es la mas complicada->

}
//Este subprograma se utiliza para calcular la dimension que va a tener  el vector A_x
int Calcular_Dimension_Ax(const Elementos_Electrolinera *Informacion_Sistema) {


	int Dimension_Variable = 0;
	int Puntos_Simulacion = Informacion_Sistema->Numero_Puntos;
	int Dimension_Constante = (NUMERO_VARIABLES + TERMINOS_FIJOS) * Puntos_Simulacion;
	
	//Para calcular la dimension del vector es necesario tener en cuenta durante cuantos puntos de simulacion
	//hay presentes vehiculos o baterias conectadas al terminal->
	int Numero_Vehiculos = Informacion_Sistema->Numero_Vehiculos;
	int Numero_Baterias = Informacion_Sistema->Numero_Baterias;
	//Para las ecuaciones de variacion del estado de bateria hay que tener en cuenta que para el punto inicial no 
	//hay ecuacion solo un termino Estado de bateria= Estado de bateria inicial . Para el resto la ecuacion
	//contiene tres terminos.
	int Termino_Inicial_Vehiculo = Numero_Vehiculos;
	int Termino_Inicial_Bateria =  Numero_Baterias;
	
	//En la dimension variable se incluye el termino inicial de las baterias del vehiculo y de la bateria
	Dimension_Variable = Termino_Inicial_Vehiculo + Termino_Inicial_Bateria;

	int Variables_Ecuacion_Bateria_Vehiculo = 0;
	int Variables_Ecuacion_Bateria_Bateria = 0;

	int Punto_Inicial_Vehiculo;
	int Punto_Final_Vehiculo;

	int Punto_Inicial_Bateria;
	int Punto_Final_Bateria;

	for (int i = 0; i < Numero_Vehiculos; i++) {
		Punto_Inicial_Vehiculo = Informacion_Sistema->Vehiculos_Sistema[i].Punto_Inicial_Carga;
		Punto_Final_Vehiculo = Informacion_Sistema->Vehiculos_Sistema[i].Punto_Final_Carga;
        
		Variables_Ecuacion_Bateria_Vehiculo = (Punto_Final_Vehiculo - Punto_Inicial_Vehiculo) * TERMINOS_EC_VARIABLE ;
	}
	for (int i = 0; i < Numero_Baterias; i++) {
		Punto_Inicial_Bateria = Informacion_Sistema->Baterias_Sistema[i].Punto_Inicial_Bateria;
		Punto_Final_Bateria = Informacion_Sistema->Baterias_Sistema[i].Punto_Final_Bateria;

		Variables_Ecuacion_Bateria_Bateria = (Punto_Final_Bateria - Punto_Inicial_Bateria) * TERMINOS_EC_VARIABLE;
	}
	Dimension_Variable = Dimension_Variable + Variables_Ecuacion_Bateria_Bateria + Variables_Ecuacion_Bateria_Vehiculo;
	
	return Dimension_Variable;
}
void Calcular_Matriz_A(const Elementos_Electrolinera *Informacion_Sistema,c_float **A_x,c_int **A_P,
	                   c_int *A_i) {
//Este subprograma se utiliza para calcular la matriz A, que contiene los terminos de las restricciones de limite
//y las ecuaciones que deben cumplir las matrices en el problema de optimizacion

//La Matriz A se indica con tres vectores que son los siguientes->
//A_x-> Son los elementos distintos de zero de la matriz A
//A_i-> Indica las filas de la matriz en las que se encuentran eso elementos distintos de zero
//A_p-> Sirve para separar los miembros de a_x en columnas. en el se indican los indices en los cuales
//se cambia de columna, por ejemplo si el elemento dos del vector a_x esta en una columna distinta del anterior
//se indica en el vector el indice 2

//Se reserva memoria para los diferentes_vectores, el tamaño de estos vectores dependen de los vehiculos 
//y baterias que haya conectados, ya que algunas ecuaciones estan presentes o no dependiendo de si hay vehiculos
//conectados

//Otros vectores van a tener siempre el mismo tamaño.
	int Dimension_Vector_Ax= Calcular_Dimension_Ax(Informacion_Sistema);
//A_i tiene la misma dimension que A_x
	int Dimension_Vector_Ai = Dimension_Vector_Ax;

	(*A_x) = (c_float*)calloc(Dimension_Vector_Ax, sizeof(c_float));
	(*A_i) = (c_float*)calloc(Dimension_Vector_Ai, sizeof(c_float));

}