#include <gtk/gtk.h>
#include <string.h>
#include "gui.h"
#include "../image_utils/draw_tools.h"
#include "../image_utils/pixel_operations.h"

// Glade relatd
GtkBuilder *builder;
GtkWidget *window;
GtkWidget *fixed1;
GtkWidget *openfilebutton;
GtkWidget *image;
GtkWidget *label;
GtkWidget *button;
GtkWidget *fixed2;
GtkWidget *draw_area;
char* image_path;

int pixelsize;

// SDL Rlatd
SDL_Surface* img_buff;


int init_interface(int argc, char**argv)
{
	gtk_init(&argc,&argv);

	builder = gtk_builder_new_from_file("gui/gui.glade");

	// Load wigdets
	window = GTK_WIDGET(gtk_builder_get_object(builder,"mainpage"));
	fixed1 = GTK_WIDGET(gtk_builder_get_object(builder,"fixed1"));
	openfilebutton = GTK_WIDGET(gtk_builder_get_object(builder,"open_file"));
	button = GTK_WIDGET(gtk_builder_get_object(builder,"button"));
	image = GTK_WIDGET(gtk_builder_get_object(builder,"image_window"));
	label = GTK_WIDGET(gtk_builder_get_object(builder,"label"));
	draw_area = GTK_WIDGET(gtk_builder_get_object(builder,"draw_area"));

	// Connecting signals
	gtk_builder_connect_signals(builder,NULL);
	g_signal_connect(window,"destroy",G_CALLBACK(gtk_main_quit),NULL);
	g_signal_connect(openfilebutton,"file-set",G_CALLBACK(on_open_file_file_activated),NULL);
    g_signal_connect (G_OBJECT (draw_area), "draw", G_CALLBACK (draw_callback), NULL);

	// Window settings
	gtk_window_set_default_size(GTK_WINDOW(window),1920,1080);//keep it like this please.
	gtk_window_set_resizable(GTK_WINDOW(window),TRUE);

	/*      Modification before this line */
	gtk_widget_show(window); // shows the window
	gtk_main();
	return EXIT_SUCCESS;
}

gboolean draw_callback(GtkWidget* widget, cairo_t *cr, gpointer data)
{
    //Unused parameters :
    widget = widget;
    data = data;

	if (!img_buff) return FALSE;

    //Actual function :
    SDL_SaveBMP(img_buff, "../cache/img_buff.bmp");
    GdkPixbuf *pixbuf;

    pixbuf = gdk_pixbuf_new_from_file("../cache/img_buff.bmp", NULL);

    gdk_cairo_set_source_pixbuf(cr, pixbuf, 0, 0);

    cairo_paint(cr);

    if (pixbuf)
        g_object_unref(pixbuf);

    return FALSE;
}

gboolean on_open_file_file_activated(GtkFileChooserButton * b)
{
	
	image_path = gtk_file_chooser_get_filename(GTK_FILE_CHOOSER(b));

	img_buff = load_image(image_path);

    gtk_widget_queue_draw_area(draw_area,0,0,img_buff->w,img_buff->h);
    
    int w = 1310;
    int h = 903;
    if(img_buff->w > 1080)
    {
        w = img_buff->w+230;
        gtk_widget_set_margin_start(GTK_WIDGET(draw_area), 0);
    }
    else
        gtk_widget_set_margin_start(GTK_WIDGET(draw_area), (w-230-img_buff->w)/2);

    if(img_buff->h > 850)
    {
        h = img_buff->h+53;
        gtk_widget_set_margin_top(GTK_WIDGET(draw_area), 0);
    }
    else
        gtk_widget_set_margin_top(GTK_WIDGET(draw_area), (h-53-img_buff->h)/2);

    gtk_window_resize(GTK_WINDOW(window), w, h);     

    g_free(image_path);

    return FALSE;
	// char* path = (char*)image_path->data;
	// if (image)
		// gtk_container_remove(GTK_CONTAINER(fixed1),image);

	/*
	image = gtk_image_new_from_file(path);
	gtk_container_add(GTK_CONTAINER(fixed1),image);

	pixelsize = gtk_image_get_pixel_size(image);
	gtk_image_set_pixel_size(image,pixelsize/50);

	gtk_widget_show(image);
	gtk_fixed_move(GTK_FIXED(fixed1),image,700,380);
	*/
}


