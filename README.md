# Sistemas Operativos y Redes
## Tarea 1

> **Integrantes**: Felipe Garrido

### USAGE

`make`

`./simulator <scheduler> <file> <quantum>`

* `scheduler`:
    * `fcfs` (first come first served)
    * `roundrobin`
    * `random`
* `file`: file format
    * _name priority initial-time N A0 B0 A1 B1 A2 ... B(N-1) AN_
* `quantum`: time to default priority (only with `roundrobin`)

En el archivo `simulator.c`, linea 8, está la línea `#define RUN_SLOW` que con el valor `TRUE` realiza la simulación utilizando un `sleep` de un segundo por cada iteración y con el valor `FALSE` realiza la simulación sin `sleep`.  

### Read Me

* Hice una lista ligada como cola de procesos.
* Los procesos son modelados como un `struct` con todos los datos (incluyendo estadísticas).
* Existen 4 listas con procesos:
    * `processes`: guarda todos los procesos del archivo.
    * `ready_queue`: guarda los procesos que ya pasaron su tiempo de inicio o que terminaron sus tiempos de espera o fueron interrumpidos por el scheduler.
    * `waiting_list`: guarda los procesos que están esperando según lo especificado en el archivo.
    * `dead_list`: guarda los procesos que ya terminaron.
* Existe un ciclo "sin término" que simula cada ciclo de la ejecución de los procesos, este termina cuando todos los procesos están en la lista `dead_list`.
* Si es que no hay procesos en la cola `ready_queue` se genera el proceso `idle` que por defecto dura 1 ciclo y tiene prioridad 1.
* Para el scheduler `random` utilicé la función `time` y apliqué el operador `%` con el largo de la cola `ready_queue` para recibir el índice "aleatorio" de la cola.
* Para las estadísticas asumí que para la cantidad de bloqueos se suman las interrupciones que realiza el scheduler (solo en `roundrobin`).

### Output

En el output se ve el número de simulación en la que está la simulación y abajo el procesos que está actualmente en la CPU y la cantidad de intervalos completados comparado con los totales (completados/totales). Luego tabulado se presentan los cambios que se realizan durante el ciclo.
La manera en que se presenta la información se encuentra entre `[]` y puede tener los siguientes valores:

* `[NUEVO] nombre_del_proceso`: cuando el proceso valor del tiempo inicial que está en la cola `processes` tiene el mismo valor que el número de la simulación.

* `[CAMBIO DE ESTADO] nombre_del_proceso {ESTADO_ANTERIOR -> NUEVO_ESTADO}`: cada vez que el proceso cambia de estado entre los posibles estados (`READY`, `WAITING`, `RUNNING`).

* `[ELECCIÓN] nombre_del_proceso`: cuando un nuevo proceso ingresa en la CPU.

* `[TERMINADO] nombre_del_proceso`: cuando un proceso termina.

Dentro de las estadísticas no se muestran las estadísticas del proceso `idle`.
