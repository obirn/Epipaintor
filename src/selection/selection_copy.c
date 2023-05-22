#include <stdlib.h>
#include <err.h>
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <limits.h>
#include "../auxiliary/auxiliary.h"
#include "selection.h"

// Generates copy of selected area as cropped image.
int copy_selection(SDL_Surface* surface, int x, int y, int x2, int y2)
{
    int width = x2 - x;
    int height = y2 - y;

    x = max(0, x);
    x = min(x, surface->w - 1);
    y = max(0, y);
    y = min(y, surface->h - 1);
    width = max(0, width);
    width = min(width, surface->w - 1);
    height = max(0, height);
    height = min(height, surface->h - 1);

    SDL_Surface* crop;
    Uint8 r, g, b;
    Uint32 pixel;

    if (x < width && y < height)
    {
        crop = SDL_CreateRGBSurface(SDL_SWSURFACE,
                                    width - x, height - y,
                                    surface->format->BitsPerPixel,
                                    surface->format->Rmask,
                                    surface->format->Gmask,
                                    surface->format->Bmask,
                                    surface->format->Amask);
        for (int i = 0; i < width - x; i++)
        {
            for (int j = 0; j < height - y; j++)
            {
                pixel = SDL_ReadPixel(surface, i + x, j + y);
                SDL_GetRGB(pixel, surface->format, &r, &g, &b);
                pixel = SDL_MapRGB(surface->format, r, g, b);
                SDL_WritePixel(crop, i, j, pixel);
            }
        }   
    }
    else
    { 
        if (x < width && y >= height)
        {
            crop = SDL_CreateRGBSurface(SDL_SWSURFACE,
                                        width - x, y - height,
                                        surface->format->BitsPerPixel,
                                        surface->format->Rmask,
                                        surface->format->Gmask,
                                        surface->format->Bmask,
                                        surface->format->Amask);
            for (int i = 0; i < width - x; i++)
            {
                for (int j = 0; j < y - height; j++)
                {
                    pixel = SDL_ReadPixel(surface, i + x, j + height);
                    SDL_GetRGB(pixel, surface->format, &r, &g, &b);
                    pixel = SDL_MapRGB(surface->format, r, g, b);
                    SDL_WritePixel(crop, i, j, pixel);
                }
            }   
        }
        else
        {
            if (x >= width && y < height)
            {
                crop = SDL_CreateRGBSurface(SDL_SWSURFACE,
                                            x - width, height - y,
                                            surface->format->BitsPerPixel,
                                            surface->format->Rmask,
                                            surface->format->Gmask,
                                            surface->format->Bmask,
                                            surface->format->Amask);
                for (int i = 0; i < x - width; i++)
                {
                    for (int j = 0; j < height - y; j++)
                    {
                        pixel = SDL_ReadPixel(surface, i + width, j + y);
                        SDL_GetRGB(pixel, surface->format, &r, &g, &b);
                        pixel = SDL_MapRGB(surface->format, r, g, b);
                        SDL_WritePixel(crop, i, j, pixel);
                    }   
                }   
            }
            else
            {
                crop = SDL_CreateRGBSurface(SDL_SWSURFACE,
                                            x - width, y - height,
                                            surface->format->BitsPerPixel,
                                            surface->format->Rmask,
                                            surface->format->Gmask,
                                            surface->format->Bmask,
                                            surface->format->Amask);
                for (int i = 0; i < x - width; i++)
                {
                    for (int j = 0; j < y - height; j++)
                    {
                        pixel = SDL_ReadPixel(surface, i + width, j + height);
                        SDL_GetRGB(pixel, surface->format, &r, &g, &b);
                        pixel = SDL_MapRGB(surface->format, r, g, b);
                        SDL_WritePixel(crop, i, j, pixel);
                    }
                }   
            }
        }
    }

    save_image(crop, "../../cache/selection_copy.png");

    return 1;
}