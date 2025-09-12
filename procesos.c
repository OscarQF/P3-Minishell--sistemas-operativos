#include <string.h>
#include <stdio.h>

#include "procesos.h"

#define MAX_TEXTO 60

enum Estado {EJECUTANDOSE, TERMINADO, CENTINELA, LIBRE};

struct proceso
{
	char texto_original[MAX_TEXTO];
	pid_t pid;
	enum Estado estado;
	int siguiente;
	int anterior;
};

#define LIBRES -2
#define OCUPADOS -1

static struct proceso tabla_procesos[MAX_PROCESOS+2] =
{
	{"", 0, CENTINELA, 0, MAX_PROCESOS - 1},
	{"", 0, CENTINELA, OCUPADOS, OCUPADOS},
	{"", 0, LIBRE, 1, LIBRES},
	{"", 0, LIBRE, 2, 1},
	{"", 0, LIBRE, 3, 2},
	{"", 0, LIBRE, 4, 3},
	{"", 0, LIBRE, LIBRES, 4}
};

static struct proceso* const procesos = tabla_procesos + 2;

static int localizar_proceso(pid_t pid)
{
	int actual;
	actual = procesos[OCUPADOS].siguiente;
	while (actual != OCUPADOS && procesos[actual].pid != pid)
	{
		actual = procesos[actual].siguiente;
	}
	return actual;
}

static void cambiar_estado(pid_t pid, enum Estado estado)
{
	const int cual = localizar_proceso(pid);
	procesos[cual].estado = estado;
}

extern void terminar_proceso(pid_t pid)
{
	cambiar_estado(pid, TERMINADO);
}

static void sacar(int cual)
{
	const int anterior = procesos[cual].anterior;
	const int siguiente = procesos[cual].siguiente;
	procesos[anterior].siguiente = siguiente;
	procesos[siguiente].anterior = anterior;
}

static void meter(int cual, int donde)
{
	const int anterior = procesos[donde].anterior;
	procesos[cual].anterior = anterior;
	procesos[cual].siguiente = donde;
	procesos[anterior].siguiente = cual;
	procesos[donde].anterior = cual;
}

extern void eliminar_proceso(pid_t pid)
{
	const int cual = localizar_proceso(pid);
	sacar(cual);
	meter(cual, LIBRES);
}

extern int num_procesos(void)
{
	int cont;
	int i;
	cont = 0;
	i = OCUPADOS;
	while (procesos[i].siguiente != OCUPADOS)
	{
		cont = cont + 1;
		i = procesos[i].siguiente;
	}
	return cont;
}

extern void anotar_proceso(const char texto_original[], pid_t pid)
{
	const int cual = procesos[LIBRES].siguiente;
	sacar(cual);
	strncpy(procesos[cual].texto_original, texto_original, MAX_TEXTO-1);
	procesos[cual].texto_original[MAX_TEXTO-1] = '\0';
	procesos[cual].pid = pid;
	procesos[cual].estado = EJECUTANDOSE;
	meter(cual, OCUPADOS);
}

static const char* texto_estado(enum Estado estado)
{
	if (estado == EJECUTANDOSE)
	{
		return "R";
	}
	else if (estado == TERMINADO)
	{
		return "T";
	}
	else
	{
		return "-";
	}
}

static void mostrar_proceso(int num_orden, int cual)
{
	const pid_t pid = procesos[cual].pid;
	const char* texto_original = procesos[cual].texto_original;
	const char* estado = texto_estado(procesos[cual].estado);
	printf("[%d]\t%s\t%8d\t%s\n", num_orden, estado, pid, texto_original);
}

extern void mostrar_procesos_y_eliminar_terminados(void)
{
	int i;
	int cont;
	cont = 1;
	i = procesos[OCUPADOS].siguiente;
	while (i != OCUPADOS)
	{
		mostrar_proceso(cont, i);
		cont = cont + 1;
		{
			const int cual = i;
			i = procesos[i].siguiente;
			if (procesos[cual].estado == TERMINADO)
			{
				sacar(cual);
				meter(cual, LIBRES);
			}
		}
	}
}

extern void mostrar_y_eliminar_procesos_terminados(void)
{
	int i;
	int cont;
	cont = 1;
	i = procesos[OCUPADOS].siguiente;
	while (i != OCUPADOS)
	{
		const int cual = i;
		i = procesos[i].siguiente;
		if (procesos[cual].estado == TERMINADO)
		{
			mostrar_proceso(cont, cual);
			sacar(cual);
			meter(cual, LIBRES);
		}
		cont = cont + 1;
	}
}

