#include "Calcular_Vector_L_U.h"
#include "Configurar_Ajustes_Problema.h"
#include "Comprobar_Informacion.h"
#include "Tipos_Optimizacion.h"
#include "osqp.h"



void Optimizacion_Cuadratica(const Elementos_Electrolinera *Informacion_Sistema,
	                        const Restricciones_Electrolinera *Restricciones_Sistema) {
	//Esta es la funcion de optimizacion principal donde se realiza el calculo de optimizacion.
	//Estructuras del espacio de trabajo para la funcion de optimizacion
	//OSQPWorkspace* work = NULL;
	OSQPSettings* settings = (OSQPSettings*)c_malloc(sizeof(OSQPSettings));
	//OSQPData* data = (OSQPData*)c_malloc(sizeof(OSQPData));

	//Matrices y vectores que son necesarios para la optimizacion
	//L y U son los vectores de los valores minimos y maximos necesarios para la optimizacion.

	c_float *Vector_L = NULL;
	c_float *Vector_U = NULL;
	//Se establecen los ajustes de la optimizacion como los predeterminados.
	osqp_set_default_settings(settings);
	//Se configuran algunos ajustes de la optimizacion->
	Configurar_Ajustes(settings);
	
	
	//Se configuran los vectores L y U:
	
	Configurar_Vector_L_U(&Vector_L,&Vector_U,Informacion_Sistema,Restricciones_Sistema);
	printf("Comprobacion vector L y U \n");
	printf("%.2f", Vector_L[0]);
	printf("%.2f", Vector_U[0]);
	
	//Se comprueba que se ha escrito el vector L y el vector U correctamente.
	Comprobar_Vector_L_U(Vector_L, Vector_U, Informacion_Sistema);
	//Se libera el espacio en memoria reservado a las estructuras creadas para la optimizacion..
	c_free(Vector_L);
	c_free(Vector_U);
	c_free(settings);
}