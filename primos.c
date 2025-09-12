/**
 * Programa que calcula los primos entre 2 valores que recibe como parámetro.
 * El primer argumento es el valor mínimo a probar y el segundo el valor máxino
 * a probar.
 */

#include <stdlib.h>
#include <stdio.h>
#include <math.h>

int es_primo(int p)
{
	int f; /* factor por el que se va comprobando si p es divisible */

	for (f = 3; f * f <= p; f = f + 2)
	{
		if ((p % f) == 0)
		{
			return 0;
		}
	}
	return 1;
}

int main ( int argc, const char *argv[] )
{
	int MIN; /* primer posible primo */
	int MAX; /* último primo posible */
	int primo; /* recorre los posibles primos en el rango [MIN,MAX] */
	int cont; /* cuenta el numero de primos encontrados */

	if ( argc != 3 )
		return 1;
	MIN = atoi ( argv[1] );
	if ((MIN %2) == 0)
	{
		MIN = MIN + 1;
	}
	MAX = atoi ( argv[2] );
	if ((MAX %2) == 0)
	{
		MAX = MAX - 1;
	}
	if (MIN < 2)
	{
		return 1;
	}
	for (cont = 0, primo = MIN; primo <= MAX; primo = primo + 2)
	{
		if (es_primo(primo))
		{
			cont++;
		}
	}
	fprintf(stdout, "primos encontrados: %d\n", cont);
	return 0;
}
