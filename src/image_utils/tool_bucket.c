#include <stdlib.h>
#include <err.h>
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include "../auxiliary/auxiliary.h"

void aux_bucket_fill(SDL_Surface* s_surface, int s_width, int s_height,
                    int x, int y,
                    Uint8 r, Uint8 g, Uint8 b,
                    Uint8 r1, Uint8 g1, Uint8 b1)
{
    Uint32 new_color = SDL_MapRGB(s_surface->format, r1, g1, b1);
    Uint32 colorxy = SDL_ReadPixel(s_surface, x, y);
    Uint8 rxy, gxy, bxy;

    SDL_GetRGB(colorxy, s_surface->format, &rxy, &gxy, &bxy);

    if (rxy == r && gxy == g && bxy == b)
    {
        SDL_LockSurface(s_surface);
        SDL_WritePixel(s_surface, x, y, new_color);
        SDL_UnlockSurface(s_surface);

        
	    printf("(x, y) = (%i, %i)\n", x, y);

        if (x - 1 >= 0)
        {
            aux_bucket_fill(s_surface, s_width, s_height, x - 1, y,
                            r, g, b, r1, g1, b1);
        }
        if (x + 1 < s_width)
        {
            aux_bucket_fill(s_surface, s_width, s_height, x + 1, y,
                            r, g, b, r1, g1, b1);
        }
        if (y - 1 >= 0)
        {
            aux_bucket_fill(s_surface, s_width, s_height, x, y - 1,
                            r, g, b, r1, g1, b1);
        }
        if (y + 1 < s_height)
        {
            aux_bucket_fill(s_surface, s_width, s_height, x, y + 1,
                            r, g, b, r1, g1, b1);
        }
    }
}

SDL_Surface* bucket_fill(SDL_Surface* s_surface, int x, int y,
                         Uint8 r1, Uint8 g1, Uint8 b1)
{
    int s_width = s_surface->w;
    int s_height = s_surface->h;

	printf("(s_width, s_height) = (%i, %i)\n", s_width, s_height);
    
    Uint32 color_to_fill = SDL_ReadPixel(s_surface, x, y);
    Uint8 r, g, b;

    SDL_GetRGB(color_to_fill, s_surface->format, &r, &g, &b);

    if ((x >= 0 && x <= s_width) && (y >= 0 && y <= s_height))
    {
        aux_bucket_fill(s_surface, s_width, s_height, x, y,
                        r, g, b, r1, g1, b1);
    }

    return s_surface;
}