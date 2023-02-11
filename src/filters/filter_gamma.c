#include <stdlib.h>
#include <err.h>
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <math.h>
#include "../auxiliary/auxiliary.h"

Uint8 pixel_to_gamma(Uint8 pixel_color, double ga)
{
    return (Uint8) (255 * SDL_pow((double) pixel_color / 255, ga));
}

SDL_Surface* gam(SDL_Surface *s_surface, double ga)
{
	int s_height = s_surface->h;
	int s_width = s_surface->w;
	Uint32* pixels = s_surface-> pixels;
	SDL_PixelFormat* format = s_surface->format;

	SDL_LockSurface(s_surface);

	for (int y = 0; y < s_height; y++)
	{
	    for (int x = 0; x < s_width; x++)
	    {
	    	Uint8 r, g, b;
	        SDL_GetRGB(pixels[y * s_width + x], format, &r, &g, &b);

	        r = pixel_to_gamma(r, ga);
	        g = pixel_to_gamma(g, ga);
	        b = pixel_to_gamma(b, ga);

	        pixels[y * s_width + x] = SDL_MapRGB(format, r, g, b);
	    }
	}

	SDL_UnlockSurface(s_surface);

	return s_surface;
}