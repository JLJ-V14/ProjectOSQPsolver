#include "Tipos_Optimizacion.h"
#include "Preparar_Arrays.h"
#include "osqp.h"

int Configurar_Objetivo_Precio(const Objetivos* Array_Objetivos, const Caracteristicas_Simulacion* Tiempo_Simulacion, const c_float Coeficiente_Precio,
	c_float* Array_Optimizacion_Precios, const c_float* Precio_Compra, const c_float* Precio_Venta) {
	int Index_Hora;
	int Hora_Actual;
	if (Array_Objetivos->Minimizar_Coste == 1) {
		Array_Optimizacion_Precios = malloc(2 * (Tiempo_Simulacion->Puntos_Simulacion) * sizeof(c_float));
		if (Array_Optimizacion_Precios == NULL) {
			return -1;
		}
		//Como el precio es algo que depende de la hora, hay que marcar cada hora con un punto de la simulacion.
		Hora_Actual = Tiempo_Simulacion->Hora_Inicio;
		Index_Hora = 0;
		for (int i = 0; i < Tiempo_Simulacion->Puntos_Simulacion; i++) {
			Calcular_Hora_Actual(Tiempo_Simulacion, Hora_Actual, Index_Hora, i);
			Array_Optimizacion_Precios[i] = (Coeficiente_Precio * (c_float)Precio_Compra[Index_Hora]) / (Tiempo_Simulacion->Resolucion_Minutos);
			Array_Optimizacion_Precios[Tiempo_Simulacion->Puntos_Simulacion + i] = ((c_float)(-1 * Coeficiente_Precio * Precio_Venta[Index_Hora])) / (Tiempo_Simulacion->Resolucion_Minutos);
		}
	}
	return 0;
}

int Configurar_Referencia_Operador_Lineal(const Objetivos* Array_Objetivos, c_float* Array_Referencia_Operador,
	const Operador_Red* Referencia_Operador, const Caracteristicas_Simulacion* Tiempo_Simulacion,const c_float Coeficiente_Referencia) {
	int Index_Hora;
	int Hora_Actual;
	c_float Delta_Minutos;
	//Si no se ha indicado que el objetivo se ignore
	if (Array_Objetivos->Obj_Referencia_Red != 0) {
		Array_Referencia_Operador = malloc(3 * (Tiempo_Simulacion->Puntos_Simulacion) * sizeof(c_float));
		if (Array_Referencia_Operador == NULL) {
			printf("La reserva de memoria \n");
			return -1;
		}
		Hora_Actual = Tiempo_Simulacion->Hora_Inicio;
		Index_Hora = 0;
		for (int i = 0; i < Tiempo_Simulacion->Puntos_Simulacion; i++) {
			if (i == 0) {
				Delta_Minutos = (c_float) (Tiempo_Simulacion->Delta_Primer_Punto);
			}
			else if (i<=Tiempo_Simulacion->Delta_Ultimo_Punto) {
				Delta_Minutos =(c_float) (Tiempo_Simulacion->Delta_Ultimo_Punto);
			}
			else {
				Delta_Minutos =(c_float) Tiempo_Simulacion->Resolucion_Minutos;
			}
			Array_Referencia_Operador[i] = (c_float)(Referencia_Operador->Fases[0].Consumo[i]) / (Delta_Minutos);
			Array_Referencia_Operador[(Tiempo_Simulacion->Puntos_Simulacion) + i]     = ((c_float)(-2) * Coeficiente_Referencia)* (((c_float)Referencia_Operador->Fases[1].Consumo[i]) / (Delta_Minutos));
			Array_Referencia_Operador[2 * (Tiempo_Simulacion->Puntos_Simulacion) + i] = ((c_float)(-2) * Coeficiente_Referencia)*(((c_float) Referencia_Operador->Fases[2].Consumo[i])/(Delta_Minutos));
		}
	}

	return 0;
}