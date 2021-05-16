#include "queue.h"

queue_t * new_queue() {
    queue_t * queue = (queue_t *) malloc(sizeof(queue_t));

    queue->size = 0;
    queue->start = NULL;
    queue->end = NULL;

    return queue;
}

void queue_push(queue_t * queue, queue_node_t * node) {
    if(queue->end == NULL) {
        queue->start = node;
    } else {
        queue->end->next = node;
    }
    queue->end = node;
    queue->size++;
}

void * queue_peek(queue_t * queue) {
    return queue->start;
}

void queue_pop(queue_t * queue) {
    if(queue->start == NULL) {
        return;
    }

    queue_node_t * next_start = queue->start->next;
    free(queue->start);
    queue->start = next_start;

    if(queue->start == NULL) {
        queue->end = NULL;
    }

    queue->size--;
}

void remove_queue(queue_t * queue) {
    for(size_t i = 0; i < queue->size; i++) {
        queue_pop(queue);
    }

    free(queue);
}

void print_queue(queue_t * queue, void (* printNode)(void *)) {
    queue_node_t * current = queue->start;
    while(current != NULL) {
        printNode(current->data);
        current = current->next;
    }
}

queue_node_t * new_queue_node(void * data, size_t data_size) {
    queue_node_t * node = (queue_node_t *) malloc(sizeof(queue_node_t));

    node->data = malloc(data_size);
    memcpy(node->data, data, data_size);
    node->next = NULL;

    return node;
}
