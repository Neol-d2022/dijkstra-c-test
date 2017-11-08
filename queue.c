#include <string.h>
#include <stdlib.h>

#include "queue.h"
#include "mm.h"

void QueueInit(Queue_t *q)
{
    memset(q, 0, sizeof(*q));
}

void QueueDeInit(Queue_t *q)
{
    QueueNode_t *c, *d;

    c = q->head;
    while (c)
    {
        d = c->next;
        Mfree(c);
        c = d;
    }
}

bool QueueEmpty(Queue_t *q)
{
    return (q->count == 0) ? (true) : (false);
}

size_t QueueCount(Queue_t *q)
{
    return q->count;
}

void QueueSetComparer(Queue_t *q, int (*queueDataComparer)(const void *a, const void *b))
{
#ifndef _EDSI_PRODUCTION
    if (q->count)
        abort();
#endif
    q->queueDataComparer = queueDataComparer;
}

void QueueEnqueue(Queue_t *q, void *data)
{
    QueueNode_t **c;
    QueueNode_t *d;

    if (q->queueDataComparer)
    {
        c = &(q->head);

        while (*c)
        {
            if ((q->queueDataComparer)(data, (*c)->data) < 0)
                break;
            c = &((*c)->next);
        }

        d = (QueueNode_t *)Mmalloc(sizeof(*d));
        d->data = data;
        d->next = *c;
        *c = d;
        q->count += 1;
        return;
    }
    else
    {
        if (q->count == 0)
            q->lastNodePtr = &(q->head);

        d = (QueueNode_t *)Mmalloc(sizeof(*d));
        d->data = data;
        d->next = *(q->lastNodePtr);
        *(q->lastNodePtr) = d;
        q->lastNodePtr = &(d->next);
        q->count += 1;
        return;
    }
}

void *QueueDequeue(Queue_t *q)
{
    QueueNode_t *c;
    void *d;

#ifndef _EDSI_PRODUCTION
    if (q->count == 0)
        abort();
#endif

    c = q->head;
    q->head = c->next;
    q->count -= 1;
    d = c->data;
    Mfree(c);

    return d;
}
