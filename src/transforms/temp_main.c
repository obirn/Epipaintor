#include <stdlib.h>
#include <err.h>
#include <math.h>
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include "../auxiliary/auxiliary.h"
#include "transforms.h"

int main(int argc, char** argv)
{
    // Checks the number of arguments.
    if (argc < 3)
        errx(EXIT_FAILURE, "Usage: image_file process args\n");

    if (strcmp(argv[2], "rotation") == 0)
    {
        SDL_Surface* rot = rotate_by_angle(load_image(argv[1]),
                           strtol(argv[3], NULL, 10));
        save_image(rot, "test.jpeg");
        return EXIT_SUCCESS;
    }

    return EXIT_SUCCESS;
}