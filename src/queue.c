#include <stdlib.h>
#include <stdio.h>
#include <time.h>
#include "queue.h"

typedef struct node {
    Process     *process;
    struct node *next;
} Node;

typedef struct queue {
    Node    *head,
            *tail;
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

Process *pop_first_process(Queue *queue) {
    Node *node = queue->head;
    Process *process = node->process;
    queue->head = node->next;
    free(node);
    queue->length--;
    return process;
}

Process *pop_random_process(Queue *queue) {
    int index = time(NULL) % queue->length;
    if (index == 0) { return pop_first_process(queue); }
    Node *parent = queue->head;
    Node *node = queue->head->next;
    while (index > 1) {
        parent = node;
        node = node->next;
        index--;
    }
    queue->length--;
    Process *process = node->process;
    parent->next = node->next;
    if (parent->next == NULL) { queue->tail = parent; }
    free(node);
    return process;
}

Process *take_first_process(Queue *queue) {
    return queue->head->process;
}

int length(Queue *queue) {
    return queue->length;
}

void destroy_queue(Queue *queue) {
    Process *process;
    while (length(queue) != 0) {
        process = pop_first_process(queue);
        destroy_process(process);
    }
    free(queue);
}

void print_queue(Queue *queue) {
    int i = 0;
    Node *node = queue->head;
    while (i < queue->length) {
        Process *process = node->process;
        printf("%s %d: %d\n", process->name, process->pid, get_running_time(process));
        node = node->next;
        i++;
    }
}
