# Makefile clásico para el proyecto "mish"

CC = gcc
CFLAGS = -Wall -Wextra -O2 -Wuninitialized

OBJS = minishell.o orden.o procesos.o

all: mish primos

mish: $(OBJS)
	$(CC) $(CFLAGS) -o mish $(OBJS)

primos: primos.o
	$(CC) $(CFLAGS) -o primos primos.o

minishell.o: minishell.c orden.h procesos.h
	$(CC) $(CFLAGS) -c minishell.c

orden.o: orden.c orden.h
	$(CC) $(CFLAGS) -c orden.c

procesos.o: procesos.c procesos.h
	$(CC) $(CFLAGS) -c procesos.c

primos.o: primos.c
	$(CC) $(CFLAGS) -c primos.c

clean:
	rm -f *.o mish

