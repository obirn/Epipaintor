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
gboolean on_previous(GtkButton *self, gpointer user_data);
gboolean on_next(GtkButton *self, gpointer user_data);
gboolean on_key_press (GtkWidget *widget, GdkEventKey *event, gpointer user_data);
gboolean update_scale_val(GtkScale *self, gpointer user_data);
gboolean gaussian_blur_apply(GtkScale *self, gpointer user_data);
gboolean gamma_apply(GtkScale *self, gpointer user_data);
unsigned char gamma_value(GtkScale *self);
unsigned char gaussian_blur_value(GtkScale *self);
void on_new_file(GtkMenuItem *self);
void on_open_file(GtkMenuItem *menu_item, gpointer user_data);
void on_save_file(GtkMenuItem *menu_item, gpointer user_data);
void epipaintor_free(gpointer user_data);

#endif
