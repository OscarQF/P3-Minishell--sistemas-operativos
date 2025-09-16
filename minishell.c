#define _POSIX_SOURCE
#include <stdio.h>
#include <string.h>
#include <limits.h>
#include <stdlib.h>
#include <sys/wait.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <limits.h>
#include <fcntl.h>
#include <unistd.h>
#include <signal.h>

#include "orden.h"
#include "procesos.h"

#define NUM_ORDENES_INTERNAS 3

static void ejecutar_pwd(const struct orden* orden);
static void ejecutar_cd(const struct orden* orden);
static void ejecutar_jobs(const struct orden* orden);
static void ejecutar_orden_externa(const struct orden* orden);

static const char* ordenes[NUM_ORDENES_INTERNAS+1] =
{
	"pwd",
	"cd",
	"jobs",
	"externa"
};

static void (*ejecutar[NUM_ORDENES_INTERNAS+1])(const struct orden* orden) =
{
	ejecutar_pwd,
	ejecutar_cd,
	ejecutar_jobs,
	ejecutar_orden_externa
};

static void ejecutar_pwd(const struct orden* orden)
{
}

static void ejecutar_cd(const struct orden* orden)
{
}

static void ejecutar_orden_externa(const struct orden* orden)
{
}

static void ejecutar_jobs(const struct orden* orden)
{
}

static void ejecutar_orden(const struct orden* orden)
{
	int i;

	i = 0;
	while (i < NUM_ORDENES_INTERNAS && strcmp(ordenes[i], orden->args[0]))
	{
		i++;
	}
	ejecutar[i](orden);
}

int main(const int argc, const char* argv[])
{
	const struct orden* actual;

	if (argc > 1)
	{
		fprintf(stderr, "Uso: %s\n", argv[0]);
		return -1;
	}
	actual = leer_orden ("mish>");
	while (strcmp("exit", actual->args[0]) != 0)
	{
		ejecutar_orden(actual);
		actual = leer_orden ("mish>");
	} 
	return 0;
}
