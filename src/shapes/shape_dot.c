#include <stdlib.h>
#include <err.h>
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include "../auxiliary/auxiliary.h"
#include "shapes.h"

SDL_Surface* dot(SDL_Surface* s_surface, Uint32 color, int x, int y, int size)
{
    Uint8 r, g, b;

    SDL_GetRGB(color, s_surface->format, &r, &g, &b);

    int i = x - size;
    int j = y;
    int offset = 0;

    SDL_LockSurface(s_surface);

    while (i < x)
    {
        while(j < y + offset)
        {
            if (i >= 0 && j >= 0 && i < s_surface->w && j < s_surface->h)
            {
                SDL_WritePixel(s_surface, i, j,
                               SDL_MapRGB(s_surface->format, r, g, b));
            }
            j++;
        }
        i++;
        offset++;
        j = y - offset;
    }

    while(i <= x+size)
    {
        while (j < y + offset)
        {
            if (i >= 0 && j >= 0 && i < s_surface->w && j < s_surface->h)
            {
                SDL_WritePixel(s_surface, i, j,
                               SDL_MapRGB(s_surface->format, r, g, b));
            }
            j++;
        }
        i++;
        offset--;
        j = y - offset;
    }

    SDL_UnlockSurface(s_surface);

    return s_surface;
}