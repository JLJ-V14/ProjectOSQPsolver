#include "Calcular_Size_Matrices.h"
#include "Tipos_Optimizacion.h"

static void Calcular_Variables_Vehiculo(const Vehiculo* Vehiculo, int* N_Variables) {
	//Son Dos porque hay variables tanto para la potencia como para el estado de la bateria.
	*N_Variables += 2 * (Vehiculo->Punto_Final_Carga - Vehiculo->Punto_Inicio_Carga);

}
static void Calcular_Vehiculos_Estacion(const Estacion* Estacion_Carga, int* N_Variables) {
	for (int j = 0; j < Estacion_Carga->Num_Vehiculos; j++) {
		Calcular_Variables_Vehiculo(&(Estacion_Carga->Vehiculos[j]), N_Variables);
	}
}

static void Calcular_Variables_Bateria(int* N_Variables, const Caracteristicas_Simulacion* Tiempo_Simulacion,
	const Bateria* Baterias_Sistema) {
	int N_Baterias = 0;
	if (Baterias_Sistema->Bateria_R == true) {
		N_Baterias += 1;
	}
	if (Baterias_Sistema->Bateria_S == true) {
		N_Baterias += 1;
	}
	if (Baterias_Sistema->Bateria_T == true) {
		N_Baterias += 1;
	}
	*N_Variables = N_Baterias * Tiempo_Simulacion->Puntos_Simulacion;
}

void Calcular_Numero_Variables_Ecuaciones(const Sistema_Carga* Electrolinera, int* N_Variables, int* N_Ecuaciones,
	const Caracteristicas_Simulacion* Tiempo_Simulacion, const Bateria* Baterias_Sistema) {
	//Calculo el numero de variables que va a haber en total en el problema matemático.
	//Calculo el numero de variables correspondientes al numero de vehiculos
	//Estado de baterias y potencia intercambiada por los vehiculos
	for (int i = 0; i < Electrolinera->Numero_Estaciones; i++) {
		Calcular_Vehiculos_Estacion(&(Electrolinera->Estaciones_Carga[i]), N_Variables);
	}
	//Añado el Numero de variables que siempre van a estar independientemente del numero de vehiculos
	N_Variables += 12 * Tiempo_Simulacion->Puntos_Simulacion;
	//Añado las variables correspondientes a las baterias del sistema.
	Calcular_Variables_Bateria(N_Variables, Tiempo_Simulacion, Baterias_Sistema);
	//Calculo el numero de Ecuaciones+Inecuaciones de la matriz A

}