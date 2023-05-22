#ifndef TRANSFORMS_H
#define TRANSFORMS_H

// -- ROTATION -- //

// Rotate image by a certain angle in degrees.
SDL_Surface* rotate_by_angle(SDL_Surface *s_surface, double angle);
SDL_Surface* flipSurfaceHorizontal(SDL_Surface* surface);
SDL_Surface* flipSurfaceVertical(SDL_Surface* surface);
SDL_Surface* resizeSurface(SDL_Surface* surface, double percentage);
#endif