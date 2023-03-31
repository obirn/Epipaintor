#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <gtk/gtk.h>
#include <string.h>
#include "./gui.h"
#include "../auxiliary/auxiliary.h"
#include "../data_structs/queue.h"
#include "../data_structs/p_queue.h"
#include "../data_structs/shared_stack.h"
#include "../image_utils/tools.h"
#include "../filters/filters.h"

// Glade related
GtkBuilder *builder;
GtkWidget *window;
GtkWidget *fixed1;
GtkWidget *image;
GtkWidget *label;
GtkWidget *button;
GtkWidget *fixed2;
GtkWidget *draw_area;
GtkWidget *brush;
GtkWidget *gray_scale;
GtkWidget *redlight;
GtkWidget *save_file_button;
GtkColorChooser* color_button;
GtkButton* bucket;
GtkWidget* previous;
GtkWidget* next;
GtkScale* scale_glider;


// Shared stack used to stock modifications
shared_stack* before;
shared_stack* after;

char* image_path;

int selected_tool = NONE;
unsigned char scale_nb = 10;
int brush_size = 2;

// SDL Related
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

// Unused variables to avoid warnings
GtkWidget* widget;
gpointer data;
GdkEvent* event;

// Colors
GdkRGBA gdk_color = {.red = 0, .green = 0, .blue = 0, .alpha = 1};

SDL_Color selected_color = {.r = 0, .g = 0, .b = 0};
SDL_Color color_buff = {.r = 0, .g = 0, .b = 0};
SDL_Color white = {.r = 255, .g = 255, .b = 255};


int init_interface(int argc, char**argv)
{
	gtk_init(&argc,&argv);

	builder = gtk_builder_new_from_file("gui/gui.glade");

	// Load wigdets
	window = GTK_WIDGET(gtk_builder_get_object(builder,"mainpage"));
	fixed1 = GTK_WIDGET(gtk_builder_get_object(builder,"fixed1"));
	button = GTK_WIDGET(gtk_builder_get_object(builder,"button"));
	image = GTK_WIDGET(gtk_builder_get_object(builder,"image_window"));
	label = GTK_WIDGET(gtk_builder_get_object(builder,"label"));
	draw_area = GTK_WIDGET(gtk_builder_get_object(builder,"draw_area"));
	brush = GTK_WIDGET(gtk_builder_get_object(builder,"brush_button"));
	save_file_button = GTK_WIDGET(gtk_builder_get_object(builder,"save_window"));
	color_button = GTK_COLOR_CHOOSER(gtk_builder_get_object(builder, "color_button"));
	bucket = GTK_BUTTON(gtk_builder_get_object(builder, "bucket_button"));
	gray_scale = GTK_WIDGET(gtk_builder_get_object(builder, "grayscale")); 
	redlight = GTK_WIDGET(gtk_builder_get_object(builder, "red_light")); 
	next = GTK_WIDGET(gtk_builder_get_object(builder, "redo")); 
	previous = GTK_WIDGET(gtk_builder_get_object(builder, "undo"));
    scale_glider = GTK_SCALE(gtk_builder_get_object(builder, "Scale")); 

	// Create events
	gtk_widget_add_events(draw_area, GDK_POINTER_MOTION_MASK);
	gtk_widget_add_events(draw_area, GDK_BUTTON_PRESS_MASK);
	gtk_widget_add_events(draw_area, GDK_BUTTON_RELEASE_MASK);

	// Connecting signals
	gtk_builder_connect_signals(builder,NULL);
	g_signal_connect(window,"destroy",G_CALLBACK(gtk_main_quit),NULL);
	g_signal_connect (G_OBJECT (draw_area), "draw", G_CALLBACK (draw_callback), NULL);
	g_signal_connect (draw_area, "motion-notify-event", G_CALLBACK(mouse_on_move), NULL);
	g_signal_connect (draw_area, "button-press-event", G_CALLBACK(mouse_on_press), NULL);
	g_signal_connect (draw_area, "button-release-event", G_CALLBACK(mouse_on_release), NULL);
	g_signal_connect(brush, "clicked", G_CALLBACK(on_brush), NULL);
	g_signal_connect(bucket, "clicked", G_CALLBACK(on_bucket), NULL);
	g_signal_connect(color_button, "color-set", G_CALLBACK(on_Color_set), NULL);
    g_signal_connect(previous, "clicked", G_CALLBACK(on_previous), NULL);
    g_signal_connect(next, "clicked", G_CALLBACK(on_next), NULL);
    g_signal_connect (G_OBJECT (window), "key_press_event", G_CALLBACK (on_key_press), NULL);
    g_signal_connect(scale_glider, "value_changed", G_CALLBACK(update_scale_val), NULL);

	// Window settings
	gtk_window_set_default_size(GTK_WINDOW(window),1920,1080);//keep it like this please.
	gtk_window_set_resizable(GTK_WINDOW(window),FALSE);
	gtk_window_set_icon_from_file(GTK_WINDOW(window),"../assets/logo_200x200.png",NULL);

	// Stacks Initialization
    before = shared_stack_new();
    after = shared_stack_new();

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
	// Save unused variables to avoid warning
	widget = self;
	event = event;
	data = user_data;

	is_pressed = TRUE;
	start_x = pos_x;
	start_y = pos_y;

	//printf("(x, y) = (%i, %i)\n", start_x, start_y);

	if (selected_tool == NONE)
		return FALSE;

	shared_stack_push(before, img_buff);
	shared_stack_empty(after);


	switch (selected_tool) {
		case BRUSH:
			break;
		case BUCKET:
			img_buff = bucket_fill(img_buff, start_x, start_y, (Uint8) selected_color.r, \
					(Uint8) selected_color.g, (Uint8) selected_color.b,70);
			gtk_widget_queue_draw_area(draw_area, 0, 0, img_buff->w, img_buff->h);
			break;
	}

	return FALSE;
}

gboolean mouse_on_release(GtkWidget* self, GdkEvent* event, gpointer user_data)
{
	// Used to avoid compilations warning.
	widget = self;
	event = event;
	data = user_data;

	is_pressed = FALSE;
	start_x = pos_x;
	start_y = pos_y;

	return FALSE;
}


gboolean on_Color_set(GtkColorChooser *self, gpointer user_data)
{
	data = user_data;

	gtk_color_chooser_get_rgba(self, &gdk_color);
	selected_color.r = (Uint8) (gdk_color.red * 255);
	selected_color.g = (Uint8) (gdk_color.green * 255);
	selected_color.b = (Uint8) (gdk_color.blue * 255);
	return FALSE;
}


gboolean mouse_on_move(GtkWidget *widget,GdkEvent *event, gpointer user_data) 
{

	//Unused parameters :
	widget = widget;
	data = user_data;
	
	if (!(event->type==GDK_MOTION_NOTIFY))
		return TRUE;

	GdkEventMotion* e =(GdkEventMotion*)event;
	old_x = pos_x;
	old_y = pos_y;
	pos_x = (guint) e->x;
	pos_y = (guint) e->y;


	if (!img_buff || !is_pressed) return FALSE;

	//printf("Old coordinates: (%u,%u)\n", old_x, old_y);
	//printf("coordinates: (%u,%u)\n", pos_x, pos_y);
	switch (selected_tool) {
		case BRUSH:
			drawline(img_buff, selected_color, old_x, old_y, pos_x, pos_y, brush_size);
			gtk_widget_queue_draw_area(draw_area,0,0,img_buff->w,img_buff->h);
			return TRUE;
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

gboolean on_bucket(GtkButton *self, gpointer user_data) {
	// Unused variables
	widget = (GtkWidget *) self;
	data = user_data;

	selected_tool = BUCKET; 

	return FALSE;
}


void on_file_chooser_menu_item_activate(GtkMenuItem *menu_item, gpointer user_data)
{
	widget = (GtkWidget *) menu_item;
	data = user_data;

	GtkWidget *file_chooser_dialog = gtk_file_chooser_dialog_new("Open File",
			NULL,
			GTK_FILE_CHOOSER_ACTION_OPEN,
			"gtk-cancel", GTK_RESPONSE_CANCEL,
			"gtk-open", GTK_RESPONSE_OK,
			NULL);
	gint response = gtk_dialog_run(GTK_DIALOG(file_chooser_dialog));

	if (response != GTK_RESPONSE_OK) {
		gtk_widget_destroy(file_chooser_dialog);
		return;
	}

	char *image_path = gtk_file_chooser_get_filename(GTK_FILE_CHOOSER(file_chooser_dialog));
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

	gtk_widget_destroy(file_chooser_dialog);
}

void on_save_button_clicked(GtkButton *b)
{
	// Unused variables
	widget = (GtkWidget *) b;

	SDL_Surface * img_buff = SDL_LoadBMP("../cache/img_buff.bmp");
	SDL_SaveBMP(img_buff,"../save/saved_img.bmp");

}
void on_grayscale_activate(GtkMenuItem *self)
{
	if(img_buff==NULL)
		return;
	widget = (GtkWidget *) self;
	img_buff = grayscale(img_buff);
	gtk_widget_queue_draw_area(draw_area,0,0,img_buff->w,img_buff->h);
}


void on_red_light_activate(GtkMenuItem *self)
{
	if(img_buff==NULL)
		return;
	widget = (GtkWidget *) self;
	img_buff = red_light(img_buff);
	gtk_widget_queue_draw_area(draw_area,0,0,img_buff->w,img_buff->h);
}


void on_sepia_activate(GtkMenuItem *self)
{
	if(img_buff==NULL)
		return;
	widget = (GtkWidget *) self;
	img_buff = sepia(img_buff);
	gtk_widget_queue_draw_area(draw_area,0,0,img_buff->w,img_buff->h);
}




void on_gaussian_blur_activate(GtkMenuItem *self)
{
	if(img_buff==NULL)
		return;
	widget = (GtkWidget *) self;
	img_buff = gaussian(img_buff,10);
	gtk_widget_queue_draw_area(draw_area,0,0,img_buff->w,img_buff->h);
}
void on_binarize_activate(GtkMenuItem *self)
{
	if(img_buff==NULL)
		return;
	widget = (GtkWidget *) self;
	img_buff = binarize(img_buff,10,img_buff->h,img_buff->w);
	gtk_widget_queue_draw_area(draw_area,0,0,img_buff->w,img_buff->h);
}

void on_negative_activate(GtkMenuItem *self)
{
	if(img_buff==NULL)
		return;
	widget = (GtkWidget *) self;
	img_buff = negative(img_buff);
	gtk_widget_queue_draw_area(draw_area,0,0,img_buff->w,img_buff->h);
}


void on_gamma_activate(GtkMenuItem *self)
{
	if(img_buff==NULL)
		return;
	widget = (GtkWidget *) self;
	img_buff = gam(img_buff,10);
	gtk_widget_queue_draw_area(draw_area,0,0,img_buff->w,img_buff->h);
}

void on_blankpage_activate(GtkMenuItem *self)
{

	widget = (GtkWidget *) self;
	char *image_path = "../assets/medium_blank.png";
	img_buff = load_image(image_path);
	gtk_widget_queue_draw_area(draw_area,0,0,img_buff->w,img_buff->h);
}


gboolean on_previous(GtkButton* self, gpointer user_data)
{
    //use this fonction to revert last modification
	widget = (GtkWidget *) self;
	data = user_data;


    if (before->size > 0)
    {
        int oldh = img_buff->h;
        int oldw = img_buff->w;

        shared_stack_push(after, img_buff);
        SDL_FreeSurface(img_buff);
        img_buff = shared_stack_pop(before);

        if (img_buff->h > oldh)
            oldh = img_buff->h;
        if (img_buff->w > oldw)
            oldw = img_buff->w;

        
		gtk_widget_queue_draw_area(draw_area, 0, 0, oldw, oldh);
        // image_resize();
    }

    return FALSE;
}

gboolean on_next(GtkButton* self, gpointer user_data)
{
    //use this fonction to re-do last modification that was reverted
    if (self && user_data)  
        return FALSE;

    if (after->size > 0)
    {
        //printf("%li\n", after->size);
        int oldh = img_buff->h;
        int oldw = img_buff->w;
        shared_stack_push(before, img_buff);
        SDL_FreeSurface(img_buff);
        img_buff = shared_stack_pop(after);

        if (img_buff->h > oldh)
            oldh = img_buff->h;
        if (img_buff->w > oldw)
            oldw = img_buff->w;

		gtk_widget_queue_draw_area(draw_area, 0, 0, oldw, oldh);
        // image_resize();
    }
    return FALSE;
}

gboolean on_key_press (GtkWidget *widget, GdkEventKey *event, gpointer user_data)
{
	// Used to avoid compilation warning
	widget = widget;
	data = user_data;

	if (!(event->type == GDK_KEY_PRESS && GDK_CONTROL_MASK))
		return FALSE;


    switch (event->keyval)
    {
        case GDK_KEY_z:
			on_previous(NULL, NULL);
			// image_resize();
            return FALSE;
        case GDK_KEY_y:
			on_next(NULL, NULL);
			// image_resize();
            return FALSE;
        case GDK_KEY_v:
                //printf("key pressed: %s\n", "ctrl + v");
                // shared_stack_push(before, img);                                    
                // shared_stack_empty(after);
                // shared_stack_push(b2, img2);                                    
                // shared_stack_empty(a2);

                // past_selection(img, copy_crop_img, pos_x, pos_y);
                // gtk_widget_queue_draw_area(image,0,0,img->w,img->h);
            return FALSE;
    }

    return FALSE;
}

gboolean update_scale_val(GtkScale *self, gpointer user_data)
{
    //Unused parameters :
	widget = (GtkWidget *) self;
    user_data = user_data;

    //Actual function :
    scale_nb = gtk_range_get_value(&(self->range));
	brush_size = (scale_nb + 10) / 10;


    return FALSE;
}