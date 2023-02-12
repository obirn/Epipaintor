#ifndef TOOLS_H
#define TOOLS_H

// -- BUCKET -- //

// Recursive looping through image pixels.
void aux_bucket_fill(SDL_Surface* s_surface, int s_width, int s_height,
                    int x, int y,
                    Uint8 r, Uint8 g, Uint8 b,
                    Uint8 r1, Uint8 g1, Uint8 b1);

// Fill color at positon X Y with new color defined as R, G, B values.
SDL_Surface* bucket_fill_rec(SDL_Surface* s_surface, int x, int y,
              			     Uint8 r1, Uint8 g1, Uint8 b1);

// Tolerance check.
int tolerance(int color1, int color2, int tol);

// Queue based color filling.
SDL_Surface* bucket_fill(SDL_Surface* s_surface, int x, int y,
                         Uint8 r1, Uint8 g1, Uint8 b1, int tol);

#endif