#include <stdlib.h>
#include <err.h>
#include <math.h>
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include "../auxiliary/auxiliary.h"
#include "../image_utils/tools.h"
#include "shapes.h"

int main(int argc, char** argv)
{
    if (argc > 90)
    {
        return EXIT_SUCCESS;
    }
    if (strcmp(argv[1], "cir") == 0)
    {
        SDL_Surface *s = circle(load_image(argv[2]), 
                       atoi(argv[3]), atoi(argv[4]), atoi(argv[5]), atoi(argv[6]),
                       atoi(argv[7]), atoi(argv[8]), atoi(argv[9]), atoi(argv[10]));
        save_image(s, "test.jpeg");
        return EXIT_SUCCESS;
    }
    if (strcmp(argv[1], "cir_fill") == 0)
    {
        SDL_Surface *s = filled_circle(load_image(argv[2]), 
                       atoi(argv[3]), atoi(argv[4]), atoi(argv[5]), atoi(argv[6]),
                       atoi(argv[7]), atoi(argv[8]), atoi(argv[9]), atoi(argv[10]));
        save_image(s, "test.jpeg");
        return EXIT_SUCCESS;
    }

    return EXIT_SUCCESS;
}