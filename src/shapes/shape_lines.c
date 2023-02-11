#include <stdlib.h>
#include <err.h>
#include <math.h>
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include "../auxiliary/auxiliary.h"
#include "shapes.h"

SDL_Surface* line(SDL_Surface* s_surface, Uint32 color,
                  int x1, int y1, int x2, int y2, int size)
{
    int x, y;
    int dx,dy;
    int xincr, yincr;
    int offset;
    int i;

    dx = abs(x2-x1);
    dy = abs(y2-y1);

    if (x1 < x2)
    {
        xincr = 1;
    }
    else
    {
        xincr = -1;
    }

    if (y1 < y2)
    {
        yincr = 1;
    }
    else
    {
        yincr = -1;
    }

    x = x1;
    y = y1;

    if (dx > dy)
    {
        offset = dx / 2;
        for (i = 0; i < dx; i++)
        {
            x += xincr;
            offset += dy;
            if (offset > dx)
            {
                offset -= dx;
                y += yincr;
            }
            dot(s_surface, color, x, y, size);
        }
    }
    else
    {
        offset = dy / 2;
        for (i = 0; i < dy; i++)
        {
            y += yincr;
            offset += dx;
            if (offset > dy)
            {
                offset -= dy;
                x += xincr;
            }
            dot(s_surface, color, x, y, size);
        }
    }

    dot(s_surface, color, x1, y1, size);
    dot(s_surface, color, x2, y2, size);

    return s_surface;
}

SDL_Surface* vertical_line(SDL_Surface *s_surface, int y1, int y2, int x,
                           Uint32 color)
{
    Uint8 r, g, b;

    SDL_GetRGB(color, s_surface->format, &r, &g, &b);

    if (x < 0 || x >= s_surface->w)
    {
        return s_surface;
    }

    if (y1 > y2)
    {
        int tmp = y1;
        y1 = y2;
        y2 = tmp;
    }

    int y;
    Uint32 pixel;

    SDL_LockSurface(s_surface);

    for (y = y1; y <= y2; y++)
    {
        if (y >= 0 && y < s_surface->h)
        {
            pixel = SDL_MapRGB(s_surface->format,
                               r, g, b);
            SDL_WritePixel(s_surface, x, y, pixel);
        }
    }

    SDL_UnlockSurface(s_surface);

    return s_surface;
}

SDL_Surface* horizontal_line(SDL_Surface *s_surface, int x1, int x2, int y,
                             Uint32 color)
{
    Uint8 r, g, b;

    SDL_GetRGB(color, s_surface->format, &r, &g, &b);

    if (y < 0 || y >= s_surface->h)
    {
        return s_surface;
    }

    if (x1 > x2)
    {
        int tmp = x1;
        x1 = x2;
        x2 = tmp;
    }

    int x;
    Uint32 pixel;

    SDL_LockSurface(s_surface);

    for (x = x1; x <= x2; x++)
    {
        if (x >= 0 && x < s_surface->w)
        {
            pixel = SDL_MapRGB(s_surface->format,
                               r, g, b);
            SDL_WritePixel(s_surface, x, y, pixel);
        }
    }

    SDL_UnlockSurface(s_surface);

    return s_surface;
}