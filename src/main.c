#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <signal.h>
#include "simulator.h"

Queue *processes;
FILE *process_file;

void handler(int signo) {
    handler_func();
    destroy_queue(processes);
    fclose(process_file);
    exit(1);
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

    signal(SIGINT, handler);

    process_file = fopen(argv[2], "r");

    Process         *process;
    char            process_name[256];
    int             priority,
                    initial_time,
                    N,
                    pid = 2;
    unsigned int    *running_time,
                    *waiting_time;

    processes = create_queue();

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
