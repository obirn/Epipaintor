#ifndef GUI_H
#define GUI_H

enum tools {NONE, BRUSH, BUCKET, RECTANGLE, CIRCLE, TRIANGLE, LINE,SELECTION,COPY,PASTE,CUT};

#include <gtk/gtk.h>

int init_interface(int argc,char**argv);

// Callbacks
gboolean on_tool_clicked(GtkButton *self, gpointer user_data);
gboolean on_open_file_file_activated(GtkFileChooserButton * b);
gboolean draw_callback(GtkWidget* widget, cairo_t *cr, gpointer data);
gboolean on_mouse_press(GtkWidget* self, GdkEvent* event, gpointer user_data);
gboolean on_mouse_release(GtkWidget* self, GdkEvent* event, gpointer user_data);
gboolean on_mouse_move(GtkWidget *widget,GdkEvent *event, gpointer user_data);
gboolean on_color_set(GtkColorChooser *self, gpointer user_data);
gboolean on_color_set(GtkColorChooser *self, gpointer user_data);
gboolean on_previous(GtkButton *self, gpointer user_data);
gboolean on_next(GtkButton *self, gpointer user_data);
gboolean on_key_press (GtkWidget *widget, GdkEventKey *event, gpointer user_data);
gboolean update_scale_val(GtkScale *self, gpointer user_data);
gboolean gaussian_blur_apply(GtkScale *self, gpointer user_data);
gboolean gamma_apply(GtkScale *self, gpointer user_data);
gboolean threshold_apply(GtkScale *self, gpointer user_data);
gboolean color_apply(GtkScale *self, gpointer user_data);
unsigned char threshold_value(GtkScale *self);
unsigned char gamma_value(GtkScale *self);
unsigned char gaussian_blur_value(GtkScale *self);
unsigned char  color_threshold_value(GtkScale *self);
double  resize_value(GtkScale *self);
unsigned char  r1_value(GtkScale *self);
unsigned char  r2_value(GtkScale *self);
unsigned char  g1_value(GtkScale *self);
unsigned char  g2_value(GtkScale *self);
unsigned char  b1_value(GtkScale *self);
unsigned char  b2_value(GtkScale *self);
void on_new_file(GtkMenuItem *self);
void on_open_file(GtkMenuItem *menu_item, gpointer user_data);
void on_save_file(GtkMenuItem *menu_item, gpointer user_data);
void epipaintor_free(gpointer user_data);
void on_rotate_left_clicked(GtkMenuItem *self);
void on_rotate_right_clicked(GtkMenuItem *self);
#endif
