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

#define PROMPT "mish>"

#define NUM_ORDENES_INTERNAS 3
#define NUM_ORDENES_TOTAL (NUM_ORDENES_INTERNAS + 1)

static void ejecutar_pwd(const struct orden* orden);
static void ejecutar_cd(const struct orden* orden);
static void ejecutar_jobs(const struct orden* orden);
static void ejecutar_orden_externa(const struct orden* orden);

static const char* ordenes[NUM_ORDENES_TOTAL] =
{
	"pwd",
	"cd",
	"jobs",
	"externa"
};

static void (*ejecutar[NUM_ORDENES_TOTAL])(const struct orden* orden) =
{
	ejecutar_pwd,
	ejecutar_cd,
	ejecutar_jobs,
	ejecutar_orden_externa
};

static void ejecutar_pwd(const struct orden* orden)
{
	{
	char cwd[PATH_MAX];
	if(orden->args[1] == NULL){
		if(orden->entrada == NULL){
			if(orden->salida == NULL){
				if(orden->plano == PRIMERO){
					if (getcwd(cwd, sizeof(cwd)) == NULL){
        			perror("pwd");
    				}else{
    					printf("%s\n", cwd);
						}

					}else{perror("pwd");
					  return;}
			}else{perror("pwd");
			  return;}

		}else {perror("pwd");
		 return;}
	}else {perror("pwd");
	       return;}

}
}

static void ejecutar_cd(const struct orden* orden)
{
	const char* home;
	if(orden->plano==SEGUNDO){
		fprintf(stderr,"cd: no se puede ejecutar en segundo plano\n");
		return;
	}

	if(orden ->args[1] !=NULL){
		if(orden->args[2]!=NULL){
			 fprintf(stderr, "cd: demasiados argumentos\n");
			return;
		}else{
		
			if(chdir(orden->args[1])!=0){
				perror("cd");
				return;
			}
		}
		
	    
	
	}else{
	home=getenv("HOME");
		if(home==NULL){
			perror("cd");
			return;
		}else{
			if(chdir(home)!=0){
				perror("cd");
				return;
			}
		}
	}
}


static void ejecutar_orden_externa(const struct orden* orden)
{
	pid_t pid = fork();

	 if (!orden || !orden->args || !orden->args[0]) {
        fprintf(stderr, "ejecutar_orden_externa: comando vacío\n");
        return;
    }

    if (pid <0) {
        perror("fork");
        return;
    }

    if (pid == 0) {
		int entrada;
		int salida;
		if(orden->entrada != NULL){
			entrada= open(orden->entrada,O_RDONLY);
			if(entrada < 0){
				perror(orden->entrada);
				exit(2);
			}
			if(dup2(entrada,STDIN_FILENO)< 0){
				perror("dup2 orden->entrada");
				close(entrada);
				exit(2);
			}
			close(entrada);
		}


		if(orden->salida!= NULL){
			salida= open(orden->salida,O_WRONLY | O_CREAT | O_TRUNC, 0666);
			if(salida < 0){
				perror(orden->salida);
				exit(2);
			}
			if(dup2(salida,STDOUT_FILENO)< 0){
				perror("dup2 orden->salida");
				close(salida);
				exit(2);
			}
			close(salida);
		}

		

        execvp(orden->args[0],orden->args);
        perror("execvp");
		exit(3);


		
    }else{
		int status;
		pid_t padre;
		while (waitpid(padre, &status, 0) == -1) { 
    	perror("waitpid");
		return;
		}
	}
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
	actual = leer_orden (PROMPT);
	while (strcmp("exit", actual->args[0]) != 0)
	{
		ejecutar_orden(actual);
		actual = leer_orden (PROMPT);
	} 
	return 0;
}
