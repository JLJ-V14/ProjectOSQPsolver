
#include "time.h"
#include "Tipos_Optimizacion.h"
#include <stdbool.h>
#include <stdlib.h>

//Defino en que columnas se encuentra la informacion
#define MAX_FILA_RESTRICCION_SISTEMA  0
#define MAX_FILA_TERMINAL_1           1
#define MAX_FILA_TERMINAL_2           2
#define MAX_FILA_TERMINAL_3           3
#define MAX_FILA_TERMINAL_4           4
#define MAX_FILA_TERMINAL_5           5
#define MAX_FILA_TERMINAL_6           6
#define MAX_FILA_TERMINAL_7           7
#define MAX_FILA_TERMINAL_8           8
#define MAX_FILA_TERMINAL_9           9
#define MAX_FILA_TERMINAL_10          10
#define MAX_FILA_TERMINAL_11          11
#define MAX_FILA_TERMINAL_12          12

#define COLUM_VALOR_RESTRICCIONES     1
void Calcular_Restricciones_Sistema(const Celda *** Data_Restricciones, Restricciones_Electrolinera *Restricciones) {

	//Se Leen las restricciones que tiene el sistema (Maxima potencia que puede intercambiarse)
	
	//Se estable cual es la maxima potencia que puede intercambiar  la red y el sistema (bidireccional) ->
	Restricciones->Maxima_Potencia_Red =  Data_Restricciones[MAX_FILA_RESTRICCION_SISTEMA][COLUM_VALOR_RESTRICCIONES]->data.dbl;
	Restricciones->Minima_Potencia_Red = -Data_Restricciones[MAX_FILA_RESTRICCION_SISTEMA][COLUM_VALOR_RESTRICCIONES]->data.dbl;
	//Se establece cual es la maxima potencia que pueden intercambiar los terminales->
	Restricciones->Maxima_Potencia_Terminal[0] =  Data_Restricciones[MAX_FILA_TERMINAL_1][COLUM_VALOR_RESTRICCIONES]->data.dbl;
	Restricciones->Maxima_Potencia_Terminal[1] =  Data_Restricciones[MAX_FILA_TERMINAL_2][COLUM_VALOR_RESTRICCIONES]->data.dbl;
	Restricciones->Maxima_Potencia_Terminal[2] =  Data_Restricciones[MAX_FILA_TERMINAL_3][COLUM_VALOR_RESTRICCIONES]->data.dbl;
	Restricciones->Maxima_Potencia_Terminal[3] =  Data_Restricciones[MAX_FILA_TERMINAL_4][COLUM_VALOR_RESTRICCIONES]->data.dbl;
	Restricciones->Maxima_Potencia_Terminal[4] =  Data_Restricciones[MAX_FILA_TERMINAL_5][COLUM_VALOR_RESTRICCIONES]->data.dbl;
	Restricciones->Maxima_Potencia_Terminal[5] =  Data_Restricciones[MAX_FILA_TERMINAL_6][COLUM_VALOR_RESTRICCIONES]->data.dbl;
	Restricciones->Maxima_Potencia_Terminal[6] =  Data_Restricciones[MAX_FILA_TERMINAL_7][COLUM_VALOR_RESTRICCIONES]->data.dbl;
	Restricciones->Maxima_Potencia_Terminal[7] =  Data_Restricciones[MAX_FILA_TERMINAL_8][COLUM_VALOR_RESTRICCIONES]->data.dbl;
	Restricciones->Maxima_Potencia_Terminal[8] =  Data_Restricciones[MAX_FILA_TERMINAL_9][COLUM_VALOR_RESTRICCIONES]->data.dbl;
	Restricciones->Maxima_Potencia_Terminal[9] =  Data_Restricciones[MAX_FILA_TERMINAL_10][COLUM_VALOR_RESTRICCIONES]->data.dbl;
	Restricciones->Maxima_Potencia_Terminal[10] = Data_Restricciones[MAX_FILA_TERMINAL_11][COLUM_VALOR_RESTRICCIONES]->data.dbl;
	Restricciones->Maxima_Potencia_Terminal[11] = Data_Restricciones[MAX_FILA_TERMINAL_12][COLUM_VALOR_RESTRICCIONES]->data.dbl;

	//Se configurar el resto de restricciones en base a la informacion que se ha leido del CSV. Se establece que la
	//maxima potencia que puede fluir en sentido contrario, es decir del sistema a la red es el mismo valor pero 
	//con sentido negativo.

	Restricciones->Minima_Potencia_Terminal[0] = -Restricciones->Maxima_Potencia_Terminal[0];
	Restricciones->Minima_Potencia_Terminal[1] = -Restricciones->Maxima_Potencia_Terminal[1];
	Restricciones->Minima_Potencia_Terminal[2] = -Restricciones->Maxima_Potencia_Terminal[2];
	Restricciones->Minima_Potencia_Terminal[3] = -Restricciones->Maxima_Potencia_Terminal[3];
	Restricciones->Minima_Potencia_Terminal[4] = -Restricciones->Maxima_Potencia_Terminal[4];
	Restricciones->Minima_Potencia_Terminal[5] = -Restricciones->Maxima_Potencia_Terminal[5];
	Restricciones->Minima_Potencia_Terminal[6] = -Restricciones->Maxima_Potencia_Terminal[6];
	Restricciones->Minima_Potencia_Terminal[7] = -Restricciones->Maxima_Potencia_Terminal[7];
	Restricciones->Minima_Potencia_Terminal[8] = -Restricciones->Maxima_Potencia_Terminal[8];
	Restricciones->Minima_Potencia_Terminal[9] = -Restricciones->Maxima_Potencia_Terminal[9];
	Restricciones->Minima_Potencia_Terminal[10] = -Restricciones->Maxima_Potencia_Terminal[10];
	Restricciones->Minima_Potencia_Terminal[11] = -Restricciones->Maxima_Potencia_Terminal[11];

   //Se configurar que la maxima potencia que puede llegar de la red a la electrolinera y de la electrolinera a la 
   //red, es igual al maximo que pueden intercambiar la red y la electrolinera, (la potencia que puede llegar a la
   //electrolinera es la misma que puede salir.

	//Aqui el valor minimo es 0, no puede entrar potencia negativa.
	Restricciones->Maxima_Potencia_Red_Entrada = Restricciones->Maxima_Potencia_Red;
	Restricciones->Minima_Potencia_Red_Entrada = 0;


	Restricciones->Maxima_Potencia_Red_Salida = Restricciones->Maxima_Potencia_Red;
	Restricciones->Minima_Potencia_Red_Salida = 0;

	//Ahora se pasa a configurar la maxima potencia que puede intercambiarse por fase. Se establece que por cada
	//fase la potencia maxima que puede intercambiarse por cada fase es la maxima entre tres.
	Restricciones->Maxima_Potencia_Red_R = Restricciones->Maxima_Potencia_Red/3;
	Restricciones->Maxima_Potencia_Red_S = Restricciones->Maxima_Potencia_Red/3;
	Restricciones->Maxima_Potencia_Red_T = Restricciones->Maxima_Potencia_Red/3;

    //Del mismo modo la potencia que puede ceder la electrolinera a la red->
	Restricciones->Minima_Potencia_Red_R = -Restricciones->Maxima_Potencia_Red / 3;
	Restricciones->Minima_Potencia_Red_S = -Restricciones->Maxima_Potencia_Red / 3;
	Restricciones->Minima_Potencia_Red_T = -Restricciones->Maxima_Potencia_Red / 3;


	//Se hace lo mismo pero no con la variable de cuanto puede intercambiar la electrolinera y la red. Sino
	//con las variables que indican cuanto puede llegar de la red a la electrolinera (en ese sentido) y de la 
    //electrolinera a la red (en ese sentido) (son variables diferentes)

	Restricciones->Maxima_Potencia_Entrada_Red_R = Restricciones->Maxima_Potencia_Red / 3;
	Restricciones->Minima_Potencia_Entrada_Red_R = 0;
	Restricciones->Maxima_Potencia_Entrada_Red_S = Restricciones->Maxima_Potencia_Red / 3;
	Restricciones->Minima_Potencia_Entrada_Red_S = 0;
	Restricciones->Maxima_Potencia_Entrada_Red_T = Restricciones->Maxima_Potencia_Red / 3;
	Restricciones->Minima_Potencia_Entrada_Red_T = 0;

    //Ahora con la variable de la potencia que vierte la electrolinera a la red->
	Restricciones->Maxima_Potencia_Salida_Red_R = Restricciones->Maxima_Potencia_Red/3;
	Restricciones->Minima_Potencia_Salida_Red_R = 0;
	Restricciones->Maxima_Potencia_Salida_Red_S = Restricciones->Maxima_Potencia_Red/3;
	Restricciones->Minima_Potencia_Salida_Red_S = 0;
	Restricciones->Maxima_Potencia_Salida_Red_T = Restricciones->Maxima_Potencia_Red/3;
	Restricciones->Minima_Potencia_Salida_Red_T = 0;



}