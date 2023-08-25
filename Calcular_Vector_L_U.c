#include "osqp.h"
#include "Tipos_Optimizacion.h"

#define NUMERO_VARIABLES 36

//El orden de las variables es el siguiente->
//La primera variable corresponde a la potencia intercambiada entre la red y la electrolinera
//

void Ajustar_Limite_Bateria(c_float *Vector_U,const int Punto_Inicial, const int Punto_Final,
	                        const int Terminal, const int Numero_Puntos_Simulacion) {
	//Se escribe en el vector que las variables que representan una bateria como maximo pueden alcanzar el 
	//100 por 100.

	//Para obtener el index correcto del vector hay que tener en cuenta las variables que van antes indexadas
	//(potencias del sistema)
	int Index_Base = 12*Numero_Puntos_Simulacion;
	int Index_Variable;
	//Hay que tener en cuenta ademas que el primer terminal es 1, no 0 (zero indexing)
	int Numero_Terminal = Terminal - 1;
	Index_Variable = Index_Base + Numero_Terminal * Numero_Puntos_Simulacion;
	for (int i = Punto_Inicial; i <= Punto_Final; i++) {
		Index_Variable = Index_Variable + i;
	//Se establece el valor maximo de 100 %
		Vector_U[Index_Variable] = 1;
	}
}
void Configurar_Restriccion_Baterias(c_float* Vector_U, const Elementos_Electrolinera *Informacion_Sistema) {
	//En este subprograma se escribe los valores maximos que la bateria de los vehiculos puede alcanzar que 
	//corresponde a 1 el 100 %.
	// Creo variables que marca de que punto a que punto de ejecucion del algoritmo esta la bateria presente
	// asi como el numero de terminal al que esta conectado la bateria en cuestion.
	int Punto_Inicial;
	int Punto_Final;
	int Numero_Terminal;
	//Cargo el Numero de vehiculos->
	int Numero_Vehiculos = Informacion_Sistema->Numero_Vehiculos;
	int Numero_Puntos_Simulacion = Informacion_Sistema->Numero_Puntos;
	for (int i = 0; i < Numero_Vehiculos; i++) {
		Punto_Inicial = Informacion_Sistema->Vehiculos_Sistema[i].Punto_Inicial_Carga;
		Punto_Final = Informacion_Sistema->Vehiculos_Sistema[i].Punto_Final_Carga;
		Numero_Terminal = Informacion_Sistema->Vehiculos_Sistema[i].Numero_Terminal;
		Ajustar_Limite_Bateria(Vector_U, Punto_Inicial, Punto_Final, Numero_Terminal, Numero_Puntos_Simulacion);
	}
    //Ahora se pasa a establecer los limites de las baterias que no pertenecen a vehiculos
	int Numero_Baterias = Informacion_Sistema->Numero_Baterias;
	for (int i = 0; i < Numero_Baterias; i++) {
		//Al igual que para las baterias de los vehiculos se carga el punto de inicio el punto de fin, y el numero
		//de terminal para saber la posicion de las variables en el vector.
		Punto_Inicial =   Informacion_Sistema->Baterias_Sistema[i].Punto_Inicial_Bateria;
		Punto_Final =     Informacion_Sistema->Baterias_Sistema[i].Punto_Final_Bateria;
		Numero_Terminal = Informacion_Sistema->Baterias_Sistema[i].Numero_Terminal;
		Ajustar_Limite_Bateria(Vector_U, Punto_Inicial, Punto_Final, Numero_Terminal, Numero_Puntos_Simulacion);
	}
}

void Configurar_Restriccion_Potencias(c_float *Vector_L, c_float *Vector_U, const int Numero_Puntos,
	                                  const Restricciones_Electrolinera*Restriccion_Sistema) {
	
	for (int i = 0; i < Numero_Puntos; i++) {
		Vector_L[i] = (c_float) Restriccion_Sistema->Minima_Potencia_Red;
		Vector_U[i] = (c_float) Restriccion_Sistema->Maxima_Potencia_Red;
		Vector_L[i + Numero_Puntos] = (c_float)Restriccion_Sistema->Minima_Potencia_Red_Entrada;
		Vector_U[i + Numero_Puntos] = (c_float)Restriccion_Sistema->Maxima_Potencia_Red_Entrada;
		Vector_L[i + 2 * Numero_Puntos] = (c_float)Restriccion_Sistema->Minima_Potencia_Red_Salida;
		Vector_U[i + 2 * Numero_Puntos] = (c_float)Restriccion_Sistema->Maxima_Potencia_Red_Salida;
		Vector_L[i + 3 * Numero_Puntos] = (c_float)Restriccion_Sistema->Minima_Potencia_Red_R;
		Vector_U[i + 3 * Numero_Puntos] = (c_float)Restriccion_Sistema->Maxima_Potencia_Red_R;
		Vector_L[i + 4 * Numero_Puntos] = (c_float)Restriccion_Sistema->Minima_Potencia_Red_S;
		Vector_U[i + 4 * Numero_Puntos] = (c_float)Restriccion_Sistema->Maxima_Potencia_Red_S;
		Vector_L[i + 5 * Numero_Puntos] = (c_float)Restriccion_Sistema->Minima_Potencia_Red_T;
		Vector_U[i + 5 * Numero_Puntos] = (c_float)Restriccion_Sistema->Maxima_Potencia_Red_T;
		Vector_L[i + 6 * Numero_Puntos] = (c_float)Restriccion_Sistema->Minima_Potencia_Entrada_Red_R;
		Vector_U[i + 6 * Numero_Puntos] = (c_float)Restriccion_Sistema->Maxima_Potencia_Entrada_Red_R;
		Vector_L[i + 7 * Numero_Puntos] = (c_float)Restriccion_Sistema->Minima_Potencia_Entrada_Red_S;
		Vector_U[i + 7 * Numero_Puntos] = (c_float)Restriccion_Sistema->Maxima_Potencia_Entrada_Red_S;
		Vector_L[i + 8 * Numero_Puntos] = (c_float)Restriccion_Sistema->Minima_Potencia_Entrada_Red_T;
		Vector_U[i + 8 * Numero_Puntos] = (c_float)Restriccion_Sistema->Maxima_Potencia_Entrada_Red_T;
		Vector_L[i + 9 * Numero_Puntos] = (c_float)Restriccion_Sistema->Minima_Potencia_Salida_Red_R;
		Vector_U[i + 9 * Numero_Puntos] = (c_float)Restriccion_Sistema->Maxima_Potencia_Salida_Red_R;
		Vector_L[i + 10 * Numero_Puntos] = (c_float)Restriccion_Sistema->Minima_Potencia_Salida_Red_S;
		Vector_U[i + 10 * Numero_Puntos] = (c_float)Restriccion_Sistema->Maxima_Potencia_Salida_Red_S;
		Vector_L[i + 11 * Numero_Puntos] = (c_float)Restriccion_Sistema->Minima_Potencia_Salida_Red_T;
		Vector_U[i + 11 * Numero_Puntos] = (c_float)Restriccion_Sistema->Maxima_Potencia_Salida_Red_T;

	}
	
}
double Obtener_Potencia_Maxima(const double Potencia_Terminal, const double Potencia_Elemento) {
	//Este programa toma dos entradas, el limite de potencia que puede intercambiar el terminal,
	//y la maxima potencia que puede intercambiar el vehiculo o bateria, devuelve el valor menor.
	if (Potencia_Terminal >= Potencia_Elemento) {
		return Potencia_Elemento;
	}
	else {
		return Potencia_Terminal;
	}

}
void Configurar_Potencia_Vehiculo(const Elementos_Electrolinera* Informacion_Sistema,const int Numero_Vehiculo,
	                              c_float *Vector_L, c_float* Vector_U,const Restricciones_Electrolinera *Restricciones_Sistema) {
	//En este subprograma se establece cuales son los limites de potencia que puede intercambiar el terminal
	//mientras este presente ese vehiculo->

	//Primero se carga a que terminal pertenece el vehiculo
	int Terminal = Informacion_Sistema->Vehiculos_Sistema[Numero_Vehiculo].Numero_Terminal;


}
void Configurar_Potencias_Terminales_Vehiculos(c_float *Vector_L, c_float* Vector_U,
	const Restricciones_Electrolinera* Restricciones_Sistema ,const Elementos_Electrolinera* Informacion_Sistema) {
	//Este subprograma se utiliza para configurar la potencia que puede intercambiar, los terminales 
	//si hay un vehiculo conectado.
	for (int i = 0; i < Informacion_Sistema->Numero_Vehiculos; i++) {

	}

}
void Configurar_Restriccion_Terminal(c_float* Vector_L, c_float* Vector_U, const int Numero_Puntos,
	const Elementos_Electrolinera* Informacion_Sistema) {
	//Para ajustar los valores maximos y minimos de potencia que pueden intercambiar los terminales de la placa
	//de redistribucion, hay que tener en cuenta lo siguiente:
	//Si no hay nada conectado el maximo y minimo de potencia que podrá intercambiar el terminal sera 0
	//Si no hay un vehiculo conectado el minimo sera 0 y el maximo dependerá de que sea menor si la restriccion
	//del terminal o del propio vehiculo.
	//Si hay una bateria conectada el minimo y el maximo dependera de que sea mas restrictivo si la potencia 
	//que pueda intercambiar el terminal, o el propio vehiculo.
	
}
void Configurar_Vector_L_U(c_float *Vector_L, c_float *Vector_U,  Elementos_Electrolinera *Informacion_Sistema,
	                       const Restricciones_Electrolinera*Restriccion_Sistema) {
	//El numero de variables en el problema de optimizacion es 36 y hay que multiplicarlas por el numero de
	//puntos de simulacion.
	int Numero_Puntos = Informacion_Sistema->Numero_Puntos;
	int Numero_Variables=NUMERO_VARIABLES*Informacion_Sistema->Numero_Puntos;

	//Reservo memoria para los vectores L y U 
	c_float *Vector_L= (c_float*)calloc(Numero_Variables,sizeof(c_float));
	c_float *Vector_U = (c_float*)calloc(Numero_Variables, sizeof(c_float));

	//Se llama a los subprogramas que se encargan de escribir en el vector L y U las restricciones que corresponden
	//a las variables.
	Configurar_Restriccion_Potencias(Vector_L,Vector_U, Numero_Puntos, Restriccion_Sistema);
	Configurar_Restriccion_Baterias(Vector_L, Vector_U, Numero_Puntos, Restriccion_Sistema);
	Configurar_Restriccion_Terminal(Vector_L, Vector_U, Numero_Puntos, Informacion_Sistema);
}