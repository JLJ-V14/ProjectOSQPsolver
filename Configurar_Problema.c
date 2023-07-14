
#include "Configurar_Problema.h"
#include "osqp.h"
#include "Configurar_Ajustes_Problema.h"
#include "Tipos_Optimizacion.h"

static void Configurar_Matriz_P(OSQPData* Data) {
	//En este subprograma se configura la matriz que viene a representar los objetivos cuadraticos

}
static void Configurar_Matriz_Q(OSQPData* Data) {
	//En este subprograma se configura la matriz de objetivos no cuadraticos
}
static void Configurar_Matriz_A(OSQPData* Data,const int Puntos_Simulacion) {
	//En este subprograma se configura la matriz que representa las restricciones de igualdad y desigualdad
	//Lo primero es crear los arrays dinamicos que la matriz A va a utilizar.
	//Hay 3 Arrays a destacar
	//A_x Array que contiene los valores diferentes de cero.
	//A_i Array que indica las filas en las que se encuentran los valores diferentes de 0.
	//A_p Array que indica las columnas en las que se encuentran los valores diferentes de 0.
		double* A_x;
		double* A_i;
		double* A_p;
//Los espacios reservados en memoria siguen la siguiente regla:
//A_x es el array que indica cuantos elementos distintos de cero hay, su tamaño depende de como
// son las ecuaciones
// A_i indica en que fila de la matriz se encuentran dichas variables
// A_p indica las columnas+1 y las columnas es el numero de variables , A_p va a ser igual al numero de variables+1
// Y aqui hay 59 variables multiplicado por el numero de puntos de la simulacion
		Crear_Array_Dinamico_Double(&A_x,158*Puntos_Simulacion);
		Crear_Array_Dinamico_Double(&A_i, 158 * Puntos_Simulacion);
		Crear_Array_Dinamico_Double(&A_p, 59 * Puntos_Simulacion + 1);

}
static int Crear_Array_Dinamico_Double (double**Matriz,const int size) {
	 *Matriz = (double*)calloc(size,sizeof(double));
	 if (*Matriz == NULL) {
		 printf("Memory allocation failed \n");
		 return -1;
	 }
	 return 0;
}
//static void Configurar_Vector_L_U(OSQPData* Data,const int N_Variables, const int N_Ecuaciones, const int Puntos_Simulacion,
								//  const Operador_Red*Operador,const int Minutos_Restantes,const int Puntos_Restantes_Horas,
								//  const Estacion* Estacion_Carga) {
	//En esta parte del subprograma se crean los arrays dinamicos l y u
	//const int size = N_Variables + N_Ecuaciones;
	//double* l;
	//double* u;
	//Crear_Array_Dinamico_Double(&l,size);
	//Crear_Array_Dinamico_Double(&u, size);
	//Se llama a un subprograma que se encarga de rellenar ese array con los datos.
	
//}

static void Configurar_Matrices(OSQPData *Data,const int N_Variables, const int N_Ecuaciones,
								const int Puntos_Simulacion,const double*Precio_Compra,const double*Precio_Venta,
								const Operador_Red* Operador, const int Minutos_Restantes, const int Puntos_Restantes_Horas,
								const Estacion* Estacion_Carga){
	Configurar_Matriz_P(Data);
	Configurar_Matriz_Q(Data);
	Configurar_Matriz_A(Data,Puntos_Simulacion);
	//Configurar_Vector_L_U(Data,N_Variables,N_Ecuaciones,Puntos_Simulacion,Operador,Minutos_Restantes, Puntos_Restantes_Horas,Estacion_Carga);

}
static void Calcular_Variables_Vehiculo(const Vehiculo *Vehiculo,int *N_Variables) {
	//Son Dos porque hay variables tanto para la potencia como para el estado de la bateria.
	*N_Variables += 2*(Vehiculo->Punto_Final_Carga-Vehiculo->Punto_Inicio_Carga);

}
static void Calcular_Vehiculos_Estacion(const Estacion* Estacion_Carga, int* N_Variables) {
	for (int j = 0; j < Estacion_Carga->Num_Vehiculos; j++) {
    Calcular_Variables_Vehiculo(&(Estacion_Carga->Vehiculos[j]), N_Variables);
	}
}
static void Calcular_Variables_Bateria(int *N_Variables,const Caracteristicas_Simulacion*Tiempo_Simulacion,
	                                   const Bateria *Baterias_Sistema) {
	int N_Baterias = 0;
	if (Baterias_Sistema->Bateria_R == true) {
		N_Baterias += 1;
	}
	if (Baterias_Sistema->Bateria_S == true) {
		N_Baterias += 1;
	}
	if (Baterias_Sistema->Bateria_T == true){
		N_Baterias += 1;
	}
	*N_Variables = N_Baterias * Tiempo_Simulacion->Puntos_Simulacion;
}

static void Calcular_Numero_Variables_Ecuaciones(const Sistema_Carga *Electrolinera, int *N_Variables, int *N_Ecuaciones,
	                                             const Caracteristicas_Simulacion* Tiempo_Simulacion, const Bateria *Baterias_Sistema) {
	//Calculo el numero de variables que va a haber en total en el problema matemático.
	//Calculo el numero de variables correspondientes al numero de vehiculos
    //Estado de baterias y potencia intercambiada por los vehiculos
	for (int i = 0; i < Electrolinera->Numero_Estaciones; i++) {
		Calcular_Vehiculos_Estacion(&(Electrolinera->Estaciones_Carga[i]),N_Variables);
	}
   //Añado el Numero de variables que siempre van a estar independientemente del numero de vehiculos
	N_Variables += 12 * Tiempo_Simulacion->Puntos_Simulacion;
	//Añado las variables correspondientes a las baterias del sistema.
	Calcular_Variables_Bateria(N_Variables,Tiempo_Simulacion,Baterias_Sistema);
	//Calculo el numero de Ecuaciones+Inecuaciones de la matriz A
	
}

void  Configurar_Problema_Optimizacion(OSQPWorkspace* Settings, OSQPData* Data,const Sistema_Carga   *Electrolinera, const Caracteristicas_Simulacion* Tiempo_Simulacion,
	const c_float        * Precio_Compra,        const c_float *Precio_Venta,     const Operador_Red *Referencia_Operador,
	const Objetivos      * Array_Objetivos,      const Bateria *Baterias_Sistema, const Panel_Solar  *Datos_Paneles,
	const Carga_Adicional* Cargas_Sistema,       const Restricciones_Electrolinera *Restricciones) {
	//const int Nvariables = 59 * Puntos_Simulacion;
	//const int Necuaciones =34 * Puntos_Simulacion;
	//Configurar_Ajustes(Settings);
	//Configurar_Matrices(Data,Nvariables,Necuaciones,Puntos_Simulacion,Precio_Compra,Precio_Venta,Operador,Minutos_Restantes,
						//Puntos_Restantes_Horas);
}