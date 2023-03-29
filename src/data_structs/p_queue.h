#ifndef P_QUEUE_H
#define P_QUEUE_H

typedef struct p_queue
{
    size_t size;
    queue* queue;
} p_queue;

p_queue* p_queue_new();

void p_queue_push(p_queue *pq, Uint32 pixel, int x, int y);

Uint32 p_queue_pop(p_queue *pq, int *x, int *y);

void p_queue_destroy(p_queue *pq);

#endif