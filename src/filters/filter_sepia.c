#include <stdlib.h>
#include <err.h>
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include "../auxiliary/auxiliary.h"
#include "filters.h"

SDL_Surface* sepia(SDL_Surface* s_surface)
{
    int s_height = s_surface->h;
    int s_width = s_surface->w;
    Uint32* pixels = s_surface->pixels;
    SDL_PixelFormat* format = s_surface->format;

    SDL_LockSurface(s_surface);

    for (int y = 0; y < s_height; y++)
    {
        for (int x = 0; x < s_width; x++)
        {
            Uint8 r, g, b;
            Uint8 new_red, new_green, new_blue;

            SDL_GetRGB(pixels[y * s_width + x], format, &r, &g, &b);

            new_red = min(255, 0.393 * r + 0.769 * g + 0.189 * b);
            new_green = min(255, 0.349 * r + 0.686 * g + 0.168 * b);
            new_blue = min(255, 0.272 * r + 0.534 * g + 0.131 * b);

            pixels[y * s_width + x] = SDL_MapRGB(format, new_red,new_green,
                                                   new_blue);
        }
    }

    SDL_UnlockSurface(s_surface);

    return s_surface;
}