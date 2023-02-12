#include <stdlib.h>
#include <err.h>
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include "../auxiliary/auxiliary.h"
#include "../_structs/queue.h"
#include "../_structs/p_queue.h"

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

SDL_Surface* bucket_fill_rec(SDL_Surface* s_surface, int x, int y,
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

int tolerance(int color1, int color2, int tol)
{
    if (color1 > color2)
    {
        return (color1 - color2) <= tol;
    }

    return (color2 - color1) <= tol;
}

SDL_Surface* bucket_fill(SDL_Surface* s_surface, int x, int y,
                         Uint8 r1, Uint8 g1, Uint8 b1, int tol)
{
    Uint32 color_to_fill = SDL_ReadPixel(s_surface, x, y);
    Uint8 r, g, b;
    SDL_GetRGB(color_to_fill, s_surface->format, &r, &g, &b);

    Uint32 pixel = SDL_ReadPixel(s_surface, x, y);

    if (r == r1 && g == g1 && b == b1)
    {
        return s_surface; // Selected pixel is already at correct color.
    }

    pixel = SDL_MapRGB(s_surface->format, r1, g1, b1);
    SDL_WritePixel(s_surface, x, y, pixel);

    p_queue *bucket = p_queue_new();
    p_queue_push(bucket, pixel, x, y);
    
    Uint8 r2, g2, b2;
    int dx = 0;
    int dy = 0;
    int nx = 0;
    int ny = 0;
    int move = 0;

    while (bucket->size != 0)
    {
        r2 = 0;
        g2 = 0;
        b2 = 0;

        pixel = p_queue_pop(bucket, &dx, &dy);

        for (move = -1; move < 2; move += 2)
        {
            nx = dx + move;
            ny = dy + move;
            if (0 <= nx && nx < s_surface->w)
            {
                pixel = SDL_ReadPixel(s_surface, nx, dy);
                SDL_GetRGB(pixel, s_surface->format, &r2, &g2, &b2);
                if (r1 != r2 || g1 != g2 || b1 != b2)
                {
                    if (tolerance(r, r2, tol)
                        && tolerance(g, g2, tol)
                        && tolerance(b, b2, tol)) 
                    {
                        pixel = SDL_MapRGB(s_surface->format, r1, g1, b1);
                        SDL_WritePixel(s_surface, nx, dy, pixel);
                        p_queue_push(bucket, pixel, nx, dy);
                    }
                }
            }
            if (0 <= ny && ny < s_surface->h)
            {
                pixel = SDL_ReadPixel(s_surface, dx, ny);
                SDL_GetRGB(pixel, s_surface->format, &r2, &g2, &b2);
                if (r1 != r2 || g1 != g2 || b1 != b2)
                {
                    if (tolerance(r, r2, tol)
                        && tolerance(g, g2, tol)
                        && tolerance(b, b2, tol)) 
                    {
                        pixel = SDL_MapRGB(s_surface->format, r1, g1, b1);
                        SDL_WritePixel(s_surface, dx, ny, pixel);
                        p_queue_push(bucket, pixel, dx, ny);
                    }
                }
            }
        }
    }

    p_queue_destroy(bucket);

    return s_surface;
}