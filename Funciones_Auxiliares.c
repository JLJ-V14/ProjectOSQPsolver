
#include "time.h"
int Comprobar_Orden_Fechas(const struct tm Fecha_1, const struct tm Fecha_2) {
	//Este subprograma tiene la funcion de comprobar si una fecha es anterior posterior o igual a la otra
	//Se devuelve -1 si la fecha 1 es anterior a la fecha 2
	//Se devuelve  0 si la fecha 1 es igual    a la fecha 2
	//Se devuelve  1 si la fecha 1 es posterior a la fecha 2

	struct tm temp1 = Fecha_1;
	struct tm temp2 = Fecha_2;
	time_t t1, t2;

	t1 = mktime(&temp1);
	t2 = mktime(&temp2);

	if (t1 < t2) {
		return -1;
	}
	else if (t1 == t2) {
		return 0;
	}
	else {
		return 1;
	}
}