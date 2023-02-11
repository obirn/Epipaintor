#include <stdlib.h>
#include <err.h>
#include <math.h>
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include "../auxiliary/auxiliary.h"
#include "tools.h"

int main(int argc, char** argv)
{
    if (argc > 80)
    {
        printf("d");
    }
    if (strcmp(argv[2], "bucket") == 0)
    {
        SDL_Surface *s = bucket_fill(load_image(argv[1]),
                                   atoi(argv[3]), atoi(argv[4]),
                                   atoi(argv[5]), atoi(argv[6]), atoi(argv[7]));
        save_image(s, "test.jpeg");
        return EXIT_SUCCESS;
    }

    return EXIT_SUCCESS;
}