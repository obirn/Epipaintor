#include <stdlib.h>
#include <err.h>
#include <math.h>
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include "../auxiliary/auxiliary.h"
#include "filters.h"

int main(int argc, char** argv)
{
    // Checks the number of arguments.
    if (argc < 3)
        errx(EXIT_FAILURE, "Usage: image_file process args\n");

    if (strcmp(argv[2], "gray") == 0)
    {
        SDL_Surface *s = red_light(load_image(argv[1]));
        save_image(s, "test.jpeg");
        return EXIT_SUCCESS;
    }

    return EXIT_SUCCESS;
}