#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <gtk/gtk.h>
#include <string.h>
#include "gui.h"
#include "../auxiliary/auxiliary.h"
#include "../image_utils/draw_tools.h"



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
GtkWidget *brush;

char* image_path;

int selected_tool = NONE;

// SDL Rlatd
SDL_Surface* img_buff;


// Booleans
int is_pressed;

// Mouse coordinates
int pos_x = 0;
int pos_y = 0;
int old_x = 0;
int old_y = 0;

int start_x = 0;
int start_y = 0;
int end_x = 0;
int end_y = 0;

// Unusd variabls to avoid warning
GtkWidget* widget;
gpointer data;
GdkEvent* event;

// Colors
SDL_Color sdl_color = {.r = 0, .g = 0, .b = 0};
SDL_Color white = {.r = 255, .g = 255, .b = 255};


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
	brush = GTK_WIDGET(gtk_builder_get_object(builder,"brush_button"));

	// Create events
    gtk_widget_add_events(draw_area, GDK_POINTER_MOTION_MASK);
    gtk_widget_add_events(draw_area, GDK_BUTTON_PRESS_MASK);
    gtk_widget_add_events(draw_area, GDK_BUTTON_RELEASE_MASK);

	// Connecting signals
	gtk_builder_connect_signals(builder,NULL);
	g_signal_connect(window,"destroy",G_CALLBACK(gtk_main_quit),NULL);
	g_signal_connect(openfilebutton,"file-set",G_CALLBACK(on_open_file_file_activated),NULL);
    g_signal_connect (G_OBJECT (draw_area), "draw", G_CALLBACK (draw_callback), NULL);
	g_signal_connect (draw_area, "motion-notify-event", G_CALLBACK(mouse_on_move), NULL);
    g_signal_connect (draw_area, "button-press-event", G_CALLBACK(mouse_on_press), NULL);
    g_signal_connect (draw_area, "button-release-event", G_CALLBACK(mouse_on_release), NULL);
	g_signal_connect(brush, "clicked", G_CALLBACK(on_brush), NULL);

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
	if (!img_buff) return FALSE;
	
    //Unused parameters :
    widget = widget;
	

    if (event->type==GDK_MOTION_NOTIFY && user_data == NULL) 
    {
        GdkEventMotion* e =(GdkEventMotion*)event;
        old_x = pos_x;
        old_y = pos_y;
        pos_x = (guint) e->x;
        pos_y = (guint) e->y;

        //printf("Old coordinates: (%u,%u)\n", old_x, old_y);
        //printf("coordinates: (%u,%u)\n", pos_x, pos_y);
		switch (selected_tool) {
			case BRUSH:
				if (is_pressed)
				{
					drawline(img_buff, sdl_color, old_x, old_y, pos_x, pos_y, 5);
					gtk_widget_queue_draw_area(draw_area,0,0,img_buff->w,img_buff->h);
				}
				break;
		}
        
    }
    return TRUE;
}

gboolean on_brush(GtkButton *self, gpointer user_data) {
	// Unused variables
	widget = (GtkWidget *) self;
	data = user_data;

	selected_tool = BRUSH; 

	return FALSE;
}
