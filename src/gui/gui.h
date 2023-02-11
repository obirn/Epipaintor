#ifndef GUI_H
#define GUI_H

// Tools:
#define NONE 0
#define BRUSH 1
#define BUCKET 2

#include <gtk/gtk.h>

int init_interface(int argc,char**argv);

// Callbacks
gboolean on_open_file_file_activated(GtkFileChooserButton * b);
gboolean draw_callback(GtkWidget* widget, cairo_t *cr, gpointer data);
gboolean mouse_on_press(GtkWidget* self, GdkEvent* event, gpointer user_data);
gboolean mouse_on_release(GtkWidget* self, GdkEvent* event, gpointer user_data);
gboolean mouse_on_move(GtkWidget *widget,GdkEvent *event, gpointer user_data);
gboolean on_Color_set(GtkColorChooser *self, gpointer user_data);
gboolean on_brush(GtkButton *self, gpointer user_data);
gboolean on_bucket(GtkButton *self, gpointer user_data);
gboolean on_Color_set(GtkColorChooser *self, gpointer user_data);

#endif