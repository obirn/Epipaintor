#include <stdlib.h>
#include <err.h>
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include "../auxiliary/auxiliary.h"
#include "shapes.h"

SDL_Surface* triangle(SDL_Surface* s_surface, int x1, int y1, int x2, int y2,
                      Uint8 r, Uint8 g, Uint8 b,
                      int thickness)
{
    Uint32 color = SDL_MapRGB(s_surface->format, r, g, b);

    int x3 = 0;

    if (x1 < x2)
    {
        if (y1 < y2)
        {
            x3 = x1 + ((x2 - x1) / 2);
        }
        else
        {
            x3 = x1 + ((x2 - x1) / 2);
        }
    }
    else
    {
        if (y1 < y2)
        {
            x3 = x2 + ((x1 - x2) / 2);
        }
        else
        {
            x3 = x2 + ((x1 - x2) / 2);
        }
    }

    s_surface = line(s_surface, color, x1, y1, x2, y1, thickness);
    s_surface = line(s_surface, color, x2, y1, x3, y2, thickness);
    s_surface = line(s_surface, color, x3, y2, x1, y1, thickness);

    return s_surface;
}

SDL_Surface* filled_triangle(SDL_Surface* s_surface,
                             int x1, int y1, int x2, int y2,
                             Uint8 r, Uint8 g, Uint8 b,
                             int thickness)
{
    Uint32 color = SDL_MapRGB(s_surface->format, r, g, b);

    int x3 = 0;
    int check;

    if (x1 < x2)
    {
        if (y1 < y2)
        {
            x3 = x1 + ((x2 - x1) / 2);
        }
        else
        {
            x3 = x1 + ((x2 - x1) / 2);
        }
        check = 1;
    }
    else
    {
        if (y1 < y2)
        {
            x3 = x2 + ((x1 - x2) / 2);
        }
        else
        {
            x3 = x2 + ((x1 - x2) / 2);
        }
        check = 0;
    }
    
    while ((check && x1 < x2) || (!check && x1 > x2))
    {
        s_surface = line(s_surface, color, x1, y1, x2, y1, thickness);
        s_surface = line(s_surface, color, x2, y1, x3, y2, thickness);
        s_surface = line(s_surface, color, x3, y2, x1, y1, thickness);

        if (check)
        {
            x1++;
            x2--;
        }
        else
        {
            x1--;
            x2++;
        }
        if (y1 < y2)
        {
            y1++;
            y2--;
        }
        else
        {
            y1--;
            y2++;
        }
    }

    return s_surface;
}