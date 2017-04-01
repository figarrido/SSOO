#include "process.h"

typedef struct queue Queue;

Queue *create_queue();
void insert_process(Queue *, Process *);
Process *pop_first_process(Queue *);
Process *pop_random_process(Queue *);
Process *take_first_process(Queue *);
int length(Queue *);
void destroy_queue(Queue *);
void print_queue(Queue *);
