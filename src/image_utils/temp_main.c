#include <stdlib.h>
#include <err.h>
#include <math.h>
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include "../auxiliary/auxiliary.h"
#include "../_structs/queue.h"
#include "../_structs/p_queue.h"
#include "tools.h"

int main(int argc, char** argv)
{
    if (argc > 80)
    {
        printf("d");
    }
    if (strcmp(argv[1], "bucket") == 0)
    {
        SDL_Surface *s = bucket_fill(load_image("test.png"), 543, 374, 255, 0, 0, 0);
        save_image(s, "test1.jpeg");
        SDL_FreeSurface(s);
        SDL_Surface *s2 = bucket_fill(load_image("test.png"), 543, 374, 255, 0, 0, 5);
        save_image(s2, "test2.jpeg");
        SDL_FreeSurface(s2);
        SDL_Surface *s3 = bucket_fill(load_image("test.png"), 543, 374, 255, 0, 0, 30);
        save_image(s3, "test3.jpeg");
        SDL_FreeSurface(s3);
        SDL_Surface *s4 = bucket_fill(load_image("test.png"), 543, 374, 255, 0, 0, 70);
        save_image(s4, "test4.jpeg");
        SDL_FreeSurface(s4);
        SDL_Surface *s5 = bucket_fill(load_image("test.png"), 543, 374, 255, 0, 0, 150);
        save_image(s5, "test5.jpeg");
        SDL_FreeSurface(s5);
        SDL_Surface *s6 = bucket_fill(load_image("test.png"), 543, 650, 255, 0, 0, 0);
        save_image(s6, "test6.jpeg");
        SDL_FreeSurface(s6);
        SDL_Surface *s7 = bucket_fill(load_image("test.png"), 204, 92, 50, 69, 254, 0);
        save_image(s7, "test7.jpeg");
        SDL_FreeSurface(s7);
        return EXIT_SUCCESS;
    }

    return EXIT_SUCCESS;
}