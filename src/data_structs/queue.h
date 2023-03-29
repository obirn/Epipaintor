#ifndef QUEUE_H
#define QUEUE_H

typedef struct queue
{
    Uint32 pixel;
    int x;
    int y;
    struct queue* next;
} queue;

queue* queue_push(queue *start, Uint32 pixel, int x, int y);

queue* queue_pop(queue *start, Uint32 *pixel, int *x, int *y);

void queue_empty(queue **pstart);

#endif