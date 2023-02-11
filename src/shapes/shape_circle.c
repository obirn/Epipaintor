#include <stdlib.h>
#include <err.h>
#include <math.h>
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include "../auxiliary/auxiliary.h"
#include "../image_utils/tools.h"
#include "shapes.h"

SDL_Surface* circle(SDL_Surface* s_surface,
                    int x1, int y1, int x2, int y2,
                    Uint8 r, Uint8 g, Uint8 b,
                    int thickness)
{
    Uint32 color = SDL_MapRGB(s_surface->format, r, g, b);

    int x = 0;
    int radius = sqrt((x2 - x1) * (x2 - x1) + (y2 - y1) * (y2 - y1));
    int y = radius;
    int d = radius - 1;
    while (y >= x)
    {
        s_surface = dot(s_surface, color, x1 + x, y1 + y, thickness);
        s_surface = dot(s_surface, color, x1 - x, y1 + y, thickness);
        s_surface = dot(s_surface, color, x1 + x, y1 - y, thickness);
        s_surface = dot(s_surface, color, x1 - x, y1 - y, thickness);
        s_surface = dot(s_surface, color, x1 + y, y1 + x, thickness);
        s_surface = dot(s_surface, color, x1 - y, y1 + x, thickness);
        s_surface = dot(s_surface, color, x1 + y, y1 - x, thickness);
        s_surface = dot(s_surface, color, x1 - y, y1 - x, thickness);

        if (d >= 2 * x)
        {
            d = d - 2 * x - 1;
            x += 1;
        }
        else
        {
            if (d < 2 * (radius - y))
            {
                d = d + 2 * y - 1;
                y -= 1;
            }
            else
            {
                d = d + 2 * (y - x - 1);
                y -= 1;
                x += 1;
            }
        }
    }

    return s_surface;
}

SDL_Surface* filled_circle(SDL_Surface* s_surface,
                           int x1, int y1, int x2, int y2,
                           Uint8 r, Uint8 g, Uint8 b,
                           int thickness)
{
    s_surface = circle(s_surface, x1, y1, x2, y2, r, g, b, thickness);

    s_surface = bucket_fill(s_surface, (x1 + x2) / 2, (y1 + y2) / 2, r, g, b);

    return s_surface;
}