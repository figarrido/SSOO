typedef struct queue Queue;

Queue *create_queue();
void insert_process(Queue *, Process *);
Process *take_first_process(Queue *);
