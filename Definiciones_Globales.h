#pragma once
#pragma once
#ifndef DEFINICIONES_GLOBALES_H
#define DEFINICIONES_GLOBALES_H


//Se definen constantes relacionadas con el uso de la estructura temporal de tipo tm->
#define OFFSET_ANYO_tm                   1900 //Usando variables de tipo tm los años empiezan a partir de 1900
#define OFFSET_MES_tm                      1  //Usando variables de tipo tm los meses empiezan en el mes 0
//Se definen constantes relacionadas con el problema matematico de optimizacion
#define NUMERO_TERMINALES 12          //Numero de terminales que hay en la placa de redistribucion
#define NUMERO_VARIABLES  36          //Numero de variables  que hay en el problema de optimizacion

//Se definen las filas columnas en las que se encuentra la informacion en el CSV que contiene la fecha inicial y final
//del algoritmo.
#define FILA_INFORMACION_TEMPORAL_ALGORITMO 1  //Fila    donde se encuentra la informacion en el CSV.
#define COLUMNA_ANYO_INICIAL_ALGORITMO      0  //Columna donde se encuentra el año inicial del algoritmo
#define COLUMNA_MES_INICIAL_ALGORITMO       1  //Columna donde se encuentra el mes inicial del algoritmo
#define COLUMNA_DIA_INICIAL_ALGORITMO       2  //Columna donde se ecuentra  el dia inicial del algoritmo
#define COLUMNA_HORA_INICIAL_ALGORITMO      3  //Columna donde se encuentra la hora inicial del algoritmo
#define COLUMNA_MINUTO_INICIAL_ALGORITMO    4  //Columna donde se encuentra el minuto inicial del algoritmo
#define COLUMNA_ANYO_FINAL_ALGORITMO        5  //Columna donde se encuentra el año final del algoritmo
#define COLUMNA_MES_FINAL_ALGORITMO         6  //Columna donde se encuentra el mes final del algoritmo
#define COLUMNA_DIA_FINAL_ALGORITMO         7  //Columna donde se encuentra el dia final del algoritmo
#define COLUMNA_HORA_FINAL_ALGORITMO        8  //Columna donde se encuentra la hora final del algoritmo
#define COLUMNA_MINUTO_FINAL_ALGORITMO      9  //Columna donde se encuentra el minuto final del algoritmo
#define COLUMNA_DELTA_T                     10 //Columna donde se encuentra la diferencia temporal entre puntos.

//Se definen las filas y columnas donde se encuentra la informacion en el CSV que contiene la informacion de los vehiculos
#define OFFSET_FILA_CSV_VEHICULOS                         1  //Variable para tener en cuenta que la primera fila del csv no tiene los datos.
#define COLUMNA_CSV_VEHICULOS_NUMERO_TERMINAL             0  //Columna donde se encuentra el terminal al que esta conectado el vehiculos
#define COLUMNA_CSV_VEHICULOS_MODO_CARGA                  1  //Columna donde se encuentra el modo de carga en el que se encuentra el vehiculo
#define COLUMNA_CSV_VEHICULOS_CAPACIDAD_BATERIA           2  //Columna donde se encuentra la capacidad de la bateria del vehiculo
#define COLUMNA_CSV_VEHICULOS_BAT_INICIAL                 3  //Columna donde se encuentra la capacidad inicial de la bateria
#define COLUMNA_CSV_VEHICULOS_BAT_FINAL                   4  //Columna donde se encuentra que porcentaje se desea de bateria
#define COLUMNA_CSV_VEHICULOS_MAX_POTENCIA                5  //Columna donde se encuentra el maximo de potencia que acepta el vehiculo
#define COLUMNA_CSV_VEHICULOS_ANYO_INICIAL                6  //Columna donde se encuentra el año inicial de la carga del vehiculo
#define COLUMNA_CSV_VEHICULOS_MES_INICIAL                 7  //Columna donde se encuentra el mes inicial de la carga del vehiculo
#define COLUMNA_CSV_VEHICULOS_DIA_INICIAL                 8  //Columna donde se encuentra el dia inicial de la carga del vehiculo
#define COLUMNA_CSV_VEHICULOS_HORA_INICIAL                9  //Columna donde se encuentra la hora inicial de la carga del vehiculo
#define COLUMNA_CSV_VEHICULOS_MINUTO_INICIAL              10 //Columna donde se encuentra el minuto inicial de la carga del vehiculo
#define COLUMNA_CSV_VEHICULOS_ANYO_FINAL                  11 //Columna donde se encuentra el año final de la carga del vehiculo
#define COLUMNA_CSV_VEHICULOS_MES_FINAL                   12 //Columna donde se encuentra el mes final de la carga del vehiculo
#define COLUMNA_CSV_VEHICULOS_DIA_FINAL                   13 //Columna donde se encuentra el dia final de la carga del vehiculo
#define COLUMNA_CSV_VEHICULOS_HORA_FINAL                  14 //Columna donde se encuentra la hora final de la carga del vehiculo
#define COLUMNA_CSV_VEHICULOS_MINUTO_FINAL                15 //Columna donde se encuentra el minuto final de la carga del vehiculo
//Defino las filas y columnas donde se encuentra la informacion en el CSV que contiene la informacion de las baterias

#define OFFSET_FILA_CSV_BATERIAS
#define COLUMNA_CSV_BATERIAS_NUMERO_TERMINAL                            0  //Columna donde se encuentra el terminal al que esta conectado la bateria
#define COLUMNA_CSV_BATERIAS_CAPACIDAD_BATERIA                          1  // Columna donde se encuentra la capacidad de la bateria
#define COLUMNA_CSV_BATERIAS_INICIAL                                    2  // Columna donde se encuentra el porcentaje de bateria inicial
#define COLUMNA_CSV_BATERIAS_FINAL                                      3  // Columna donde se encuentra el porcentaje de bateria final deseado
#define COLUMNA_CSV_BATERIAS_MAXIMA_POTENCIA                            4  // Columna donde se encuentra la maxima potencia que puede intercambiar la bateria
#define COLUMNA_CSV_BATERIAS_ANYO_INICIAL                               5  // Columna donde se ecuentra el año inicial de carga de la bateria
#define COLUMNA_CSV_BATERIAS_MES_INICIAL                                6  // Columna donde se encuentra el mes inicial de carga de las baterias
#define COLUMNA_CSV_BATERIAS_DIA_INICIAL                                7  // Columna donde se encuentra el dia inicial de carga de las baterias
#define COLUMNA_CSV_BATERIAS_HORA_INICIAL                               8  // Columna donde se encuentra la hora inicial de carga de la bateria
#define COLUMNA_CSV_BATERIAS_MINUTO_INICIAL                             9  // Columna donde se encuentra el minuto inicial de carga de la bateria
#define COLUMNA_CSV_BATERIAS_ANYO_FINAL                                 10 // Columna donde se encuentra si se considera objetivo de carga de la bateria
#define COLUMNA_CSV_BATERIAS_MES_FINAL                                  11 // Columna donde se encuentra el año objetivo de carga de la bateria
#define COLUMNA_CSV_BATERIAS_DIA_FINAL                                  12 // Columna donde se encuentra el mes objetivo de carga de la bateria
#define COLUMNA_CSV_BATERIAS_HORA_FINAL                                 13 // Columna donde se encuentra el dia objetivo de carga de la bateria
#define COLUMNA_CSV_BATERIAS_MINUTO_FINAL                               14 // Columna donde se encuentra la hora objetivo de carga de la bateria
#define COLUMNA_CSV_BATERIAS_OBJETIVO_CARGA                             15 // Columna donde se considera el objetivo de la carga.
#define COLUMNA_CSV_BATERIAS_ANYO_OBJETIVO                              16 // Columna donde se encuentra el año objetivo de la carga
#define COLUMNA_CSV_BATERIAS_MES_OBJETIVO                               17 // Columna donde se encuentra el mes objetivo de la carga
#define COLUMNA_CSV_BATERIAS_DIA_OBJETIVO                               18 // Columna donde se encuentra el dia objetivo de la carga
#define COLUMNA_CSV_BATERIAS_HORA_OBJETIVO                              19 // Columna donde se encuentra el hora objetivo de la carga
#define COLUMNA_CSV_BATERIAS_MINUTO_OBJETIVO                            20 // Columna donde se encuentra el minuto objetivo de la carga 
//-----------------------------------------------------------------------------------------------------------
//Defino los codigos para las funciones tipo int que devuelven -1 si ha habido algun error y 0 si no
#define EXITO 0
#define ERROR -1



//--------------Se definen las columnas donde se encuentran las columnas donde se encuentra la informacion en el CSV
//--------------de los precios del kilovatio hora

#define PRIMERA_FILA_CSV_PRECIO   1     //La primera fila que tiene informacion es la que viene indexada con el 1
#define COLUMNA_CSV_ANYO_PRECIO   0     //La columna donde se encuentra el año asociado a la fecha es la columna 0
#define COLUMNA_CSV_MES_PRECIO    1     //La columna donde se encuentra el mes asociado a la fecha es la columna 1
#define COLUMNA_CSV_DIA_PRECIO    2     //La columna donde se encuentra el dia asociado a la fecha es la columna 2
#define COLUMNA_CSV_HORA_PRECIO   3     //La columna donde se encuentra la hora asociada a la fecha es la columna 3
#define COLUMNA_CSV_PRECIO        4     //La columna donde se encuentra el precio es la columna 4

//---------------Definiciones adicionales----------------------------------------------------------------

#define SEGUNDOS_HORA  3600   //Segundos que hay en una hora

//--------------------Se define donde se encuentra la informacion en el CSV de los terminales

#define COLUMNA_CSV_NUMERO_TERMINAL 0  // La columna donde viene indicado el numero de terminal
#define COLUMNA_CSV_FASE_TERMINAL   1  // La columna donde viene indicado la fase del terminal.

#ifdef __cplusplus
extern "C" {
#endif

#ifdef __cplusplus
}
#endif
#endif