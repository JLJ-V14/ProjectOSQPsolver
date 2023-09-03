
//En este archivo se encuentran los subprogramas que verifican 
//que los datos de los CSVs de los vehiculos son correctos.


#include "Definiciones_Globales.h"
#include "Funciones_Auxiliares.h"
#include "Tipos_Optimizacion.h"
#include "Verificar_Tiempo_Algoritmo.h"

//--Revisado-----------

static int Comprobar_Validez_Datos(const double Capacidad_Bateria, const double Maxima_Potencia, const double Bateria_Inicial,
	const double Bateria_Deseada) {
	//Este subprograma se utiliza para comprobar si:
	//El dato de la capacidad de la bateria tiene sentido
	//El dato de la potencia maxima que se puede aceptar tiene sentido
	//El dato de la bateria inicial tiene sentido
	//El dato de la bateria deseada tiene sentido
	if (Capacidad_Bateria <= 0) {
		printf("La capacidad de la bateria debe ser mayor que 0 \n");
		return ERROR;
	}
	else if (Maxima_Potencia < 0) {
		printf("La potencia maxima no puede ser inferior a 0 \n");
		return ERROR;
	}
	else if ((Bateria_Inicial < 0) || (Bateria_Inicial > 1)) {
		printf("La bateria Inicial ha de estar entre 0 o 1 (0% y 100% \n");
		return ERROR;
	}
	else if ((Bateria_Deseada < 0) || (Bateria_Deseada > 1)) {
		printf("La bateria deseada ha de estar entre 0 o 1 (0% y 100% \n");
		return ERROR;
	}
	return EXITO;
}

static void Cargar_Fecha_Inicial_Vehiculo(const Datos_CSV*** Datos_Vehiculos, struct tm* Fecha_Inicial_Vehiculo,
	                                      const int Numero_Vehiculo) {
	//Se carga la fecha inicial de carga del vehiculo en la variable Fecha_Inicial_Vehiculo
	Fecha_Inicial_Vehiculo->tm_year = Datos_Vehiculos[Numero_Vehiculo][COLUMNA_CSV_VEHICULOS_ANYO_INICIAL]->data.dbl - OFFSET_ANYO_tm;
	Fecha_Inicial_Vehiculo->tm_mon  = Datos_Vehiculos[Numero_Vehiculo][COLUMNA_CSV_VEHICULOS_MES_INICIAL]->data.dbl - OFFSET_MES_tm;
	Fecha_Inicial_Vehiculo->tm_mday = Datos_Vehiculos[Numero_Vehiculo][COLUMNA_CSV_VEHICULOS_DIA_INICIAL]->data.dbl;
	Fecha_Inicial_Vehiculo->tm_hour = Datos_Vehiculos[Numero_Vehiculo][COLUMNA_CSV_VEHICULOS_HORA_INICIAL]->data.dbl;
	Fecha_Inicial_Vehiculo->tm_min  = Datos_Vehiculos[Numero_Vehiculo][COLUMNA_CSV_VEHICULOS_MINUTO_INICIAL]->data.dbl;
	Fecha_Inicial_Vehiculo->tm_sec  = 0;
}

static void Cargar_Fecha_Final_Vehiculo(const Datos_CSV*** Datos_Vehiculos, struct tm* Fecha_Final_Vehiculo,
	                                    const int Numero_Vehiculo) {
	//Se carga la fecha final de carga del vehiculo en la variable Fecha_Final_Vehiculo
	Fecha_Final_Vehiculo->tm_year =  Datos_Vehiculos[Numero_Vehiculo][COLUMNA_CSV_VEHICULOS_ANYO_FINAL]->data.dbl - OFFSET_ANYO_tm;
	Fecha_Final_Vehiculo->tm_mon  =  Datos_Vehiculos[Numero_Vehiculo][COLUMNA_CSV_VEHICULOS_MES_FINAL]->data.dbl - OFFSET_MES_tm;
	Fecha_Final_Vehiculo->tm_mday =  Datos_Vehiculos[Numero_Vehiculo][COLUMNA_CSV_VEHICULOS_DIA_FINAL]->data.dbl;
	Fecha_Final_Vehiculo->tm_hour =  Datos_Vehiculos[Numero_Vehiculo][COLUMNA_CSV_VEHICULOS_HORA_FINAL]->data.dbl;
	Fecha_Final_Vehiculo->tm_min  =  Datos_Vehiculos[Numero_Vehiculo][COLUMNA_CSV_VEHICULOS_MINUTO_FINAL]->data.dbl;
	Fecha_Final_Vehiculo->tm_sec  =  0;
    }

    static int Comprobar_Fecha_Inicial_Vehiculo(struct tm* Fecha_Inicial_Vehiculo, const Datos_CSV*** Datos_Vehiculos,
	const int Numero_Vehiculo, const struct tm Fecha_Inicial_Algoritmo) {

	//Este subprograma se utiliza para comprobar que la fecha inicial 
	//de carga del vehiculo tiene sentido
	Cargar_Fecha_Inicial_Vehiculo(Datos_Vehiculos, Fecha_Inicial_Vehiculo, Numero_Vehiculo);
	int Resultado_Comparacion;

	Resultado_Comparacion = Comprobar_Orden_Fechas(Fecha_Inicial_Algoritmo, *Fecha_Inicial_Vehiculo);

	//Si la fecha inicial del algoritmo es posterior a la fecha inicial de carga del vehiculo, la fecha es incorrecta
	if (Resultado_Comparacion > 0) {
		printf("La fecha inicial del vehiculo no puede ser anterior a la fecha inicial del algoritmo %d \n", Numero_Vehiculo);
		return ERROR;
	}
	else {
		return EXITO;
	}
    }

    static int Comprobar_Fecha_Final_Vehiculo(const Datos_CSV*** Datos_Vehiculos, struct tm* Fecha_Final_Vehiculo,
	const struct tm Fecha_Final_Algoritmo, const int Numero_Vehiculo) {
	//Este subprograma se utiliza para comprobar 
	//que la fecha final del vehiculo es correcta->

	Cargar_Fecha_Final_Vehiculo(Datos_Vehiculos, Fecha_Final_Vehiculo, Numero_Vehiculo);
	int Resultado_Comparacion;

	Resultado_Comparacion = Comprobar_Orden_Fechas(Fecha_Final_Algoritmo, *Fecha_Final_Vehiculo);

	if (Resultado_Comparacion < 0) {
		printf("Error el algoritmo no puede acabar antes que la carga del vehiculo %d \n", Numero_Vehiculo);
		return ERROR;
	}
	else {
		return EXITO;
	}
    }

    static int Comprobar_Orden(const struct tm Fecha_Inicial, const struct tm Fecha_Final) {
	//Este subprograma se utiliza para comprobar que la fecha inicial de conexion 
	//del vehiculo es anterior a la fecha de desconexion.

	int Resultado_Comparacion = Comprobar_Orden_Fechas(Fecha_Inicial, Fecha_Final);
	if (Resultado_Comparacion > 0) {
		return ERROR;
	}
	return EXITO;
    }

    int Comprobar_Fecha_Vehiculos(const int Numero_Vehiculo, const Datos_CSV*** Datos_Vehiculos, const Datos_CSV ***Datos_Algoritmo) {

	//En este subprograma se comprueba que las fechas de inicio y 
	//finalizacion de carga de los vehiculos son correctas.
	
	//Creo variables Auxiliares
	struct tm Fecha_Inicial_Algoritmo = { 0 };
	struct tm Fecha_Final_Algoritmo   = { 0 };
	struct tm Fecha_Inicial_Vehiculo;
	struct tm Fecha_Final_Vehiculo;

	Cargar_Fecha_Inicial_Algoritmo(Datos_Algoritmo, &Fecha_Inicial_Algoritmo, Numero_Vehiculo);
	Cargar_Fecha_Final_Algoritmo(Datos_Algoritmo,   &Fecha_Final_Algoritmo,   Numero_Vehiculo);

	//Se comprueba si la fecha inicial del vehiculo es igual o posterior a la fecha de inicio del algoritmo
	if (Comprobar_Fecha_Inicial_Vehiculo(&Fecha_Inicial_Vehiculo, Datos_Vehiculos, Numero_Vehiculo, Fecha_Inicial_Algoritmo) == ERROR) {
	printf("Error la fecha inicial del vehiculo %d es incorrecta \n ", Numero_Vehiculo + 1);
	return ERROR;
	}

	//Se comprueba que la fecha final del vehiculo no es posterior a la fecha final del algoritmo
	if (Comprobar_Fecha_Final_Vehiculo(Datos_Vehiculos, &Fecha_Final_Vehiculo, Fecha_Final_Algoritmo, Numero_Vehiculo) == ERROR) {
	printf("Error la fecha final del vehiculo %d es incorrecta \n", Numero_Vehiculo + 1);
	return ERROR;
	}

    //Se comprueba que la fecha inicial no supera a la fecha final
	if (Comprobar_Orden(Fecha_Inicial_Vehiculo, Fecha_Final_Vehiculo) == ERROR) {
	printf("Error la fecha inicial de carga del vehiculo %d no puede ser posterior a la fecha final de carga \n", Numero_Vehiculo + 1);
	return ERROR;
	}

	return EXITO;
    }

    static int Comprobar_Validez_Modo_Carga(const Datos_CSV ***Datos_Vehiculos,const int Numero_Vehiculo) {
	//Se utiliza este subprograma para comprobar que el modo de carga coincide con la palabra rapida o normal
	char* Modo_Carga = Datos_Vehiculos[OFFSET_FILA_CSV_VEHICULOS+Numero_Vehiculo][COLUMNA_CSV_VEHICULOS_MODO_CARGA];

	if (strcmp(Modo_Carga, "RAPIDA") == 0) {
		return EXITO;
	}
	else if (strcmp(Modo_Carga, "NORMAL") == 0) {
		return EXITO;
	}
	else {
		return ERROR;
	}
    }
    static int Verificar_Bateria_Vehiculo(const Datos_CSV ***Datos_Vehiculos, const int Numero_Vehiculo) {
    //Este subprograma se utiliza para comprobar si los datos de la bateria de cada vehiculo, son correctos

	double Capacidad_Bateria = Datos_Vehiculos[Numero_Vehiculo][COLUMNA_CSV_VEHICULOS_CAPACIDAD_BATERIA]->data.dbl;
	double Maxima_Potencia   = Datos_Vehiculos[Numero_Vehiculo][COLUMNA_CSV_VEHICULOS_MAX_POTENCIA]->data.dbl;
	double Bateria_Inicial   = Datos_Vehiculos[Numero_Vehiculo][COLUMNA_CSV_VEHICULOS_BAT_INICIAL]->data.dbl;
	double Bateria_Deseada   = Datos_Vehiculos[Numero_Vehiculo][COLUMNA_CSV_VEHICULOS_BAT_FINAL]->data.dbl;

	if (Comprobar_Validez_Datos(Capacidad_Bateria, Maxima_Potencia, Bateria_Inicial, Bateria_Deseada) == ERROR) {
		printf("Los Datos de la bateria del vehiculo %d son incorrectos,", Numero_Vehiculo);
		return ERROR;
	}
	else {
		return EXITO;
	}
    }

    int Verificar_Informacion_Vehiculos(const Datos_CSV ***Datos_Vehiculos, const int Numero_Filas_CSV_Vehiculos,
	                                       const Datos_CSV ***Datos_Algoritmo) {
    
	//Este subprograma se utiliza para comprobar que la infromacion del CSV de los vehiculos 
	//es correcta

	for (int Numero_Vehiculo = 1; Numero_Vehiculo < Numero_Filas_CSV_Vehiculos; Numero_Vehiculo++) {

	//Se comprueba que los datos de la bateria del vehiculo sean correctos
	if (Verificar_Bateria_Vehiculo(Datos_Vehiculos, Numero_Vehiculo) == ERROR) {
	printf("Los Datos de la bateria del vehiculo %d son incorrectos", Numero_Vehiculo);
     return ERROR;
	}

	//Se comprueba que el modo de carga del vehiculo es valido
	if (Comprobar_Validez_Modo_Carga(Datos_Vehiculos, Numero_Vehiculo) == ERROR) {
	printf("Los Datos del modo de carga del vehiculo %d son incorrectos", Numero_Vehiculo);
	return ERROR;
	}

	//Se comprueba que las fechas iniciales y finales de los diferentes vehiculos son correctas.
	if (Comprobar_Fecha_Vehiculos(Numero_Vehiculo, Datos_Vehiculos, Datos_Algoritmo) == ERROR) {
	printf("La fecha inicial o final de carga del vehiculo %d son incorrectas", Numero_Vehiculo );
	return ERROR;
	}

	}
	return EXITO;
}

