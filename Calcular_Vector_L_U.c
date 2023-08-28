#include "osqp.h"
#include "Tipos_Optimizacion.h"

#define VARIABLES_POTENCIAS  12
#define BATERIAS_TERMINALES  12
#define POTENCIAS_TERMINALES 12
#define NUMERO_VARIABLES 36
#define NUMERO_ECUACIONES 20
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
	printf("El numero de terminal es %d \n", Numero_Terminal);
	Index_Variable = Index_Base + Numero_Terminal * Numero_Puntos_Simulacion + Punto_Inicial;
	printf("EL index de la variable es %d", Index_Variable);
	for (int i = Punto_Inicial; i <= Punto_Final; i++) {
		printf("La iteracion en el bucle es %d \n", i);
		
	//Se establece el valor maximo de 100 %
		Vector_U[Index_Variable] = (c_float) 1;

		Index_Variable += 1;
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
	
	printf("QQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQ\n");
	printf("El numero de terminal es %d", Informacion_Sistema->Vehiculos_Sistema[0].Numero_Terminal);
	printf("\n");
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
double Obtener_Potencia_Minima(const double Potencia_Terminal,const double Potencia_Elemento) {
	//Este subprograma toma dos entradas la potencia que puede intercambiar el terminal y la potencia
	//que puede ceder la bateria y devuelve el valor mas restrictivo.
	if (Potencia_Terminal>=Potencia_Elemento) {
		return Potencia_Terminal;
	}
	else {
		return Potencia_Elemento;
	}
}
void Configurar_Potencia_Vehiculo(const Elementos_Electrolinera* Informacion_Sistema,const int Numero_Vehiculo,
	                               c_float* Vector_U,const Restricciones_Electrolinera *Restricciones_Sistema) {
	//En este subprograma se establece cuales son los limites de potencia que puede intercambiar el terminal
	//mientras este presente ese vehiculo->

	//Primero se carga a que terminal pertenece el vehiculo
	int Terminal = Informacion_Sistema->Vehiculos_Sistema[Numero_Vehiculo].Numero_Terminal;
	//Se determina que restriccion es mayor si la potencia que puede atravesar el terminal, o la que puede aceptar
	//el vehiculo
	double Maxima_Potencia_Terminal = Restricciones_Sistema->Maxima_Potencia_Terminal[Terminal-1];
	double Maxima_Potencia_Vehiculo = Informacion_Sistema->Vehiculos_Sistema[Numero_Vehiculo].Maxima_Potencia;
	double Potencia_Maxima = Obtener_Potencia_Maxima(Maxima_Potencia_Terminal, Maxima_Potencia_Vehiculo);
	//Una vez hecho esto se pasa a escribir en el vector los limites de potencia que pueden intercambiar los terminales
	//cuando hay un vehiculo presente.
	//Cargo el punto inicial y final de los vehiculos->
	int Punto_Inicial = Informacion_Sistema->Vehiculos_Sistema[Numero_Vehiculo].Punto_Inicial_Carga;
	int Punto_Final = Informacion_Sistema->Vehiculos_Sistema[Numero_Vehiculo].Punto_Final_Carga;
	
	//Se calcula en que posicion del vector U hay que empezar a escribir los limites->
	int Puntos_Simulacion = Informacion_Sistema->Numero_Puntos;
	int Index = VARIABLES_POTENCIAS * Puntos_Simulacion + BATERIAS_TERMINALES * Puntos_Simulacion +(Terminal-1) *Puntos_Simulacion+ Numero_Vehiculo * Puntos_Simulacion;
	Index = Index + Punto_Inicial;
	//Se escribe en las posiciones correspondientes del vector U los valores de potencia maxima.
	for (int i = Punto_Inicial; i <= Punto_Final; i++) {
		Vector_U[Index] = (c_float)Potencia_Maxima;
		Index += 1;
	}

}
void Configurar_Potencias_Terminales_Vehiculos(c_float* Vector_U,const Restricciones_Electrolinera* Restricciones_Sistema ,
	const Elementos_Electrolinera* Informacion_Sistema) {
	//Este subprograma se utiliza para configurar la potencia que puede intercambiar, los terminales 
	//si hay un vehiculo conectado.
	for (int i = 0; i < Informacion_Sistema->Numero_Vehiculos; i++) {
		Configurar_Potencia_Vehiculo(Informacion_Sistema, i, Vector_U, Restricciones_Sistema);
	}

}

void Configurar_Potencia_Bateria(c_float * Vector_L, c_float *Vector_U,const Restricciones_Electrolinera *Restricciones_Sistema,
	                             const Elementos_Electrolinera *Informacion_Sistema,const int Numero_Bateria) {
	//Primero se carga a que terminal pertenece la bateria
	int Terminal = Informacion_Sistema->Baterias_Sistema[Numero_Bateria].Numero_Terminal;
	//Es necesario determinar que es mas restrictivo si la potencia que puede intercambiar el terminal, o la que puede
	//intercambiar la bateria->
	double Maxima_Potencia_Terminal = Restricciones_Sistema->Maxima_Potencia_Terminal[Terminal-1];
	double Minima_Potencia_Terminal = Restricciones_Sistema->Minima_Potencia_Terminal[Terminal - 1];
	double Maxima_Potencia_Bateria =  Informacion_Sistema->Baterias_Sistema[Numero_Bateria].Maxima_Potencia;
	double Minima_Potencia_Bateria = -Informacion_Sistema->Baterias_Sistema[Numero_Bateria].Maxima_Potencia;

	//Obtengo las potencias maximas y minimas que puede intercambiar el terminal mientras haya una bateria presente->
	double Maxima_Potencia = Obtener_Potencia_Maxima(Maxima_Potencia_Terminal,Maxima_Potencia_Bateria);
	double Minima_Potencia = Obtener_Potencia_Minima(Minima_Potencia_Terminal, Minima_Potencia_Bateria);

	//Se calcula en que posicion del vector hay que situar los valores maximos y minimos.
	int Puntos_Simulacion = Informacion_Sistema->Numero_Puntos;
	
	//Se calcula la posicion del vector a la que hay que acceder
	int Index = VARIABLES_POTENCIAS * Puntos_Simulacion + BATERIAS_TERMINALES * Puntos_Simulacion + (Terminal - 1) * Puntos_Simulacion;

	//Cargo el punto inicial y final en el que estan presentes las baterias
	int Punto_Inicial = Informacion_Sistema->Baterias_Sistema[Numero_Bateria].Punto_Inicial_Bateria;
	int Punto_Final = Informacion_Sistema->Baterias_Sistema[Numero_Bateria].Punto_Final_Bateria;

	//Actualizo el Index con el punto inicial;
	Index = Index + Punto_Inicial;

	printf("El punto inicial de la bateria es %d \n", Punto_Inicial);
	printf("El punto final de la bateria es %d \n", Punto_Final);
	//Se escriben en las posiciones correspondientes del vector los valores de restriccion
	for (int i = Punto_Inicial; i <= Punto_Final; i++) {
		
		Vector_L[Index] = (c_float) Minima_Potencia;
		Vector_U[Index] = (c_float) Maxima_Potencia;
		Index = Index + 1;
	}
}

void Configurar_Potencias_Terminal_Bateria(c_float *Vector_L ,c_float *Vector_U, const Restricciones_Electrolinera *Restricciones_Sistema,
	                                       const Elementos_Electrolinera* Informacion_Sistema) {
	//En este subprograma se configura la potencia que pueden intercambiar los terminales, en el caso de que haya
	//una bateria conectada.
	for (int i = 0; i < Informacion_Sistema->Numero_Baterias; i++) {
		Configurar_Potencia_Bateria(Vector_L, Vector_U, Restricciones_Sistema, Informacion_Sistema,i);
	}
}
void Configurar_Restriccion_Terminal(c_float* Vector_L, c_float* Vector_U, const int Numero_Puntos,
	const Elementos_Electrolinera* Informacion_Sistema,const Restricciones_Electrolinera *Restricciones_Sistema) {
	//Para ajustar los valores maximos y minimos de potencia que pueden intercambiar los terminales de la placa
	//de redistribucion, hay que tener en cuenta lo siguiente:
	//Si no hay nada conectado el maximo y minimo de potencia que podr� intercambiar el terminal sera 0
	//Si no hay un vehiculo conectado el minimo sera 0 y el maximo depender� de que sea menor si la restriccion
	//del terminal o del propio vehiculo.
	//Si hay una bateria conectada el minimo y el maximo dependera de que sea mas restrictivo si la potencia 
	//que pueda intercambiar el terminal, o el propio vehiculo.
	
	Configurar_Potencias_Terminales_Vehiculos(Vector_U, Restricciones_Sistema, Informacion_Sistema);
	Configurar_Potencias_Terminal_Bateria(Vector_L, Vector_U, Restricciones_Sistema, Informacion_Sistema);

}
void Configurar_Vector_L_U(c_float **Vector_L, c_float **Vector_U,  Elementos_Electrolinera *Informacion_Sistema,
	                       const Restricciones_Electrolinera*Restriccion_Sistema) {
	//El numero de variables en el problema de optimizacion es 36 y hay que multiplicarlas por el numero de
	//puntos de simulacion.

	//Ademas el vector L ha de ser tan grande como numero de ecuaciones haya y por tanto->

	int Numero_Puntos = Informacion_Sistema->Numero_Puntos;
	
	int Dimension_Vector=(NUMERO_VARIABLES+NUMERO_ECUACIONES)*Numero_Puntos;
	
	
	//Reservo memoria para los vectores L y U 
	 *Vector_L= (c_float*)calloc(Dimension_Vector,sizeof(c_float));
	 *Vector_U = (c_float*)calloc(Dimension_Vector, sizeof(c_float));

	//Se llama a los subprogramas que se encargan de escribir en el vector L y U las restricciones que corresponden
	//a las variables.
	//Se configuran los limites de potencia que la electrolinera puede intercambiar con la red
	Configurar_Restriccion_Potencias(*Vector_L,*Vector_U, Numero_Puntos, Restriccion_Sistema);
	//Se configurar los limites que pueden tomar las baterias (vehiculos o baterias adicionales)
	
	Configurar_Restriccion_Baterias(*Vector_U, Informacion_Sistema);
	//Se configuran los limties de potencia que pueden intercambiar los terminales de la placa de redistribucion
	Configurar_Restriccion_Terminal(*Vector_L, *Vector_U, Numero_Puntos, Informacion_Sistema,Restriccion_Sistema);
	
	printf("Comprobacion vector L y U \n");
	printf("%.2f", (*Vector_L)[0]);
	printf("%.2f", (*Vector_U)[0]);
}