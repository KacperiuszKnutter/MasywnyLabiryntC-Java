#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include "BFSqueue.h"

void initialize_queue(queue_t queue) {

    if (queue != NULL) {
        queue->start = NULL;
        queue->end = NULL;
    }
    queue->ile_node = 0;
}

// Adds a new element to the queue
bool enqueue(queue_t queue, int position, char direction) {

    if (queue == NULL) {
        return false;
    }
    queue->ile_node += 1;

    node_t new_node = (node_t) malloc(sizeof(struct Node));

    if (new_node == NULL) {
        return false;
    }
    new_node->position = position;
    new_node->direction = direction; 
    new_node->next = NULL;

    if (queue->end == NULL) {
        queue->start = new_node;
        queue->end = new_node;
    } else {
        queue->end->next = new_node;
        queue->end = new_node;
    }

    return true;
}


bool dequeue(queue_t queue, int *position, char *direction) {

    if (queue->ile_node <= 0) {
        fprintf(stderr, "Kolejka jest pusta!\n");
        return false;
    }

    queue->ile_node -= 1;

    node_t front_node = queue->start;
    if (position != NULL) *position = front_node->position;
    if (direction != NULL) *direction = front_node->direction;

    queue->start = queue->start->next;
    if (queue->start == NULL) {
        queue->end = NULL;
    }

    free(front_node);
    return true;
}


void free_queue(queue_t queue) {
    if (queue == NULL) {
        return;
    }

    // Iterate through the queue and free each node
    node_t current_node = queue->start;
    while (current_node != NULL) {
        // Hold the next node because current_node is going to be freed
        node_t next_node = current_node->next;
        
        // Free the current node
        free(current_node);
        
        // Move to the next node
        current_node = next_node;
    }

    // Reset the queue's pointers to indicate it's empty
    queue->start = NULL;
    queue->end = NULL;
    queue->ile_node = 0;
}