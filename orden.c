#include <stdio.h>
#include <string.h>

#include "orden.h"

#define MAX_LIN 1024

const struct orden salir = {{"exit", NULL}, NULL, NULL, PRIMERO, NULL};
static struct orden actual;
static char lin[MAX_LIN]; /* la línea leída */
static char orden[MAX_LIN]; /* args apuntará aquí */
static int pos_actual; /* recorre la orden */
static int num_param; /* número de parámetros de la orden */
static int longitud; /* longitud de la línea leída */

static void borrar_orden_actual(void)
{
	pos_actual = 0;
	num_param=0;
	longitud = strlen(lin) - 1;
	lin[longitud]='\0';
	actual.entrada = NULL;
	actual.salida = NULL;
	actual.plano = PRIMERO;
	actual.texto_original = lin;
}

static void saltar_blancos(void)
{
	while (lin[pos_actual] == ' ' || lin[pos_actual] == '\t')
	{
		pos_actual++;
	}
}

static void final_palabra(void)
{
	while
	(	lin[pos_actual] != ' ' &&
		lin[pos_actual] != '\t' &&
		lin[pos_actual] != '&' &&
		lin[pos_actual] != '>' &&
		lin[pos_actual] != '<' &&
		lin[pos_actual] != '\0'
	)
	{
		pos_actual++;
	}
}

static char* obtener_cadena(void)
{
	char* pal = &orden[pos_actual];
	final_palabra();
	orden[pos_actual] = '\0';
	return pal;
}

static int param_fuera_de_sitio(void)
{
	return
		actual.entrada != NULL ||
		actual.salida != NULL ||
		actual.plano == SEGUNDO;
}

static void extraer_param(void)
{
	if (param_fuera_de_sitio())
	{
		fprintf(stderr, "Hay parámetros extra\n");
		actual.args[0] = NULL;
		return;
	}
	actual.args[num_param++] = obtener_cadena();
}

static void redirigir_entrada(void)
{
	pos_actual++;
	if (actual.entrada != NULL)
	{
		fprintf(stderr, "Más de una redirección de la entrada\n");
		actual.args[0] = NULL;
		return;
	}
	saltar_blancos();
	actual.entrada = obtener_cadena();
	if (strlen(actual.entrada) == 0)
	{
		fprintf(stderr, "Falta fichero de entrada\n");
		actual.args[0] = NULL;
	}
}

static void redirigir_salida(void)
{
	pos_actual++;
	if (actual.salida != NULL)
	{
		fprintf(stderr, "Más de una redirección de la salida\n");
		actual.args[0] = NULL;
		return;
	}
	saltar_blancos();
	actual.salida = obtener_cadena();
	if (strlen(actual.salida) == 0)
	{
		fprintf(stderr, "Falta fichero de salida\n");
		actual.args[0] = NULL;
	}
}

static void anotar_segundo_plano(void)
{
	if (actual.plano == SEGUNDO)
	{
		fprintf(stderr, "Más de un '&' no permitido\n");
		actual.args[0] = NULL;
		return;
	}
	actual.plano = SEGUNDO;
	pos_actual++;
}

static void extraer_palabra(void)
{
	saltar_blancos();
	switch (lin[pos_actual])
	{
		case '<':
			redirigir_entrada(); break;
		case '>':
			redirigir_salida(); break;
		case '&':
			anotar_segundo_plano(); break;
		case '\0':
			break;
		default:
			extraer_param();
	}
}

static int seguir_analizando(void)
{
	return
		pos_actual < longitud &&
		actual.args[0] != NULL &&
		num_param < MAX_ARGS-2;
}

static void analizar_orden(void)
{
	do
	{
		extraer_palabra();
	} while (seguir_analizando());
	if (num_param >= MAX_ARGS-2)
	{
		fprintf(stderr, "Demasiados argumentos\n");
		actual.args[0] = NULL;
	}
	actual.args[num_param] = NULL;
}

static int demasiado_larga(void)
{
	const int lon = strlen(lin);
	const int larga = !feof(stdin) && (lin[lon-1] != '\n');
	if (larga)
	{
		fprintf(stderr, "orden demasiado larga\n" );
		while (fgetc(stdin)!='\n');
	}
	return larga;
}

static void leer_lin(const char* indicador)
{
	char* err;
	do
	{
		fprintf(stdout, "%s ", indicador);
		err = fgets(lin, MAX_LIN, stdin);
	} while (err != NULL && demasiado_larga());
	strncpy(orden, lin, MAX_LIN);
}

const struct orden* leer_orden(const char* indicador)
{
	do
	{
		leer_lin(indicador);
		if (feof(stdin))
		{
			return &salir;
		}
		else
		{
			borrar_orden_actual();
			analizar_orden();
		}
	} while (actual.args[0] == NULL);
	return &actual;
}
