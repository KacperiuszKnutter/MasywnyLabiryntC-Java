#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>


typedef struct Node {
    int position;
    char direction; 
    struct Node *next;
} *node_t;


typedef struct Queue {
    node_t start;
    node_t end;
    int ile_node;
} *queue_t;



void initialize_queue(queue_t queue);
bool enqueue(queue_t queue, int position, char direction);
bool dequeue(queue_t queue, int *position, char *direction);
void free_queue(queue_t queue);

