#pragma once

#ifndef SIN_SUPERPOSICION_H
#define SIN_SUPERPOSICION_H

/* Include files */

#include <stddef.h>
#include <stdlib.h>
#include "Tipos_Optimizacion.h"

#ifdef __cplusplus
extern "C" {
#endif

	    int Verificar_Superposicion_Terminales(const Datos_CSV*** Datos_Vehiculos, const Datos_CSV*** Datos_Baterias,
		const int Numero_Filas_Csv_Vehiculos, const int Numero_Filas_Csv_Baterias);

#ifdef __cplusplus
}
#endif

#endif