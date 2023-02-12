#include <stdlib.h>
#include <err.h>
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <limits.h>
#include "../auxiliary/auxiliary.h"
#include "filters.h"

Uint32 binarize_pixel_otsu(Uint32 pixel_color, SDL_PixelFormat* format,
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

SDL_Surface* binarize_otsu(SDL_Surface *s_surface, Uint8 threshold,
                      int s_width, int s_height)
{
    int len = s_width * s_height;
    Uint32* pixels = s_surface->pixels;
    SDL_PixelFormat* format = s_surface->format;

    SDL_LockSurface(s_surface);

    for (int i = 0; i < len; i++)
    {
        pixels[i] = binarize_pixel_otsu(pixels[i], format, threshold);
    }

    SDL_UnlockSurface(s_surface);

    // save_image(s_surface, "bin_image.jpeg");

    return s_surface;
}

Uint8 get_histogram_threshold_otsu(int *histogram, int start, int end)
{
    int pixel_count = 0;

    for (int i = start; i < end; i++)
    {
        pixel_count += histogram[i];
    }

    int sum1 = 0, sum2 = 0, w1 = 0, w2 = 0, m1 = 0, m2 = 0;
    double mid = 0, max = 0;
    Uint8 threshold = 0;

    int i;

    for (i = start; i < end; i++)
    {
        sum1 += i * histogram[i];
    }

    for (i = start; i < end; i++)
    {
        w1 += histogram[i];

        if (w1 == 0)
            continue;

        w2 = pixel_count - w1;

        if (w2 == 0)
            break;

        sum2 += i * histogram[i];
        m1 = sum2 / w1;
        m2 = (sum1 - sum2) / w2;
        mid = w1 * w2 * (m1 - m2) * (m1 - m2);

        if (mid >= max)
        {
            threshold = i;
            max = mid;
        }
    }

    return squeeze(threshold, 70, 240);
}

Uint8 get_histogram_threshold(int *histogram, int start, int end)
{
    return get_histogram_threshold_otsu(histogram, start, end);
}

SDL_Surface* otsu_threshold(SDL_Surface *s_surface)
{
    int s_height = s_surface->h;
    int s_width = s_surface->w;
    int *hist = image_grayscale_histogram(s_surface, 0, s_width, 0, s_height);
    Uint8 threshold = get_histogram_threshold(hist, 0, 256);
    SDL_Surface *otsu = binarize_otsu(s_surface, threshold, s_width, s_height);

    free(hist);

    return otsu;
}