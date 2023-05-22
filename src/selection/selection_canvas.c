#include <stdlib.h>
#include <err.h>
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <limits.h>
#include "../auxiliary/auxiliary.h"
#include "selection.h"

void create_selection_base(SDL_Surface *s_surface)
{
    SDL_surface* selection;

    selection = SDL_CreateRGBSurface(SDL_SWSURFACE, d_width, d_height, 
        s_surface->format->BitsPerPixel, s_surface->format->Rmask,
        s_surface->format->Gmask, s_surface->format->Bmask,
        s_surface->format->Amask);

    // Unique pixel identifier is (253, 254, 255).
    SDL_FillRect(selection, NULL,
                 SDL_MapRGB(selection->format, 253, 254, 255));

    save_image(selection, "../../cache/selection.png");

    // printf("Selection: generated selection base.");
}

void clear_selection_base(SDL_Surface* s_surface)
{
    create_selection_base(s_surface);
}