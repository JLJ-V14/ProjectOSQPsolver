#pragma once
#ifndef OPTIMIZACION_CUADRATICA_H
#define OPTIMIZACION_CUADRATICA_H
#include <stddef.h>
#include <stdlib.h>
#include "Tipos_Optimizacion.h"
#include "osqp.h"
#ifdef __cplusplus
extern "C" {
#endif
    void Optimizacion_Cuadratica(const Sistema_Carga* Electrolinera, const Caracteristicas_Simulacion* Tiempo_Simulacion,
        const double* Precio_Compra, const double* Precio_Venta, const Operador_Red* Referencia_Operador, const int* Array_Objetivos,
        const Bateria* Baterias_Sistema, const Panel_Solar* Datos_Paneles, const Carga_Adicional* Cargas_Sistema, const* Restricciones_Electrolinera);
#ifdef __cplusplus
}
#endif
#endif