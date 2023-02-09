#include <gtk/gtk.h>

// Widgets
GtkWindow* window;
GtkWidget* draw_area;

// Variable that hold unused values
GtkWidget* widget;
gpointer data;
GtkEvent* event;

SDL_Surface* image;

// Main function.
int main()
{
    // Initializes GTK.
    gtk_init(NULL, NULL);

    GtkBuilder* builder = gtk_builder_new();
    GError* error = NULL;

    image = load_image("./blank.png");

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
    g_signal_connect (G_OBJECT (image), "draw", G_CALLBACK (draw_callback), NULL);

    gtk_widget_set_app_paintable(draw_area, TRUE);

    // Show the window and start the gtk instance.
    gtk_widget_show_all((GtkWidget *) window);
    gtk_main();
    return 0;
}


gboolean draw_callback(GtkWidget* widget, cairo_t *cr, gpointer data)
{
    //Unused parameters :
    widget = widget;
    data = data;

    //Actual function :
    SDL_SaveBMP(img, "./tmpfile.bmp");
    GdkPixbuf *pixbuf;

    pixbuf = gdk_pixbuf_new_from_file("./GTK/tmpfile.bmp", NULL);
    

    gdk_cairo_set_source_pixbuf(cr, pixbuf, 0, 0);

    cairo_paint(cr);

    if (pixbuf)
        g_object_unref(pixbuf);

    return FALSE;
}

gboolean mouse_press(GtkWidget* self, GdkEvent* event, gpointer user_data)
{
    widget = self;
    event = event;

    if(user_data == NULL)
    {
        is_pressed = TRUE;
        start_x = pos_x;
        start_y = pos_y;
        //printf("Start coordinates: (%u,%u)\n", start_x, start_y);

        switch (tool_value)         
        {
            case -1:
                break;

             case 2:
                // bucket call
                shared_stack_push(before, img);
                shared_stack_empty(after);                                        
                filling_seal(img, start_x, start_y, sdl_color, ((int)scale_nb * 255) / 100);   
                gtk_widget_queue_draw_area(image,0,0,img->w,img->h);
                break;
                
         }
    }

    return FALSE;
}
