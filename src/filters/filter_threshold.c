#include <stdlib.h>
#include <err.h>
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <limits.h>
#include "../auxiliary/auxiliary.h"
#include "filters.h"

Uint32 binarize_pixel(Uint32 pixel_color, SDL_PixelFormat* format,
                      Uint8 threshold)
{
    Uint8 r, g, b;

    SDL_GetRGB(pixel_color, format, &r, &g, &b);

    Uint8 average = 0.3 * r + 0.3 * g + 0.3 * b;
    Uint32 color;

    if (average >= threshold)
    {
        color = SDL_MapRGB(format, 255, 255, 255);
    }
    else
    {
        color = SDL_MapRGB(format, 0, 0, 0);
    }

    return color;
}

SDL_Surface* binarize(SDL_Surface *s_surface, Uint8 threshold,
                      int s_width, int s_height)
{
    int len = s_width * s_height;
    Uint32* pixels = s_surface->pixels;
    SDL_PixelFormat* format = s_surface->format;

    SDL_LockSurface(s_surface);

    for (int i = 0; i < len; i++)
    {
        pixels[i] = binarize_pixel(pixels[i], format, threshold);
    }

    SDL_UnlockSurface(s_surface);

    // save_image(s_surface, "bin_image.jpeg");

    return s_surface;
}

SDL_Surface* threshold(SDL_Surface *s_surface, Uint8 threshold)
{
    int s_height = s_surface->h;
    int s_width = s_surface->w;
    SDL_Surface *binarized = binarize(s_surface, threshold, s_width, s_height);

    return binarized;
}