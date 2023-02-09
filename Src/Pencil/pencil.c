#include <gtk/gtk.h>

// Widgets
GtkWindow* window;
GtkWidget* draw_area;


// Main function.
int main()
{
    // Initializes GTK.
    gtk_init(NULL, NULL);

    GtkBuilder* builder = gtk_builder_new();
    GError* error = NULL;

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

    gtk_widget_set_app_paintable(draw_area, TRUE);

    // Show the window and start the gtk instance.
    gtk_widget_show_all((GtkWidget *) window);
    gtk_main();
    return 0;
}
