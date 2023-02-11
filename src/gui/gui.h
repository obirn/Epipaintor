#ifndef GUI_H
#define GUI_H

#include <gtk/gtk.h>

int init_interface(int argc,char**argv);
gboolean on_open_file_file_activated(GtkFileChooserButton * b);
gboolean draw_callback(GtkWidget* widget, cairo_t *cr, gpointer data);
gboolean mouse_on_press(GtkWidget* self, GdkEvent* event, gpointer user_data);
gboolean mouse_on_release(GtkWidget* self, GdkEvent* event, gpointer user_data);
gboolean mouse_on_move(GtkWidget *widget,GdkEvent *event, gpointer user_data);

#endif