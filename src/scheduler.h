#include "queue.h"

typedef enum { WITH_INITIAL_TIME, WITH_WAITING_TIME } Comparison;
typedef enum { FCFS, ROUND_ROBIN, RANDOM } Scheduler;

void put_process_in_ready(  Queue *,
                            Queue *,
                            unsigned int,
                            Comparison);
void update_waiting_list(Queue *, Queue *);
void update_ready_queue(Queue *);
void change_process_if_ready(   Queue *,
                                Queue *,
                                Queue *,
                                Process **,
                                int,
                                Scheduler);

void fcfs_or_random(Queue *,
                    Queue *,
                    Queue *,
                    Process **,
                    int,
                    Scheduler);

void round_robin(   Queue *,
                    Queue *,
                    Queue *,
                    Process **,
                    unsigned int *,
                    unsigned int *,
                    int);

int calculate_quantum(Process *, int);
