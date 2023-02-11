#ifndef SHAPES_H
#define SHAPES_H

// -- DOT HEADER -- //

// Draw dot.
SDL_Surface* dot(SDL_Surface* s_surface, Uint32 color, int x, int y, int size);

// -- LINE HEADER -- //

// Draw custom line.
SDL_Surface* line(SDL_Surface* s_surface, Uint32 color,
                  int x1, int y1, int x2, int y2, int size);

// Draw vertical line.
SDL_Surface* vertical_line(SDL_Surface *s_surface, int y1, int y2, int x,
                           Uint32 color);

// Drasw horizontal line.
SDL_Surface* horizontal_line(SDL_Surface *s_surface, int x1, int x2, int y,
                             Uint32 color);

// -- RECTANGLE HEADER -- //

// Draw rectangle.
SDL_Surface* rectangle(SDL_Surface* s_surface, 
                       int x1, int y1, int x2, int y2,
                       Uint8 r, Uint8 g, Uint8 b,
                       int thickness);

// Draw and fill rectangle.
SDL_Surface* filled_rectangle(SDL_Surface* s_surface,
                              int x1, int y1, int x2, int y2,
                              Uint8 r, Uint8 g, Uint8 b);

// -- TRIANGLE HEADER -- //

// Draw triangle.
SDL_Surface* triangle(SDL_Surface* s_surface, int x1, int y1, int x2, int y2,
                      Uint8 r, Uint8 g, Uint8 b,
                      int thickness);

// Draw and fill triangle.
SDL_Surface* filled_triangle(SDL_Surface* s_surface,
                             int x1, int y1, int x2, int y2,
                             Uint8 r, Uint8 g, Uint8 b,
                             int thickness);

// -- CIRCLE HEADER -- //

// Draw circle according to bresenham's algorithm.
SDL_Surface* circle(SDL_Surface* s_surface,
                    int x1, int y1, int x2, int y2,
                    Uint8 r, Uint8 g, Uint8 b,
                    int thickness);

// Draw and fill circle according to bresenham's algorithm.
SDL_Surface* filled_circle(SDL_Surface* s_surface,
                           int x1, int y1, int x2, int y2,
                           Uint8 r, Uint8 g, Uint8 b,
                           int thickness);

#endif