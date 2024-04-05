#include <BFSqueue.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>


void initialize_queue(queue_t new_queue){
    new_queue->end = NULL;
    new_queue->start = NULL;
}

bool enqueue(queue_t queue, int position){
    node_t new_node = (node_t)malloc(sizeof(node_t));
    if(new_node == NULL){
        return false;
    }
    new_node->position = position;
    new_node->next = NULL;
    if(queue->end != NULL){
        queue->end->next = new_node;
    }
    queue->end = new_node;

    if(queue->start == NULL){
        queue->start = new_node;
    }
    return true;
}

int dequeue(queue_t queue){
    if(queue->start == NULL){
        fprintf(stderr,"Kolejka jest Pusta!");
        return -1;
    }
    node_t temp_node = queue->start;
    int deleted_node = temp_node->position;
    queue->start= queue->start->next;
    if(queue->start == NULL){
        queue->end = NULL;
    }
    free(temp_node);
    return deleted_node;
}