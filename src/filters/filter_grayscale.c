#include <stdlib.h>
#include <err.h>
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <math.h>

// Compute grayscale function.
Uint32 pixel_to_grayscale(Uint32 pixel_color, SDL_PixelFormat* format)
{
	Uint8 r, g, b;

	SDL_GetRGB(pixel_color, format, &r, &g, &b);

	Uint8 average = 0.4 * r + 0.35 * g + 0.25 * b;
	r = g = b = average;
	Uint32 color = SDL_MapRGB(format, r, g, b);

	return color;
}

// Apply grayscale filter to SDL_Surface*.
SDL_Surface* grayscale(SDL_Surface *s_surface)
{
	Uint32* pixels = s_surface-> pixels;
	int len = s_surface->w * s_surface->h;
	SDL_PixelFormat* format = s_surface->format;

	SDL_LockSurface(s_surface);

	int p = 0;

	while (p < len)
	{
		pixels[p] = pixel_to_grayscale(pixels[p], format);
		p++;
	}

	SDL_UnlockSurface(s_surface);

	return s_surface;
}