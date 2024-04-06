#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

// Definition of the node structure
typedef struct Node {
    int position;
    char direction; // Add this line to include a direction field
    struct Node *next;
} *node_t;
// Definition of the queue structure

typedef struct Queue {
    node_t start;
    node_t end;
    int ile_node;
} *queue_t;


// Function declarations (as you've provided)
void initialize_queue(queue_t queue);
bool enqueue(queue_t queue, int position, char direction);
bool dequeue(queue_t queue, int *position, char *direction);
void free_queue(queue_t queue);

