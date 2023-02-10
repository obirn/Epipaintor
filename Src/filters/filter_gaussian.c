#include <stdlib.h>
#include <err.h>
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include "../utils/auxiliary.h"
#include "filters.h"

Uint32 mean(SDL_Surface *s_surface, int i, int j, int radius,
            int s_width, int s_height)
{
    const int initial_h = SDL_max(i - radius, 0);
    const int initial_w = SDL_max(j - radius, 0);
    const int final_h = SDL_min(i + radius, s_height - 1);
    const int final_w = SDL_min(j + radius, s_width - 1);
    const int nb_pixel = ((final_h - initial_h) * (final_w - initial_w));
    const Uint32 *pixels = s_surface->pixels;
    Uint32 sum_r = 0, sum_g = 0, sum_b = 0;
    SDL_Color color;

    for (i = initial_h; i < final_h; i++)
    {
        for (j = initial_w; j < final_w; j++)
        {
            SDL_GetRGB(pixels[i * s_width + j], s_surface->format,
                &color.r, &color.g, &color.b);

            sum_r += color.r;
            sum_g += color.g;
            sum_b += color.b;
        }
    }

    return SDL_MapRGB(s_surface->format, sum_r / nb_pixel, sum_g / nb_pixel,
                      sum_b / nb_pixel);
}

SDL_Surface* gaussian(SDL_Surface *s_surface, int radius)
{
    int s_width = s_surface->w;
    int s_height = s_surface->h;
    Uint32* pixels = s_surface->pixels;

    SDL_LockSurface(s_surface);

    for (int y = 0; y < s_height; ++y)
    {
        for(int x = 0; x < s_width; ++x)
        {
            pixels[y * s_width + x] = mean(s_surface, y, x, radius
                                           s_width, s_height);
        }
    }

    SDL_UnlockSurface(s_surface);

    return s_surface;
}