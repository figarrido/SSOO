#include <stdio.h>
#include <stdlib.h>

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
    char process_name[256];
    int priority;
    int initial_time;
    int N;

    fscanf(process_file, "%s %d %d %d", process_name, &priority, &initial_time, &N);
    int *running_time = (int *) malloc(sizeof(int) * N);
    int *waiting_time = (int *) malloc(sizeof(int) * N);

    for (int i = 0; i < 2 * N; i++) {
        if (i % 2 == 0) { fscanf(process_file, "%d", running_time + (i / 2)); }
        else { fscanf(process_file, "%d", waiting_time + (i / 2)); }
    }


    free(running_time);
    free(waiting_time);
    fclose(process_file);
    return 0;
}
