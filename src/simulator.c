#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include "scheduler.h"

#define TRUE 1
#define FALSE 0

Queue   *ready_queue,
        *waiting_list,
        *dead_list;

void simulate(Queue *processes, Scheduler scheduler, int quantum) {
    ready_queue = create_queue();
    waiting_list = create_queue();
    dead_list = create_queue();
    unsigned int    simulation_time = 0,
                    simulation_quantum;
    Process *process = NULL;

    while (TRUE) {
        printf("%d\n", simulation_time);
        // printf("=======================================================================\n");
        // if (process != NULL) printf("%d itereación, process: %s %d\n", simulation_time, process->name, get_running_time(process));
        put_process_in_ready(waiting_list, ready_queue, 0, WITH_WAITING_TIME);
        put_process_in_ready(processes, ready_queue, simulation_time, WITH_INITIAL_TIME);
        // printf("READY:\n");
        // print_queue(ready_queue);

        if (scheduler == FCFS || scheduler == RANDOM)
            fcfs_or_random(ready_queue, waiting_list, dead_list, &process, simulation_time, scheduler);
        else if (scheduler == ROUND_ROBIN)
            round_robin(ready_queue, waiting_list, dead_list, &process, &simulation_time, &simulation_quantum, quantum);
        else {
            printf("Posibles Schedulers: fcfs roundrobin random\n");
            exit(1);
        }
        // printf("\nWAITING:\n");
        // print_queue(waiting_list);
        // printf("\n");
        // printf("=======================================================================\n\n");

        // sleep(1);
        update_user_time(process);
        update_waiting_list(waiting_list, ready_queue);
        update_ready_queue(ready_queue);
        simulation_time++;
        simulation_quantum = (ROUND_ROBIN) ? simulation_quantum - 1 : 0;
        if (length(ready_queue) == 0 && length(waiting_list) == 0 && get_pid(process) == 1 && length(processes) == 0) {
            destroy_process(process);
            break;
        }
    }
    destroy_queue(ready_queue);
    destroy_queue(waiting_list);
    destroy_queue(dead_list);
}

int main(int argc, char const *argv[]) {
    int quantum;
    if (argc == 4) {
        quantum = atoi(argv[3]);
    }
    else if (argc == 3) {
        quantum = 3;
    }
    else {
        printf("USAGE: ./simulator <scheduler> <file> [<quantum>]\n");
        return -1;
    }

    FILE *process_file = fopen(argv[2], "r");

    Process         *process;
    char            process_name[256];
    int             priority,
                    initial_time,
                    N,
                    pid = 2;
    unsigned int    *running_time,
                    *waiting_time;

    Queue *processes = create_queue();

    while (fscanf(process_file, "%s ", process_name) != EOF) {
        fscanf(process_file, "%d %d %d", &priority, &initial_time, &N);
        running_time = (unsigned int *) malloc(sizeof(int) * N);
        waiting_time = (unsigned int *) malloc(sizeof(int) * (N - 1));

        for (int i = 0; i < 2 * N - 1; i++) {
            if (i % 2 == 0) { fscanf(process_file, "%d", running_time + (i / 2)); }
            else { fscanf(process_file, "%d", waiting_time + (i / 2)); }
        }

        fscanf(process_file, "/n");

        process = create_process(pid,
                                process_name,
                                priority,
                                initial_time,
                                N,
                                running_time,
                                waiting_time);

        insert_process(processes, process);
        pid++;
    }

    Scheduler scheduler;
    if (!strcmp(argv[1], "fcfs")) { scheduler = FCFS; }
    else if (!strcmp(argv[1], "roundrobin")) { scheduler = ROUND_ROBIN; }
    else if (!strcmp(argv[1], "random")) { scheduler = RANDOM; }

    simulate(processes, scheduler, quantum);

    destroy_queue(processes);
    fclose(process_file);
    return 0;
}
