#ifndef SELECTION_H
#define SELECTION_H

// -- SELECTION BASE CREATE -- //

// Create base of selection.
void create_selection_base(SDL_Surface *s_surface);

// Clear selection.
void clear_selection_base(SDL_Surface* s_surface);

// -- SELECTION -- //
void make_selection(int x1, int y1, int x2, int y2);

// -- SELECTION DISPLAY -- //

/*
** Display selection.
** Status 1 meaning the display is already active and should be turned off.
** Other status meaning the display should be turned on.
*/
SDL_Surface* display_selection_on_image(SDL_Surface *s_surface, int status);

// -- SELECTION COPY -- //

// Generates copy of selected area as cropped image.
int copy_selection(SDL_Surface* surface, int x, int y, int x2, int y2);

// -- SELECTION PASTE -- //

// Paste selection from selection_copy.png.
SDL_Surface* paste_selection(SDL_Surface* s_surface, int x, int y);

// -- SELECTION CUT -- //

// Cut selection from source image.
SDL_Surface* cut_selection(SDL_Surface* s_surface, int x, int y, int x2, int y2);

#endif