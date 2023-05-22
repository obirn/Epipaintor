#ifndef SELECTION_H
#define SELECTION_H

// -- SELECTION BASE CREATE -- //

// Create base of selection.
void create_selection_base(SDL_Surface *s_surface);

// Clear selection.
void clear_selection_base(SDL_Surface* s_surface);

// -- SELECTION DISPLAY -- //

/*
** Display selection.
** Status 1 meaning the display is already active and should be turned off.
** Other status meaning the display should be turned on.
*/
SDL_Surface* display_selection_on_image(SDL_Surface *s_surface, int status);

#endif