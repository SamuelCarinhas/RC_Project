#include <stdio.h>
#include "data_structures/queue.h"

void printInt(void * data) {
    printf("%d\n", *(int *) data);
}

int main() {
    
    queue_t * queue = new_queue();

    int numbers[] = {1, 4, 2, 3, 5};

    for(int i = 0; i < (int) (sizeof(numbers) / sizeof(int)); i++) {
        queue_push(queue, new_queue_node(&numbers[i], sizeof(int)));
    }

    printf("Queue size: %ld\n", queue->size);
    print_queue(queue, printInt);

    queue_pop(queue);

    printf("Queue size: %ld\n", queue->size);
    print_queue(queue, printInt);

    return 0;
}