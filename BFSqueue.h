#include <stdio.h>
#include <limits.h>
#include <stdbool.h>
#include <stdlib.h>

#ifndef BFSQUEUE_H
#define BFSQUEUE_H

typedef struct node{
    int position;
    struct node *next;
} *node_t;

typedef struct {
    node_t start;
    node_t end;
} *queue_t;

void initialize_queue(queue_t new_queue);

bool enqueue(queue_t queue, int position);

int dequeue(queue_t queue);

#endif //BFSQUEUE_H