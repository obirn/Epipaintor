#include <stdlib.h>
#include <err.h>
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <math.h>
#include "../auxiliary/auxiliary.h"
#include "transforms.h"

SDL_Surface* rotate_by_angle(SDL_Surface *s_surface, double angle)
{
	SDL_Surface* d_surface;
	int s_height = s_surface->h;
	int s_width = s_surface->w;
	int m_x = (s_width / 2);
	int m_y = (s_height / 2);
	double angle_radian = degrees_to_rad(angle);
	double cos_s = cos(angle_radian);
	double sin_s = sin(angle_radian);
	double d_width  = ceil(s_width * fabs(cos_s) 
					  + s_height * fabs(sin_s));
	double d_height = ceil(s_width * fabs(sin_s) 
					  + s_height * fabs(cos_s));
	double rotate_x, rotate_y;
	Uint32 color;

	d_surface = SDL_CreateRGBSurface(SDL_SWSURFACE, d_width, d_height, 
		s_surface->format->BitsPerPixel, s_surface->format->Rmask,
		s_surface->format->Gmask, s_surface->format->Bmask,
		s_surface->format->Amask);

	SDL_FillRect(d_surface, NULL, SDL_MapRGB(d_surface->format, 255, 255, 255));

	if (d_surface == NULL)
	{
		errx(EXIT_FAILURE,"%s", SDL_GetError());
	}

	int m_x_dest = d_surface->w/2.;
	int m_y_dest = d_surface->h/2.;

	SDL_LockSurface(s_surface);
	SDL_LockSurface(d_surface);

	for (int y = 0; y < d_height; ++y)
	{
		for (int x = 0; x < d_width; ++x)
		{
			rotate_x = (ceil (cos_s * (x-m_x_dest) 
						+ sin_s * (y-m_y_dest) + m_x));
			rotate_y = (ceil (-sin_s * (x-m_x_dest) 
						+ cos_s * (y-m_y_dest) + m_y));

			if (0 <= rotate_x 
				&& rotate_x < s_width
				&& 0 <= rotate_y 
				&& rotate_y < s_height)
			{
				color = SDL_ReadPixel(s_surface, rotate_x, rotate_y);
				SDL_WritePixel(d_surface, x, y, color);
			}
		}
	}

	SDL_UnlockSurface(s_surface);
	SDL_UnlockSurface(d_surface);

	return d_surface;
}