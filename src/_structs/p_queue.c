#include <stdlib.h>
#include <err.h>
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include "../auxiliary/auxiliary.h"
#include "queue.h"
#include "p_queue.h"

p_queue* p_queue_new()
{
    struct p_queue *new_p_queue = malloc(sizeof(struct p_queue));

    if (new_p_queue == NULL)
    {
        errx(EXIT_FAILURE, "Failure malloc() p_queue");
    }

    new_p_queue->size = 0;
    new_p_queue->queue = NULL;

    return new_p_queue;
}

void p_queue_push(p_queue *pq, Uint32 pixel, int x, int y)
{
    pq->queue = queue_push(pq->queue, pixel, x, y);
    pq->size += 1;
}

Uint32 p_queue_pop(p_queue *pq, int *x, int *y)
{
    Uint32 pixel;
    pq->queue = queue_pop(pq->queue, &pixel, x, y);
    pq->size -= 1;
    return pixel;
}

void p_queue_destroy(p_queue *pq)
{
    queue_empty(&(pq->queue));
    free(pq);
}
