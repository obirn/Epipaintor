#ifndef GUI_H
#define GUI_H

#include <gtk/gtk.h>

int init_interface(int argc,char**argv);
gboolean on_open_file_file_activated(GtkFileChooserButton * b);
gboolean draw_callback(GtkWidget* widget, cairo_t *cr, gpointer data);

#endif