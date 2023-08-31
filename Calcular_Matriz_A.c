#include "osqp.h"
#include "Tipos_Optimizacion.h"

#define NUMERO_VARIABLES 36       //Numero de variables que tiene el problema de optimizacion
#define TERMINOS_FIJOS   33       //Numero de variables que siempre van a estar en el vector 
#define TERMINOS_EC_VARIABLE 3    //3 Variables que apareceran mas o menos en funcion del numero de puntos de simulacion
#define PGRID_ECUACIONES 3        //Numero de veces que aparece la variable de intercambio de potencia de la red y la electrolinera
#define P_ENTRADA_ELECTROLINERA 3 //Numero de veces que aparece la variable que representa la potencia que entra a la electrolienra
#define P_SALIDA_ELECTROLINERA  3 //Numero de veces que aparece 
#define P_RED_ELECTROLINERA_R   3 //Potencia que intercambia la red con la electrolinera fase R
#define P_RED_ELECTROLINERA_S    3 //Potencia que intercambia la red con la electrolinera fase S
#define P_RED_ELECTROLINERA_T   3 //Potencia que intercambia la red con la electrolinera fase T

//La Matriz A sirve para definir las restricciones de borde, inecuaciones y ecuaciones.
//Cada columna representa una variable del problema matematico
//Cada fila una restriccion de borde, inecuacion o ecuacion.

//Variables por columna.
//Columnas (0 a 1*Puntos_Simulacion)-> Potencia que intercambian la electrolinera y la red 
//Columnas (Puntos_Simulacion+1 a 2*Puntos_Simulacion)-> Potencia que entra a la electrolinera de la red
//Columnas (2*Puntos_Simulacion+1 a 3*Puntos_Simulacion)-> Potencia que sale de la electorlinera a la red
//Columnas (3*Puntos_Simulacion+1 a 4*Puntos_Simulacion)-> Potencia que intercambian la electrolinera y la red fase R
//Columnas (4*Puntos_Simulacion+1 a 5*Puntos_Simulacion)-> Potencia que intercambian la electrolinera y la red fase S
//Columnas (5*Puntos_Simulacion+1 a 6*Puntos_Simulacion)-> Potencia que intercambian la electrolinera y la red fase T
//Columnas (6*Puntos_Simulacion+1 a 7*Puntos_Simulacion)-> Potencia que entra a la electrolinera fase R
//Columnas (7*Puntos_Simulacion+1 a 8*Puntos_Simulacion)-> Potencia que entra a la electrolinera fase S
//Columnas (8*Puntos_Simulacion+1 a 9*Puntos_Simulacion)-> Potencia que entra a la electrolinera fase T
//Columnas (9*Puntos_Simulacion+1 a 10*Puntos_Simulacion)-> Potencia que sale de la electrolinera fase R
//Columnas (10*Puntos_Simulacion+1 a 11*Puntos_Simulacion)-> Potencia que sale de la electrolinera fase S
//Columnas (11*Puntos_Simulacion+1 a 12*Puntos_Simulacion)-> Potencia que sale de la electrolinera fase T
//Columnas (12*Puntos_Simulacion+1 a 13*Puntos_Simulacion)-> Bateria del elemento conectado al terminal 1 
//Columnas (13*Puntos_Simulacion+1 a 14*Puntos_Simulacion)-> Bateria del elemento conectado al terminal 2
//Columnas (14*Puntos_Simulacion+1 a 15*Puntos_Simulacion)-> Bateria del elemento conectado al terminal 3
//Columnas (15*Puntos_Simulacion+1 a 16*Puntos_Simulacion)-> Bateria del elemento conectado al terminal 4
//Columnas (16*Puntos_Simulacion+1 a 17*Puntos_Simulacion)-> Bateria del elemento conectado al terminal 5
//Columnas (17*Puntos_Simulacion+1 a 18*Puntos_Simulacion)-> Bateria del elemento conectado al terminal 6
//Columnas (18*Puntos_Simulacion+1 a 19*Puntos_Simulacion)-> Bateria del elemento conectado al terminal 7
//Columnas (19*Puntos_Simulacion+1 a 20*Puntos_Simulacion)-> Bateria del elemento conectado al terminal 8
//Columnas (20*Puntos_Simulacion+1 a 21*Puntos_Simulacion)-> Bateria del elemento conectado al terminal 9
//Columnas (21*Puntos_Simulacion+1 a 22*Puntos_Simulacion)-> Bateria del elemento conectado al terminal 10
//Columnas (22*Puntos_Simulacion+1 a 23*Puntos_Simulacion)-> Bateria del elemento conectado al terminal 11
//Columnas (23*Puntos_Simulacion+1 a 24*Puntos_Simulacion)-> Bateria del elemento conectado al terminal 12
//Columnas (24*Puntos_Simulacion+1 a 25*Puntos_Simulacion)-> Potencia que intercambia el terminal 1 de la placa de redistribucion 
//Columnas (25*Puntos_Simulacion+1 a 26*Puntos_Simulacion)-> Potencia que intercambia el terminal 2 de la placa de redistribucion
//Columnas (26*Puntos_Simulacion+1 a 27*Puntos_Simulacion)-> Potencia que intercambia el terminal 3 de la placa de redistribucion
//Columnas (27*Puntos_Simulacion+1 a 28*Puntos_Simulacion)-> Potencia que intercambia el terminal 4 de la placa de redistribucion
//Columnas (28*Puntos_Simulacion+1 a 29*Puntos_Simulacion)-> Potencia que intercambia el terminal 5 de la placa de redistribucion
//Columnas (29*Puntos_Simulacion+1 a 30*Puntos_Simulacion)-> Potencia que intercambia el terminal 6 de la placa de redistribucion
//Columnas (30*Puntos_Simulacion+1 a 31*Puntos_Simulacion)-> Potencia que intercambia el terminal 7 de la placa de redistribucion
//Columnas (31*Puntos_Simulacion+1 a 32*Puntos_Simulacion)-> Potencia que intercambia el terminal 8 de la placa de redistribucion
//Columnas (32*Puntos_Simulacion+1 a 33*Puntos_Simulacion)-> Potencia que intercambia el terminal 9 de la placa de redistribucion
//Columnas (33*Puntos_Simulacion+1 a 34*Puntos_Simulacion)-> Potencia que intercambia el terminal 10 de la placa de redistribucion
//Columnas (34*Puntos_Simulacion+1 a 35*Puntos_Simulacion)-> Potencia que intercambia el terminal 11 de la placa de redistribucion
//Columnas (35*Puntos_Simulacion+1 a 36*Puntos_Simulacion)-> Potencia que intercambia el terminal 12 de la placa de redistribucion



//Ecuaciones que definen el comportamiento del sistema
// Pgrid->Potencia que intercambian la electrolinera y la red
// t->Punto de simulacion
//Ecuacion 1 Pgrid fase R,t = Sumatorio (Pterminales fase R) 
//Ecuacion 2 Pgrid fase S,t = Sumatorio (Pterminales fase S)
//Ecuacion 3 Pgrid fase T,t = Sumatorio (Pterminales fase T)
//Ecuacion 4 Pgrid,t        = Pin electolinera,t -Psalida,electrolinera,t 
//Ecuacion 5 Pgrid fase R,t = Pin electrolinera fase R,t -P salida electrolinera fase R,t
//Ecuacion 6 Pgrid fase S,t = Pin electrolinera fase S,t -P salida electrolinera fase S,t
//Ecuacion 7 Pgrid fase T,t = Pin electrolinera fase T,t -P salida electrolinera fase T,t
//Ecuacion 8 Pentrada electrolinera  = Sumatorio (Pentrada electrolinera fase R,t, Pentrada electrolinera fase S,t Pentrada electrolinera fase T,t
//Ecuacion 9 Psalida grid   = Sumatorio (Psalida electrolinera fase R, Psalida electrolinera fase S,t Psalida electrolinera fase R,t)
//Ecuacion 10 Potencia intercambiada por la red = Sumatorio (Potencia intercambiada con la red fase R S T) (cada fase tiene su variable independiente.
//Ecuacion 11 Ecuacion de modelado del comportamiento de la bateria SOC = SOC (anterior) +Pterminal * delta t /CAP
//SOC = State of charge CAP = Capacidad de la bateria en kilovatios hora delta t= diferencia en minutos de un punto a otro.

//Para la formulacion de la matriz A todos los terminos de la ecuacion se pasa al lado izquierdo, para igualar
//la expresion a 0.
// ya que en el problema de optimizacion la matriz A, los vectores lb y ub estan organizados de la siguiente forma
//  L< A <U             
// Las ecuaciones serian de la forma 0 = A_x = 0


//Las columnas de la matriz A estan escritos en el vector A_x de manera continua, es decir primero todos los 
//elementos distintos de 0 en la columna 1 luego los de la columna 2 y así sucesivamente.

void Restriccion_Potencia_Red_Fase(c_float *A_x, const int Index_Inicial,const int Index_Final) {
	//Este subprograma se encarga de escribir los terminos correspondientes en la matriz A, para la variable que
	//representa la potencia intercambiada entre la electrolinera y la red por cada fase para las restricciones
	//de ecuacion.

	for (int i = Index_Inicial; i < Index_Final; i+=4) {
		A_x[i] = 1;      //Termino Restriccion de borde
		A_x[i + 1] = 1;  //Ecuacion 1,2 o 3 depende de la fase
		A_x[i + 2] = 1;  //Ecuacion 5,6 o 7 depende de la fase
		A_x[i + 3] = -1; //Ecuacion 10
	}
}
void Restriccion_Potencia_Red(c_float* A_x, const int  Columna_Inicial, const int Columna_Final) {
	//Este subprogama añade la variable de la potencia que intercambian red y electroninera.
	for (int i = Columna_Inicial; i < Columna_Final-3; i+=3) {
		A_x[i] = 1;       //Termino Restriccion de Borde
		A_x[i + 1] = 1;   //Termino Ecuacion 4
		A_x[i + 2] = 1;   //Termino Ecuacion 10
	}
}
void Restriccion_Potencia_Entrada_Electrolinera(c_float *A_x, const int Columna_Inicial, const int Columna_Final) {
	//Este subprograma escribe en el vector A_x la potencia que entra a la electrolinera 
	for (int i = Columna_Inicial; i < Columna_Final - 3; i += 3){
		A_x[i] = 1;      //Termino Restriccion de borde
		A_x[i + 1] = -1; // Termino Ecuacion 4 
		A_x[i + 2] =  1; // Termino Ecuacion 8 
	}
}

void Restriccion_Potencia_Salida_Electrolinera(c_float* A_x, const int Columna_Inicial, const int Columna_Final) {
	//Este subprograma escribe en el vector A_x el termino correspondiente a la potencia que sale de la 
	//electrolinera.
	for (int i = Columna_Inicial; i < Columna_Final-3; i += 3) {
		A_x[i] = 1;        //Termino Restriccion de borde
		A_x[i + 1] = 1;    //Termino Ecuacion 4
		A_x[i + 2] = 1;    //Termino Ecuacion 9
	}
}
void Restriccion_Potencia_Electrolinera_Fase(c_float* A_x, const int Columna_Inicial, const int Columna_Final) {
	//Este subprograma escribe en el vector A_x el termino correspondiente a la potencia que intercambia la 
	//red y la electrolinera por una de las tres fases R,S o T, que sea una u otra depende del valor de columna
	//inicial o final introducido.
	for (int i = Columna_Inicial; i < Columna_Final - 4; i += 4) {
		A_x[i] = 1;        //Termino Restriccion de borde 
		A_x[i + 1] =  1;   //Termino Ecuacion 1 2 o 3 (Depende de la fase)
		A_x[i + 2] =  1 ;  //Termino Ecuacion 5 6 o 7 (Depende de la fase)
		A_x[i + 3] = -1;   //Termino Ecuacion 10
	}
}
void Restriccion_Potencia_Entrada_Electrolinera_Fase(c_float *A_x, const int Columna_Inicial, const int Columna_Final) {
	//Este subprograma escribe en el vector A_x el termino correspondiente a la potencia que entra a la 
	//electrolinera por cada una de las tres fases R,S,o T que sea una u otra depende del valor de columna inicial o
	//final introducido
	for (int i = Columna_Inicial; i < Columna_Final - 3; i += 3) {
		A_x[i] = 1;       //Restriccion de borde
		A_x[i + 1] = -1;  //Ecuacion 5,6 o 7
		A_x[i + 2] = -1;  //Ecuacion 8
	}
}

void Restriccion_Potencia_Salida_Electrolinera_Fase(c_float* A_x, const int Columna_Inicial, const int Columna_Final) {
	for (int i = Columna_Inicial; i < Columna_Final - 3; i += 3) {
		A_x[i] = 1;       //Restriccion de borde
		A_x[i + 1] = 1;   //Ecuacion 5, 6 o 7
		A_x[i + 2] = -1;  //Ecuacion 9
	}
}

int  Obtener_Numero_Vehiculos_Terminal(const int Terminal, const Elementos_Electrolinera* Informacion_Sistema) {
	//Este subprograma se utiliza para obtener cuantos vehiculos tienen su carga programa en un terminal.
	int Numero_Vehiculos_Terminal = 0;
	int Numero_Vehiculos_Electrolinera = Informacion_Sistema->Numero_Vehiculos;
	for (int i = 0; i < Numero_Vehiculos_Electrolinera; i++) {
		if (Informacion_Sistema->Vehiculos_Sistema[i].Numero_Terminal == Terminal) {

		}
	}
}
void Restriccion_Bateria_Terminal(c_float* A_x, const int Columna_Inicial, const int Columna_Final,
	                              const int Terminal, const Elementos_Electrolinera *Informacion_Sistema) {
	//En este subprograma se escriben los terminos de la matriz A que corresponde a la bateria del elemento conectado
	//al terminal.
	// Hay que tener en cuenta que aparte de las restricciones de borde hay que tener en cuenta las ecuaciones de 
	// modelado de la bateria si hay algo conectado, si no lo hay no.
	// Hay que tener este hecho en cuenta a la hora de escribir los terminos correspondientes en el vector A_x
	//Cargo el numero de puntos de simulacion
	int Puntos_Simulacion = Informacion_Sistema->Numero_Puntos;
	int Columna_Actual = Columna_Inicial;
	for (int i = 0; i < Puntos_Simulacion; i++) {
		A_x[Columna_Actual] = 1;    //Restriccion de Borde
		Columna_Actual += 1;
	}
}


void Calcular_Vector_Ax(const Elementos_Electrolinera* Informacion_Sistema,c_float *A_x) {
     //Hay que rellenar el vector con la informacion procedente, con los elementos distintos de 0.
	// Dentro de cada subprograma se indica la funcion
	int Columna_Inicial = 0;
	int Columna_Final = Informacion_Sistema->Numero_Puntos;
	Restriccion_Potencia_Red(A_x, Columna_Inicial, Columna_Final);

}
int Calcular_Dimension_Ap(const Elementos_Electrolinera* Informacion_Sistema) {
	//El tamaño de este vector es simplemente el numero de variables que hay 36, por
	//el numero de puntos de simulacion
	int    Dimension_Ap = NUMERO_VARIABLES* Informacion_Sistema->Numero_Puntos;
	return Dimension_Ap;
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
void Calcular_Matriz_A(const Elementos_Electrolinera *Informacion_Sistema,c_float **A_x,c_int **A_p,
	                   c_int **A_i) {
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
	int Dimension_Vector_Ap = Calcular_Dimension_Ap(Informacion_Sistema);

	(*A_x) = (c_float*)calloc(Dimension_Vector_Ax, sizeof(c_float));
	(*A_i) = (c_int*)calloc(Dimension_Vector_Ai, sizeof(c_int));
	(*A_p) = (c_int*)calloc(Dimension_Vector_Ap, sizeof(c_int));



}