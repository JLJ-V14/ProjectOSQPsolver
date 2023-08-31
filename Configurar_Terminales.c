#include "Definiciones_Globales.h"
#include "Tipos_Optimizacion.h"

void Inicializar_Terminales(Informacion_Terminal** Informacion_Terminales) {
	//Este subprograma se utiliza para inicializar la variable que contiene todo lo que tiene su carga programada
	//en cada terminal.
	(*Informacion_Terminales) = (Informacion_Terminal*)malloc(NUMERO_TERMINALES*sizeof(Informacion_Terminal));
	for (int i = 0; i < NUMERO_TERMINALES; i++) {
		//Se inicializa la informacion de la variable que contiene los datos de los terminales->
		Informacion_Terminales[i]->Numero_Vehiculos = 0;
		Informacion_Terminales[i]->Numero_Baterias = 0;
		Informacion_Terminales[i]->Array_Punto_Inicio_Bateria = NULL;
		Informacion_Terminales[i]->Array_Punto_Final_Bateria = NULL;
		Informacion_Terminales[i]->Array_Punto_Inicio_Vehiculo = NULL;
		Informacion_Terminales[i]->Array_Punto_Final_Vehiculo = NULL;
	}
}
void Configurar_Terminales(Elementos_Electrolinera *Informacion_Sistema,Informacion_Terminal **Informacion_Terminales) {
	//Subprograma para rellenar con la informacion procedente la variable que contiene la informacion de cada terminal
	//Itero por cada uno de los vehiculos.
	//Creo variables para llevar la cuenta de cuantos vehiculos hay por terminal
	int Numero_Vehiculos_Terminal[12] = 0;
	int Numero_Baterias_Terminal[12] = 0;
	//Se crean variables para llevar la cuenta de cuantas baterias hay por terminal
	for (int i = 0; i < Informacion_Sistema->Numero_Vehiculos; i++) {
		//Se reserva memoria para el nuevo elemento que tiene su carga programada en el terminal->
		(*Informacion_Terminales).
		int Numero_Terminal =       Informacion_Sistema->Vehiculos_Sistema[i].Numero_Terminal;
		int Punto_Inicio_Vehiculo = Informacion_Sistema->Vehiculos_Sistema[i].Punto_Inicial_Carga;
		int Punto_Final_Vehiculo =  Informacion_Sistema->Vehiculos_Sistema[i].Punto_Final_Carga;

	}

	//Itero por cada una de las baterias->
	for (int i = 0; i < Informacion_Sistema->Numero_Baterias; i++) {
		int Numero_Terminal      = Informacion_Sistema->Baterias_Sistema[i].Numero_Terminal;
		int Punto_Inicio_Bateria = Informacion_Sistema->Baterias_Sistema[i].Punto_Inicial_Bateria;
		int Punto_Final_Bateria  = Informacion_Sistema->Baterias_Sistema[i].Punto_Final_Bateria;

	}
}