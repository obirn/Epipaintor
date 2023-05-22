#include <stdlib.h>
#include <err.h>
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <limits.h>
#include "../auxiliary/auxiliary.h"
#include "../shapes/shapes.h"
#include "selection.h"

void make_selection(int x1, int y1, int x2, int y2)
{
    Uint8 r = 253;
    Uint8 g = 254;
    Uint8 b = 255;

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

    SDL_Surface* selection = load_image("../cache/selection.png");

    Uint32 pixel;

    SDL_LockSurface(selection);

    for (x = x1; x <= x2; x++)
    {
        for (y = y1; y <= y2; y++)
        {
            if (x >= 0 && x < selection->w && y >= 0 && y < selection->h)
            {
                pixel = SDL_MapRGB(selection->format,
                                   r, g, b);
                SDL_WritePixel(selection, x, y, pixel);
            }
        }
    }

    SDL_UnlockSurface(selection);

    save_image(selection, "../cache/selection.png");
}