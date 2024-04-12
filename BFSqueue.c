#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <BFSqueue.h>

void initialize_queue(queue_t queue) {

    if (queue != NULL) {
        queue->start = NULL;
        queue->end = NULL;
    }
    queue->ile_node = 0;
}

// dodaj nowy element o kolejki
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

//zdejmij z kolejki 
bool dequeue(queue_t queue, int *position, char *direction) {

    if (queue->ile_node <= 0) {
        fprintf(stderr, "Kolejka jest pusta!\n");
        queue->ile_node = -1;
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

//zwolnij cala kolejke
void free_queue(queue_t queue) {
    if (queue == NULL) {
        return;
    }

    
    node_t current_node = queue->start;
    while (current_node != NULL) {
       
        node_t next_node = current_node->next;
        
       
        free(current_node);
        
       
        current_node = next_node;
    }

    // reset wskaznikow
    queue->start = NULL;
    queue->end = NULL;
    queue->ile_node = 0;
}