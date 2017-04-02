#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include "process.h"

#define TRUE 1
#define FALSE 0

Process *create_process(int pid,
                        char *name,
                        int priority,
                        int initial_time,
                        int N,
                        unsigned int *running_array,
                        unsigned int *waiting_array) {
    Process *process = (Process *) malloc(sizeof(Process));
    process->pid = pid;

    strcpy(process->name, name);

    process->priority = priority;
    process->initial_time = initial_time;
    process->state = NON_READY;
    process->n_processes = N;
    process->running_array = running_array;
    process->waiting_array = waiting_array;
    process->index = 0;
    process->user_time = 0;
    process->idle_time = 0;
    process->selected = 0;
    process->blocked = 0;
    process->response_time = 0;
    process->responded = FALSE;
    return process;
}

Process *create_idle_process(int t) {
    unsigned int *running_array = (unsigned int *) malloc(sizeof(int));
    unsigned int *waiting_array = (unsigned int *) malloc(sizeof(int));
    running_array[0] = 1;
    return create_process(  1,
                            "idle",
                            1,
                            t,
                            1,
                            running_array,
                            waiting_array
                        );
}

int get_pid(Process *process) {
    return process->pid;
}

int get_priority(Process *process) {
    return process->priority;
}

int get_intervals(Process *process) {
    return process->n_processes;
}

int get_index(Process *process) {
    return process->index;
}

char *get_name(Process *process) {
    return process->name;
}

int get_selected(Process *process) {
    return process->selected;
}
int get_blocked(Process *process) {
    return process->blocked;
}
int get_turnaround(Process *process) {
    return process->user_time + process->idle_time;
}
int get_response_time(Process *process) {
    return process->response_time;
}
int get_idle_time(Process *process) {
    return process->idle_time;
}

int get_n_processes(Process *process) {
    return process->n_processes;
}

int get_running_time(Process *process) {
    return process->running_array[process->index];
}

void update_user_time(Process *process) {
    process->user_time++;
    process->running_array[process->index]--;
}

int get_waiting_time(Process *process) {
    return process->waiting_array[process->index];
}

void update_idle_time(Process *process, int waiting) {
    process->idle_time++;
    if (waiting) { process->waiting_array[process->index]--; }
    if (!(process->responded)) { process->response_time++; }
}

State get_state(Process *process) {
    return process->state;
}

void update_state(Process *process, State state) {

    if (process->state == NON_READY && state == READY) {
        printf("\t[NUEVO] %s\n", get_name(process));
    }
    else if (process->state == READY && state == RUNNING) {
        process->responded = TRUE;
        process->selected++;
        printf("\t[CAMBIO DE ESTADO] %s  {READY   ->  RUNNING}\n", get_name(process));
    }
    else if (process->state == RUNNING && state == WAITING) {
        printf("\t[CAMBIO DE ESTADO] %s  {RUNNING ->  WAITING}\n", get_name(process));
        process->blocked++;
    }
    else if (process->state == RUNNING && state == DEAD) {
        printf("\t[TERMINADO] %s\n", get_name(process));
    }
    else if (process->state == WAITING && state == READY) {
        printf("\t[CAMBIO DE ESTADO] %s  {WAITING ->  READY}\n", get_name(process));
        process->index++;
    }
    else if (process->state == RUNNING && state == READY) {
        printf("\t[CAMBIO DE ESTADO] %s  {RUNNING ->  READY}\n", get_name(process));
        process->blocked++;
    }

    process->state = state;
}

int last_execution(Process *process) {
    return (process->index + 1 == process->n_processes) ? 1 : 0;
}

void destroy_process(Process *process) {
    free(process->running_array);
    free(process->waiting_array);
    free(process);
}
