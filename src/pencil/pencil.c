#include <gtk/gtk.h>
#include <stdio.h>
#include <stdlib.h>
#include "pixel_operations.h"
#include "pencil.h"
#include "draw_tools.h"

// Mouse coordinates
int pos_x = 0;
int pos_y = 0;
int old_x = 0;
int old_y = 0;

int start_x = 0;
int start_y = 0;
int end_x = 0;
int end_y = 0;

// Booleans
int is_pressed;


gboolean draw_callback(GtkWidget* widget, cairo_t *cr, gpointer data)
{
    //Unused parameters :
    widget = widget;
    data = data;

    //Actual function :
    SDL_SaveBMP(image_buff, "./image_buff.bmp");
    GdkPixbuf *pixbuf;

    pixbuf = gdk_pixbuf_new_from_file("./image_buff.bmp", NULL);
    

    gdk_cairo_set_source_pixbuf(cr, pixbuf, 0, 0);

    cairo_paint(cr);

    if (pixbuf)
        g_object_unref(pixbuf);

    return FALSE;
}

// Main function.
int main()
{
    // Initializes GTK.
    gtk_init(NULL, NULL);

    GtkBuilder* builder = gtk_builder_new();
    GError* error = NULL;

    image_buff = load_image("./blank.png");

    if (gtk_builder_add_from_file(builder, "gtk.glade", &error) == 0)
    {
        g_printerr("Error loading file: %s\n", error->message);
        g_clear_error(&error);
        return 1;
    }

    // Gets the widgets.
    window = GTK_WINDOW(gtk_builder_get_object(builder, "main_window"));
    draw_area = GTK_WIDGET(gtk_builder_get_object(builder, "drawing_area"));

    // Create events
    gtk_widget_add_events(draw_area, GDK_POINTER_MOTION_MASK);
    gtk_widget_add_events(draw_area, GDK_BUTTON_PRESS_MASK);
    gtk_widget_add_events(draw_area, GDK_BUTTON_RELEASE_MASK);

    // Connects signal handlers.
    g_signal_connect(window, "destroy", G_CALLBACK(gtk_main_quit), NULL);
    g_signal_connect (G_OBJECT (draw_area), "draw", G_CALLBACK (draw_callback), NULL);
    g_signal_connect (draw_area, "motion-notify-event", G_CALLBACK(mouse_on_move), NULL);
    g_signal_connect (draw_area, "button-press-event", G_CALLBACK(mouse_on_press), NULL);
    g_signal_connect (draw_area, "button-release-event", G_CALLBACK(mouse_on_release), NULL);

    gtk_widget_set_app_paintable(draw_area, TRUE);

    gtk_builder_connect_signals(builder,NULL);

    // Show the window and start the gtk instance.
    gtk_widget_show_all((GtkWidget *) window);
    gtk_main();
    return 0;
}


gboolean mouse_on_press(GtkWidget* self, GdkEvent* event, gpointer user_data)
{
    // printf("Mouse on press\n");
    widget = self;
    event = event;

    if(user_data == NULL)
    {
        is_pressed = TRUE;
        start_x = pos_x;
        start_y = pos_y;
        //printf("Start coordinates: (%u,%u)\n", start_x, start_y);
    }

    return FALSE;
}

gboolean mouse_on_release(GtkWidget* self, GdkEvent* event, gpointer user_data)
{
    widget = self;
    event = event;

    // printf("Mouse on release\n");
    if(user_data == NULL)
    {
        is_pressed = FALSE;
        start_x = pos_x;
        start_y = pos_y;
        //printf("Start coordinates: (%u,%u)\n", start_x, start_y);
    }

    return FALSE;
}



gboolean mouse_on_move(GtkWidget *widget,GdkEvent *event, gpointer user_data) 
{
    //Unused parameters :

    // printf("Mouse on move\n");
    widget = widget;

    //Actual function :
    if (event->type==GDK_MOTION_NOTIFY && user_data == NULL) 
    {
        GdkEventMotion* e =(GdkEventMotion*)event;
        old_x = pos_x;
        old_y = pos_y;
        pos_x = (guint) e->x;
        pos_y = (guint) e->y;

        //printf("Old coordinates: (%u,%u)\n", old_x, old_y);
        //printf("coordinates: (%u,%u)\n", pos_x, pos_y);

        if (is_pressed)
        {
            drawline(image_buff, sdl_color, old_x, old_y, pos_x, pos_y, 5);
            gtk_widget_queue_draw_area(draw_area,0,0,image_buff->w,image_buff->h);
        }
    }
    return TRUE;
}
