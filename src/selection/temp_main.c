#include <stdlib.h>
#include <err.h>
#include <math.h>
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include "../auxiliary/auxiliary.h"
#include "../shapes/shapes.h"
#include "selection.h"

int main(int argc, char** argv)
{
    // Checks the number of arguments.
    if (argc < 3)
        errx(EXIT_FAILURE, "Usage: image_file process args\n");

    if (strcmp(argv[2], "s_create") == 0)
    {
        create_selection_base(load_image(argv[1]));
        printf("Created selection...\n");
        make_selection(476, 291, 763, 656);
        printf("Selected selection...\n");
        SDL_Surface* display = display_selection_on_image(load_image(argv[1]), 1);
        printf("Displayed selection...\n");
        save_image(display, "selection.png");
        printf("Saved selected\n");
        if (copy_selection(load_image(argv[1]), 476, 291, 763, 656))
        {
            printf("Copied selection.\n");
            SDL_Surface* pasted = paste_selection(load_image(argv[1]), 1800, 867);
            printf("Pasted selection.\n");
            save_image(pasted, "paste.png");
            SDL_Surface* cut = cut_selection(pasted, 1800, 867, 1900, 900);
            printf("Cut selection.\n");
            save_image(cut, "cut.png");
        }
    }

    return EXIT_SUCCESS;
}
