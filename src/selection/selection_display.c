#include <stdlib.h>
#include <err.h>
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <limits.h>
#include "../auxiliary/auxiliary.h"
#include "selection.h"

SDL_Surface* display_selection_on_image(SDL_Surface *s_surface, int status)
{
    SDL_Surface* selection = load_image("../../cache/selection.png");

    if (status == 1)
    {
        s_surface = load_image("../../cache/temp_copy.png");
        return s_surface;
    }

    // Create temporary copy of SDL_Surface.
    save_image(s_surface, "../../cache/temp_copy.png");

    int s_height = s_surface->h;
    int s_width = s_surface->w;
    Uint32* pixels = s_surface->pixels;
    SDL_PixelFormat* format = s_surface->format;
    Uint32* selection_pixels = selection->pixels;
    SDL_PixelFormat* selection_format = selection->format;

    SDL_LockSurface(s_surface);
    SDL_LockSurface(selection);

    for (int y = 0; y < s_height; y++)
    {
        for (int x = 0; x < s_width; x++)
        {
            Uint8 selection_r, selection_g, selection_b;
            SDL_GetRGB(selection_pixels[y * s_width + x], selection_format,
                       &selection_r, &selection_g, &selection_b);
            if (selection_r == 253 && selection_g == 254 && selection_b == 255)
            {
                Uint8 r, g, b;
                Uint8 new_red, new_green, new_blue;

                SDL_GetRGB(pixels[y * s_width + x], format, &r, &g, &b);

                new_red = 0;
                new_green = 0;
                new_blue = 255;

                pixels[y * s_width + x] = SDL_MapRGB(format, new_red,new_green,
                                                       new_blue);
            }
        }
    }

    SDL_UnlockSurface(s_surface);
    SDL_UnlockSurface(selection);

    return s_surface;
}