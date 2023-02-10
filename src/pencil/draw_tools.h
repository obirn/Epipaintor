#ifndef DRAWING_TOOLS_H
#define DRAWING_TOOLS_H

#include <SDL.h>

void point(SDL_Surface* surface, SDL_Color color, int x, int y, int size);
void drawline(SDL_Surface* img, SDL_Color color, int x1, int y1, int x2, int y2, int size);

#endif