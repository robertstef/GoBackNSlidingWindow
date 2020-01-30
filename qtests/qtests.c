/*
 * Robert Stefanyshin
 * robertstefanyshin@gmail.com
 */

#include <stdio.h>
#include <stdlib.h>
#include "../queue.h"

int main()
{
    QUEUE *queue;
    int *vals;
    int rv;

    // initialize values to put in queue
    vals = malloc(sizeof(int) * 20);

    for(int i = 1; i < 21; i++)
        vals[i - 1] = i;

    // Tests queueCreate()
    printf("Testing queueCreate()\n");
    queue = queueCreate();

    if ( queue->head != NULL || queue->tail != NULL || queue->size != 0 )
        printf("queueCreate() error - default values incorrect\n");

    // Test enqueue()
    printf("Testing enqueue()\n");
    
    rv = enqueue(queue, &vals[0]);
    if ( rv < 0 )
        printf("enqueue(): return value is -1\n");
    if ( *(int *)(queue->head->item) != 1 || *(int *)(queue->tail->item) != 1 
            || queue->size != 1 )
        printf("enqueue() error: adding to empty queue\n");

    rv = enqueue(queue, &vals[1]);
    if ( rv < 0 )
        printf("enqueue(): return value is -1\n");
    if ( *(int *)(queue->head->item) != 1 || *(int *)(queue->tail->item) != 2 
            || queue->size != 2 )
        printf("enqueue() error: adding to non-empty queue\n");
    
    rv = enqueue(queue, &vals[2]);
    if ( rv < 0 )
        printf("enqueue(): return value is -1\n");
    if ( *(int *)(queue->head->item) != 1 || *(int *)(queue->tail->item) != 3
            || queue->size != 3 )
        printf("enqueue() error: adding to non-empty queue\n");

    rv = enqueue(queue, &vals[3]);
    if ( rv < 0 )
        printf("enqueue(): return value is -1\n");
    if ( *(int *)(queue->head->item) != 1 || *(int *)(queue->tail->item) != 4
            || queue->size != 4 )
        printf("enqueue() error: adding to non-empty queue\n");

    // clean up
    free(queue);
    free(vals);
    queue = NULL;
    vals = NULL;

    printf("***TEST SCRIPT COMPLETE***\n");
    return 0;
}
