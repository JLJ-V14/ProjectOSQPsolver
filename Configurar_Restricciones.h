#pragma once

#ifndef CONFIGURAR_RESTRICCIONES_H
#define CONFIGURAR_RESTRICCIONES_H

#include <stddef.h>
#include <stdlib.h>
#include <stdbool.h>
#include "Tipos_Optimizacion.h"
#ifdef __cplusplus
extern "C" {
#endif
	void Calcular_Restricciones_Sistema(const Datos_CSV *** Data_Restricciones, Restricciones_Electrolinera *Restricciones);
#ifdef __cplusplus
}
#endif

#endif