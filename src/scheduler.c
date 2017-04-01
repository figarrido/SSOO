#include <unistd.h>
#include <stdio.h>
#include "scheduler.h"

#define TRUE 1
#define FALSE 0

int process_in_time(Process *process, int t, Comparison comparison) {
    switch (comparison) {
        case WITH_INITIAL_TIME:
            return process->initial_time == t;
        case WITH_WAITING_TIME:
            return get_waiting_time(process) == 0;
    }
    return 0;
}

void change_process_if_ready(   Queue *ready_queue,
                                Queue *waiting_list,
                                Queue *dead_list,
                                Process **process_add,
                                int t,
                                Scheduler scheduler) {

    if (length(ready_queue) == 0) {
        *process_add = create_idle_process(t);
    }
    else {
        *process_add = (scheduler != RANDOM) ? pop_first_process(ready_queue)
                                            : pop_random_process(ready_queue);
    }

    update_state(*process_add, RUNNING);
    while (get_running_time(*process_add) == 0) {
        printf("Entrando en change\n");
        if (!last_execution(*process_add)) {
            update_state(*process_add, WAITING);
            insert_process(waiting_list, *process_add);
        }
        else {
            update_state(*process_add, DEAD);
            insert_process(dead_list, *process_add);
        }
        if (length(ready_queue) == 0) {
            *process_add = create_idle_process(t);
        }
        else {
            *process_add = (scheduler != RANDOM) ? pop_first_process(ready_queue)
                                                : pop_random_process(ready_queue);
        }
    }
}

void put_process_in_ready(  Queue *processes,
                            Queue *ready,
                            unsigned int t,
                            Comparison comparison) {
    int index = 0;
    while (index < length(processes)) {
        Process *process = pop_first_process(processes);
        if (process_in_time(process, t, comparison)) {
            insert_process(ready, process);
            update_state(process, READY);
        }
        else { insert_process(processes, process); index++; }
    }
}

void update_waiting_list(Queue *waiting_list, Queue *ready_queue) {
    int index = 0;
    while (index < length(waiting_list)) {
        Process *process = pop_first_process(waiting_list);
        update_idle_time(process, TRUE);
        if (get_waiting_time(process) == 0) {
            insert_process(ready_queue, process);
            update_state(process, READY);
        }
        else { insert_process(waiting_list, process); index++; }
    }
}

void update_ready_queue(Queue *ready_queue) {
    int index = 0;
    while (index < length(ready_queue)) {
        Process *process = pop_first_process(ready_queue);
        update_idle_time(process, FALSE);
        insert_process(ready_queue, process);
        index++;
    }
}

void fcfs_or_random(Queue *ready_queue,
                    Queue *waiting_list,
                    Queue *dead_list,
                    Process **process,
                    int simulation_time,
                    Scheduler scheduler) {

    if (*process == NULL) {
        change_process_if_ready(ready_queue, waiting_list, dead_list, process, simulation_time, scheduler);
    }
    else {
        if (get_running_time(*process) == 0) {
            if (get_pid(*process) == 1) {
                destroy_process(*process);
            }
            else if (!last_execution(*process)) {
                update_state(*process, WAITING);
                insert_process(waiting_list, *process);
            }
            else {
                update_state(*process, DEAD);
                insert_process(dead_list, *process);
            }
            change_process_if_ready(ready_queue, waiting_list, dead_list, process, simulation_time, scheduler);
        }
    }
}

int calculate_quantum(Process *process, int quantum) {
    return (get_priority(process) > 32) ? quantum * get_priority(process) / 32
                        : quantum * (get_priority(process) + 30) / 62;
}

void round_robin(   Queue *ready_queue,
                    Queue *waiting_list,
                    Queue *dead_list,
                    Process **process,
                    unsigned int *simulation_time,
                    unsigned int *simulation_quantum,
                    int quantum) {

    if (*process == NULL) {
        change_process_if_ready(ready_queue, waiting_list, dead_list, process, *simulation_time, ROUND_ROBIN);
        *simulation_quantum = calculate_quantum(*process, quantum);
    }
    else {
        if (*simulation_quantum == 0 && get_running_time(*process) == 0) {
            if (get_pid(*process) == 1) {
                destroy_process(*process);
            }
            else if (!last_execution(*process)) {
                update_state(*process, WAITING);
                insert_process(waiting_list, *process);
            }
            else {
                update_state(*process, DEAD);
                insert_process(dead_list, *process);
            }
            change_process_if_ready(ready_queue, waiting_list, dead_list, process, *simulation_time, ROUND_ROBIN);
            *simulation_quantum = calculate_quantum(*process, quantum);
        }
        else if (*simulation_quantum == 0 && get_running_time(*process) != 0) {
            update_state(*process, READY);
            insert_process(ready_queue, *process);
            change_process_if_ready(ready_queue, waiting_list, dead_list, process, *simulation_time, ROUND_ROBIN);
            *simulation_quantum = calculate_quantum(*process, quantum);
        }
        else if (*simulation_quantum != 0 && get_running_time(*process) == 0) {
            if (!last_execution(*process)) {
                update_state(*process, WAITING);
                insert_process(waiting_list, *process);
            }
            else {
                update_state(*process, DEAD);
                insert_process(dead_list, *process);
            }
            change_process_if_ready(ready_queue, waiting_list, dead_list, process, *simulation_time, ROUND_ROBIN);
            *simulation_quantum = calculate_quantum(*process, quantum);
        }
    }
}
