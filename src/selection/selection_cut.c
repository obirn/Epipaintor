#include <stdlib.h>
#include <err.h>
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <limits.h>
#include "../auxiliary/auxiliary.h"
#include "../shapes/shapes.h"
#include "selection.h"

// Cut selection from source image.
SDL_Surface* cut_selection(SDL_Surface* s_surface, int x, int y, int x2, int y2)
{
    SDL_Surface* cut = filled_rectangle(s_surface, x, y, x2, y2, 255, 255, 255);

    return cut;
}