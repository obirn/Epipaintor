#include <stdlib.h>
#include <err.h>
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <limits.h>
#include "../auxiliary/auxiliary.h"
#include "selection.h"

// Paste selection from selection_copy.png.
SDL_Surface* paste_selection(SDL_Surface* s_surface, int x, int y)
{
    SDL_Surface* paste = load_image("../cache/selection_copy.png");

    if (paste == NULL)
    {
        printf("Selection - Paste: error.");
        return paste;
    }

    Uint8 r, g, b;
    Uint32 pixel;

    int start_x = 0;
    int start_y = 0;

    if (x < paste->w / 2)
    {
        start_x = paste->w / 2 - x;
    }
    if (y < paste->h / 2)
    {
        start_y = paste->h / 2 - y;
    }


    int b1x = SDL_max(0, x - paste->w / 2);
    int b2x = SDL_min(s_surface->w, x + paste->w / 2);
    int b1y = SDL_max(0, y - paste->h / 2);
    int b2y = SDL_min(s_surface->h, y + paste->h / 2);

    SDL_LockSurface(s_surface);

    for (int i = b1x; i < b2x; i++)
    {
        for (int j = b1y; j < b2y; j++)
        {
            pixel = SDL_ReadPixel(paste, start_x + i - b1x, start_y + j - b1y);
            SDL_GetRGB(pixel, paste->format, &r, &g, &b);
            pixel = SDL_MapRGB(paste->format, r, g, b);
            SDL_WritePixel(s_surface, i, j, pixel);
        }
    }

    SDL_UnlockSurface(s_surface);

    return s_surface;
}