#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include "simulator.h"

#define TRUE 1
#define FALSE 0
#define RUN_SLOW FALSE

unsigned int    simulation_time;
Queue           *ready_queue,
                *waiting_list,
                *dead_list;
Process         *global_process;

void statistics() {
    printf("Terminaron %d procesos.\n", length(dead_list));
    printf("La simulación duró %d ciclos.\n", simulation_time);
    printf("\nEstadísticas por proceso terminado:\n");
    Process *process;
    int selected, acc_selected = 0,
        blocked, acc_blocked = 0,
        turnaround, acc_turnaround = 0,
        response_time, acc_response_time = 0,
        waiting_time, acc_waiting_time = 0,
        n_processes = length(dead_list);
    while (length(dead_list) != 0) {
        process = pop_first_process(dead_list);
        printf("\t%s\n", get_name(process));
        selected = get_selected(process); acc_selected += selected;
        blocked = get_blocked(process); acc_blocked += blocked;
        turnaround = get_turnaround(process); acc_turnaround += turnaround;
        response_time = get_response_time(process); acc_response_time += response_time;
        waiting_time = get_idle_time(process); acc_waiting_time += waiting_time;
        printf("\t\tSeleccionado para CPU:\t%d veces\n", selected);
        printf("\t\tBloqueado:\t\t%d veces\n", blocked);
        printf("\t\tTurnaround time:\t%d ciclos\n", turnaround);
        printf("\t\tResponse time:\t\t%d ciclos\n", response_time);
        printf("\t\tWaiting time:\t\t%d ciclos\n", waiting_time);
    }
    printf("\nEstadísticas promedio de la simulación:\n");
    printf("\tSeleccionado para CPU:\t%.2f veces\n", (float) acc_selected / n_processes);
    printf("\tBloqueado:\t\t%.2f veces\n", (float) acc_blocked / n_processes);
    printf("\tTurnaround time:\t%.2f ciclos\n", (float) acc_turnaround / n_processes);
    printf("\tResponse time:\t\t%.2f ciclos\n", (float) acc_response_time / n_processes);
    printf("\tWaiting time:\t\t%.2f ciclos\n", (float) acc_waiting_time / n_processes);
}

void simulate(Queue *processes, Scheduler scheduler, int quantum) {
    ready_queue = create_queue();
    waiting_list = create_queue();
    dead_list = create_queue();
    unsigned int simulation_quantum;
    simulation_time = 0;
    global_process = NULL;

    while (TRUE) {
        printf("\nTIEMPO: %d\n", simulation_time);
        if (global_process != NULL) printf("PROCESO ACTIVO: %s (%d/%d)\n", get_name(global_process), get_index(global_process) + 1, get_intervals(global_process));
        put_process_in_ready(waiting_list, ready_queue, 0, WITH_WAITING_TIME);
        put_process_in_ready(processes, ready_queue, simulation_time, WITH_INITIAL_TIME);

        if (scheduler == FCFS || scheduler == RANDOM)
            fcfs_or_random(ready_queue, waiting_list, dead_list, &global_process, simulation_time, scheduler);
        else if (scheduler == ROUND_ROBIN)
            round_robin(ready_queue, waiting_list, dead_list, &global_process, &simulation_time, &simulation_quantum, quantum);
        else {
            printf("Posibles schedulers: fcfs roundrobin random\n");
            exit(1);
        }

        sleep(RUN_SLOW);
        update_user_time(global_process);
        update_waiting_list(waiting_list, ready_queue);
        update_ready_queue(ready_queue);
        simulation_time++;
        simulation_quantum = (ROUND_ROBIN) ? simulation_quantum - 1 : 0;
        if (length(ready_queue) == 0 && length(waiting_list) == 0 && get_pid(global_process) == 1 && length(processes) == 0) {
            destroy_process(global_process);
            break;
        }
    }
    printf("\n----------------FIN----------------\n\n");
    statistics();

    destroy_queue(ready_queue);
    destroy_queue(waiting_list);
    destroy_queue(dead_list);
}

void handler_func() {
    printf("\n----------------TÉRMINO FORZADO----------------\n\n");
    printf("Terminaron %d procesos.\n", length(dead_list));
    printf("La simulación fue interrumpida a los %d ciclos.\n", simulation_time);

    Process *process;
    if (length(ready_queue) != 0 || length(waiting_list) != 0 || get_pid(global_process) != 1) {
        printf("\nEstadísticas de procesos no terminados:\n");
    }
    if (get_pid(global_process) != 1) {
        printf("\t%s\n", get_name(global_process));
        printf("\t\tIntervalos:\t\t%d de %d\n", get_index(global_process) + 1, get_n_processes(global_process));
        printf("\t\tSeleccionado para CPU:\t%d veces\n", get_selected(global_process));
        printf("\t\tBloqueado:\t\t%d veces\n", get_blocked(global_process));
        printf("\t\tTurnaround time:\t- ciclos\n");
        printf("\t\tResponse time:\t\t%d ciclos\n", get_response_time(global_process));
        printf("\t\tWaiting time:\t\t%d ciclos\n", get_idle_time(global_process));
    }
    while (length(ready_queue) != 0) {
        process = pop_first_process(ready_queue);
        printf("\t%s\n", get_name(process));
        printf("\t\tIntervalos:\t%d de %d\n", get_index(process) + 1, get_n_processes(process));
        printf("\t\tSeleccionado para CPU:\t%d veces\n", get_selected(process));
        printf("\t\tBloqueado:\t\t%d veces\n", get_blocked(process));
        printf("\t\tTurnaround time:\t- ciclos\n");
        printf("\t\tResponse time:\t\t%d ciclos\n", get_response_time(process));
        printf("\t\tWaiting time:\t\t%d ciclos\n", get_idle_time(process));
    }
    while (length(waiting_list) != 0) {
        process = pop_first_process(waiting_list);
        printf("\t%s\n", get_name(process));
        printf("\t\tIntervalos:\t%d de %d\n", get_index(process) + 1, get_n_processes(process));
        printf("\t\tSeleccionado para CPU:\t%d veces\n", get_selected(process));
        printf("\t\tBloqueado:\t\t%d veces\n", get_blocked(process));
        printf("\t\tTurnaround time:\t- ciclos\n");
        printf("\t\tResponse time:\t\t%d ciclos\n", get_response_time(process));
        printf("\t\tWaiting time:\t\t%d ciclos\n", get_idle_time(process));
    }

    if (length(dead_list) != 0) {
        printf("\nEstadísticas de procesos terminados:\n");
    }
    int selected, acc_selected = 0,
        blocked, acc_blocked = 0,
        turnaround, acc_turnaround = 0,
        response_time, acc_response_time = 0,
        waiting_time, acc_waiting_time = 0,
        n_processes = length(dead_list);
    while (length(dead_list) != 0) {
        process = pop_first_process(dead_list);
        printf("\t%s\n", get_name(process));
        selected = get_selected(process); acc_selected += selected;
        blocked = get_blocked(process); acc_blocked += blocked;
        turnaround = get_turnaround(process); acc_turnaround += turnaround;
        response_time = get_response_time(process); acc_response_time += response_time;
        waiting_time = get_idle_time(process); acc_waiting_time += waiting_time;
        printf("\t\tSeleccionado para CPU:\t%d veces\n", selected);
        printf("\t\tBloqueado:\t\t%d veces\n", blocked);
        printf("\t\tTurnaround time:\t%d ciclos\n", turnaround);
        printf("\t\tResponse time:\t\t%d ciclos\n", response_time);
        printf("\t\tWaiting time:\t\t%d ciclos\n", waiting_time);
    }
    if (n_processes != 0)
    {
        printf("\nEstadísticas promedio de procesos terminados de la simulación:\n");
        printf("\tSeleccionado para CPU:\t%.2f veces\n", (float) acc_selected / n_processes);
        printf("\tBloqueado:\t\t%.2f veces\n", (float) acc_blocked / n_processes);
        printf("\tTurnaround time:\t%.2f ciclos\n", (float) acc_turnaround / n_processes);
        printf("\tResponse time:\t\t%.2f ciclos\n", (float) acc_response_time / n_processes);
        printf("\tWaiting time:\t\t%.2f ciclos\n", (float) acc_waiting_time / n_processes);
    }


    destroy_process(global_process);
    destroy_queue(ready_queue);
    destroy_queue(waiting_list);
    destroy_queue(dead_list);
}
