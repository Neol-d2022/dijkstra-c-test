#ifndef _QUEUE_H_LOADED
#define _QUEUE_H_LOADED

#include <stddef.h>
#include <stdbool.h>

typedef struct QueueNode_struct_t
{
    void *data;
    struct QueueNode_struct_t *next;
} QueueNode_t;

typedef struct
{
    QueueNode_t *head;
    QueueNode_t **lastNodePtr;
    int (*queueDataComparer)(const void *a, const void *b);
    size_t count;
} Queue_t;

/* Initialize a queue object */
/* The newly initialized queue is considered to be a standard queue */
void QueueInit(Queue_t *q);

/* De-initialize a queue object */
/* You may need to destroy / deallocate the data store by it */
void QueueDeInit(Queue_t *q);

/* Check whether the queue is empty */
bool QueueEmpty(Queue_t *q);

/* Return items count of a queue */
size_t QueueCount(Queue_t *q);

/* Assign a comparing function to a queue, make it a priority queue */
/* CAN ONLY CALL ON EMPTY QUEUE, otherwise the program aborts */
void QueueSetComparer(Queue_t *q, int (*queueDataComparer)(const void *a, const void *b));

/* Add data to the queue */
/* Complexity O(1) for standard queue */
/* Complexity O(n) for prioity queue */
void QueueEnqueue(Queue_t *q, void *data);

/* Remove the first (smallest) element */
/* ABORT ON EMPTY QUEUE */
void *QueueDequeue(Queue_t *q);

#endif
