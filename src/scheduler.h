#include "queue.h"

typedef enum { WITH_INITIAL_TIME, WITH_WAITING_TIME } Comparison;
typedef enum { FCOME_FSERVED, ROUND_ROBIN, RANDOM } Scheduler;

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
                                int);

void FCFS(  Queue *,
            Queue *,
            Queue *,
            Process **,
            int);

void round_robin(   Queue *,
                    Queue *,
                    Queue *,
                    Process **,
                    int *,
                    int *,
                    int);

int calculate_quantum(Process *, int);
