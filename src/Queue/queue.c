#include <stdlib.h>
#include "queue.h"
#include "../Process.h"

typedef struct node {
    Process     *process;
    struct node *next;
} Node;

typedef struct queue {
    Node    *head,
            *tail:
    int     length;
} Queue;

Queue *create_queue() {
    Queue *queue = (Queue *) malloc(sizeof(Queue));
    queue->head = NULL;
    queue->tail = NULL;
    queue->length = 0;
    return queue;
}

void insert_process(Queue *queue, Process *process) {
    Node *new_node = (Node *) malloc(sizeof(Node));
    new_node->process = process;
    new_node->next = NULL;
    if (queue->length == 0) {
        queue->head = new_node;
        queue->tail = queue->head;
    }
    else {
        queue->tail->next = new_node;
        queue->tail = new_node;
    }
    queue->length++;
}

Process *take_first_process(Queue *queue) {
    Node *node = queue->head;
    Process *process = node->process;
    queue->head = node->next;
    free(node);
    queue--;
    return process;
}
