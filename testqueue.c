#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include "BFSqueue.h"

int main(int argc, char** argv)
{
    struct Queue myQueueStruct;
    queue_t myQueue = &myQueueStruct;
    initialize_queue(myQueue);

    // Enqueue elements
    printf("Enqueueing elements...\n");
    enqueue(myQueue, 10, 'P');
    enqueue(myQueue, 20, 'L');
    enqueue(myQueue, 30, 'G');

    int pos;
    char dir;
    while (dequeue(myQueue, &pos, &dir)) {
        printf("Dequeued: Position = %d, Direction = %c\n", pos, dir);
    }


    return 0;
}