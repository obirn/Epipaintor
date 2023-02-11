#ifndef PENCIL_H
#define PENCIL_H

SDL_Color sdl_color = {.r = 0, .g = 0, .b = 0};
SDL_Color white = {.r = 255, .g = 255, .b = 255};

// Widgets
GtkWindow* window;
GtkWidget* draw_area;

// Variable that hold unused values
GtkWidget* widget;
gpointer data;
GdkEvent* event;

// Buffer to paste on the GtkDrawingArea 'draw_area'
SDL_Surface* image_buff;

gboolean mouse_on_press(GtkWidget* self, GdkEvent* event, gpointer user_data);
gboolean mouse_on_release(GtkWidget* self, GdkEvent* event, gpointer user_data);
gboolean mouse_on_move(GtkWidget *widget,GdkEvent *event, gpointer user_data);

#endif