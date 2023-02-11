#include <stdlib.h>
#include <err.h>
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include "../auxiliary/auxiliary.h"
#include "shapes.h"

SDL_Surface* rectangle(SDL_Surface* s_surface, 
                       int x1, int y1, int x2, int y2,
                       Uint8 r, Uint8 g, Uint8 b,
                       int thickness)
{
    int x1_offset = 0;
    int x2_offset = 0;
    int y1_offset = 0;
    int y2_offset = 0;

    Uint32 color = SDL_MapRGB(s_surface->format, r, g, b);

    if (x2 > x1)
    {
        if (y1 > y2)
        {
            x2_offset = thickness;
            y1_offset = thickness;
        }
        else
        {
            y2_offset = thickness;
            x2_offset = thickness;
        }
    }
    else
    {
        if (y2 > y1)
        {
            y2_offset = thickness;
            x1_offset = thickness;
        }
        else
        {
            y1_offset = thickness;
            x1_offset = thickness;
        }
    }

    int x, y;

    for (x = x1 - x1_offset; x < s_surface->w && x <= x1 + x2_offset; x++)
    {
        s_surface = vertical_line(s_surface, y1, y2, x, color);
    }

    for (x = x2 - x2_offset; x < s_surface->w && x <= x2 + x1_offset; x++)
    {
        s_surface = vertical_line(s_surface, y1, y2, x, color);
    }

    for (y = y1 - y1_offset; y < s_surface->h && y <= y1 + y2_offset; y++)
    {
        s_surface = horizontal_line(s_surface, x1, x2, y, color);
    }

    for (y = y2 - y2_offset; y < s_surface->h && y <= y2 + y1_offset; y++)
    {
        s_surface = horizontal_line(s_surface, x1, x2, y, color);
    }

    return s_surface;
}

SDL_Surface* filled_rectangle(SDL_Surface* s_surface,
                              int x1, int y1, int x2, int y2,
                              Uint8 r, Uint8 g, Uint8 b)
{
    int x, y;
    int tmp;

    if (x1 > x2)
    {
        tmp = x1;
        x1 = x2;
        x2 = tmp;

        if (y1 > y2)
        {
            tmp = y1;
            y1 = y2;
            y2 = tmp;
        }
    }
    else
    {
        if (y1 > y2)
        {
            tmp = y1;
            y1 = y2;
            y2 = tmp;
        }
    }

    Uint32 pixel;

    SDL_LockSurface(s_surface);

    for (x = x1; x <= x2; x++)
    {
        for (y = y1; y <= y2; y++)
        {
            if (x >= 0 && x < s_surface->w && y >= 0 && y < s_surface->h)
            {
                pixel = SDL_MapRGB(s_surface->format,
                                   r, g, b);
                SDL_WritePixel(s_surface, x, y, pixel);
            }
        }
    }

    SDL_UnlockSurface(s_surface);

    return s_surface;
}