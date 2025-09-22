#ifndef _PROCESOS_H_
#define _PROCESOS_H_

#include <unistd.h>

/* Máximo número de procesos activos que permite minishell */
#define MAX_PROCESOS 5

/**
 * Marca el proceso cuyo PID recibe como parámetro como terminado.
 */
extern void terminar_proceso(pid_t pid);

/**
 * Elimina el proceso cuyo PID recibe como parámetro de la lista de
 * procesos activos.
 */
extern void eliminar_proceso(pid_t pid);

/**
 * Devuelve el número de procesos activos.
 */
extern int num_procesos(void);

/**
 * Anota un nuevo proceso en la lista de procesos activos, con su PID y
 * el texto de la orden que ejecuta, poniéndole como estado ejecutándose.
 */
extern void anotar_proceso(const char texto_original[], pid_t pid);

/**
 * Muestra todos los procesos que hay activos, indicando el número de
 * tarea, su estado, su pid y la orden correspondiente. Cuando un proceso
 * ha terminado, lo borra una vez mostrado.
 */
extern void mostrar_procesos_y_eliminar_terminados(void);

/**
 * Muestra todos los procesos que están terminados y los elimina de la
 * lista de procesos activos. No procesa los que aún no han terminado.
 */
extern void mostrar_y_eliminar_procesos_terminados(void);

#endif
