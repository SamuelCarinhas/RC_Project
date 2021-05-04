#ifndef QUEUE_HEADER
#define QUEUE_HEADER

#include <stdlib.h>
#include <string.h>

typedef struct queue_node queue_node_t;
typedef struct queue queue_t;

struct queue_node {
    void * data;
    queue_node_t * next;
};

struct queue {
    size_t size;
    queue_node_t * start, * end;
};

queue_t * new_queue();
queue_node_t * new_queue_node(void * data, size_t data_size);

void queue_push(queue_t * queue, queue_node_t * node);
void queue_pop(queue_t * queue);
void * queue_peek(queue_t * queue);
void remove_queue(queue_t * queue);
void print_queue(queue_t * queue, void (* printNode)(void *));

#endif