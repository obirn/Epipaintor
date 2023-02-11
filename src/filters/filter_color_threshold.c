#include <stdlib.h>
#include <err.h>
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include "../auxiliary/auxiliary.h"
#include "filters.h"

Uint32 colorize_pixel(Uint32 pixel_color, SDL_PixelFormat* format,
                      Uint8 threshold,
                      Uint8 r1, Uint8 g1, Uint8 b1,
                      Uint8 r2, Uint8 g2, Uint8 b2)
{
    Uint8 r, g, b;

    SDL_GetRGB(pixel_color, format, &r, &g, &b);

    Uint8 average = 0.3 * r + 0.3 * g + 0.3 * b;
    Uint32 color;

    if (average >= threshold)
    {
        color = SDL_MapRGB(format, r1, g1, b1);
    }
    else
    {
        color = SDL_MapRGB(format, r2, g2, b2);
    }

    return color;
}

SDL_Surface* colorize(SDL_Surface *s_surface, Uint8 threshold,
                      int s_width, int s_height,
                      Uint8 r1, Uint8 g1, Uint8 b1,
                      Uint8 r2, Uint8 g2, Uint8 b2)
{
    int len = s_width * s_height;
    Uint32* pixels = s_surface->pixels;
    SDL_PixelFormat* format = s_surface->format;

    SDL_LockSurface(s_surface);

    for (int i = 0; i < len; i++)
    {
        pixels[i] = colorize_pixel(pixels[i], format, threshold,
                                   r1, g1, b1, r2, g2, b2);
    }

    SDL_UnlockSurface(s_surface);

    return s_surface;
}

SDL_Surface* color_threshold(SDL_Surface *s_surface, Uint8 threshold,
                            Uint8 r1, Uint8 g1, Uint8 b1,
                            Uint8 r2, Uint8 g2, Uint8 b2)
{
    int s_height = s_surface->h;
    int s_width = s_surface->w;
    SDL_Surface *colorized = colorize(s_surface, threshold, s_width, s_height,
                                      r1, g1, b1, r2, g2, b2);

    return colorized;
}