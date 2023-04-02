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
#include "../shapes/shapes.h"

// Glade related
GtkMenuItem *self;
GtkBuilder *builder;
GtkWidget *window;
GtkWidget *color_window;
GtkWidget *color_filter;
GtkWidget *fixed1;
GtkWidget *image;
GtkWidget *label;
GtkWidget *button;
GtkWidget *fixed2;
GtkWidget *draw_area;
GtkWidget *brush;
GtkWidget *gray_scale;
GtkWidget *otsu;
GtkWidget *redlight;
GtkWidget *new_file;
GtkWidget *open_file;
GtkWidget *save_file;
GtkWidget *quit;
GtkColorChooser* color_button;
GtkButton* bucket;
GtkButton* rectangle_button;
GtkButton* triangle_button;
GtkButton* circle_button;
GtkWidget* previous;
GtkWidget* next;
GtkScale* scale_glider;
GtkScale* gaussian_blur_slider;
GtkWidget* color_threshold_window;
GtkWidget* gaussian_blur_window;
GtkWidget* custom_filter_window;
GtkWidget* gamma_window;
GtkWidget* threshold_window;
GtkWidget *apply_button;
GtkWidget *apply_button_gamma;
GtkWidget *apply_button_threshold;
GtkWidget *apply_button_color;
GtkScale* gamma_slider;
GtkScale* threshold_slider;
GtkScale* color_threshold_slider;
GtkScale* r1_slider;
GtkScale* r2_slider;
GtkScale* g1_slider;
GtkScale* g2_slider;
GtkScale* b1_slider;
GtkScale* b2_slider;
// Shared stack used to stock modifications
shared_stack* before;
shared_stack* after;

char* image_path;

int selected_tool = NONE;
unsigned char scale_nb = 10;
int brush_size = 2;

// SDL Related
SDL_Surface* img_buff= NULL;
SDL_Surface* pre_visualisation = NULL;

// Booleans
int is_pressed = FALSE;

// Mouse coordinates
int curr_x = 0;
int curr_y = 0;

int last_x = 0;
int last_y = 0;

int on_press_x = 0;
int on_press_y = 0;
int end_x = 0;
int end_y = 0;

// Unused variables to avoid warnings
GtkWidget* widget;
gpointer data;
GdkEvent* event;

// Colors
GdkRGBA gdk_color = {.red = 0, .green = 0, .blue = 0, .alpha = 1};

SDL_Color selected_color = {.r = 0, .g = 0, .b = 0};
SDL_Color white = {.r = 255, .g = 255, .b = 255};


int init_interface(int argc, char**argv)
{
	gtk_init(&argc,&argv);

	builder = gtk_builder_new_from_file("gui/gui.glade");

	// Load wigdets
	window = GTK_WIDGET(gtk_builder_get_object(builder,"mainpage"));
	color_window=  GTK_WIDGET(gtk_builder_get_object(builder,"color_window"));
	fixed1 = GTK_WIDGET(gtk_builder_get_object(builder,"fixed1"));
	button = GTK_WIDGET(gtk_builder_get_object(builder,"button"));
	image = GTK_WIDGET(gtk_builder_get_object(builder,"image_window"));
	label = GTK_WIDGET(gtk_builder_get_object(builder,"label"));
	draw_area = GTK_WIDGET(gtk_builder_get_object(builder,"draw_area"));
	brush = GTK_WIDGET(gtk_builder_get_object(builder,"brush_button"));
	color_button = GTK_COLOR_CHOOSER(gtk_builder_get_object(builder, "color_button"));
	bucket = GTK_BUTTON(gtk_builder_get_object(builder, "bucket_button"));
	rectangle_button = GTK_BUTTON(gtk_builder_get_object(builder, "square"));
	circle_button = GTK_BUTTON(gtk_builder_get_object(builder, "circle"));
	triangle_button = GTK_BUTTON(gtk_builder_get_object(builder, "triangle"));
	gray_scale = GTK_WIDGET(gtk_builder_get_object(builder, "grayscale")); 
	gray_scale = GTK_WIDGET(gtk_builder_get_object(builder, "otsu_threshold")); 
	redlight = GTK_WIDGET(gtk_builder_get_object(builder, "red_light")); 
	next = GTK_WIDGET(gtk_builder_get_object(builder, "redo")); 
	previous = GTK_WIDGET(gtk_builder_get_object(builder, "undo"));
    scale_glider = GTK_SCALE(gtk_builder_get_object(builder, "Scale"));
	quit  = GTK_WIDGET(gtk_builder_get_object(builder,"quit"));
	save_file = GTK_WIDGET(gtk_builder_get_object(builder,"save_file"));
	new_file = GTK_WIDGET(gtk_builder_get_object(builder,"new_file"));
	open_file = GTK_WIDGET(gtk_builder_get_object(builder,"open_file"));
	quit = GTK_WIDGET(gtk_builder_get_object(builder,"quit"));
	gaussian_blur_window = GTK_WIDGET(gtk_builder_get_object(builder,"gaussian_blur_window"));
	gaussian_blur_slider =GTK_SCALE(gtk_builder_get_object(builder,"gaussian_blur_slider"));
	apply_button = GTK_WIDGET(gtk_builder_get_object(builder,"apply_button"));
	apply_button_gamma = GTK_WIDGET(gtk_builder_get_object(builder,"apply_button_gamma"));
	apply_button_color = GTK_WIDGET(gtk_builder_get_object(builder,"apply_button_color"));
	gamma_slider=GTK_SCALE(gtk_builder_get_object(builder,"gamma_slider"));
	threshold_slider=GTK_SCALE(gtk_builder_get_object(builder,"threshold_slider"));
	apply_button_threshold = GTK_WIDGET(gtk_builder_get_object(builder,"apply_button_threshold"));
	gamma_window = GTK_WIDGET(gtk_builder_get_object(builder,"gamma_window"));
	threshold_window = GTK_WIDGET(gtk_builder_get_object(builder,"threshold_window"));
	//r1_slider= GTK_SCALE(gtk_builder_get_object(builder,"r1_slider"));
	r1_slider= GTK_SCALE(gtk_builder_get_object(builder,"r1_slider"));
	r2_slider = GTK_SCALE(gtk_builder_get_object(builder,"r2_slider"));
	g1_slider= GTK_SCALE(gtk_builder_get_object(builder,"g1_slider"));
	g2_slider= GTK_SCALE(gtk_builder_get_object(builder,"g2_slider"));
	b1_slider= GTK_SCALE(gtk_builder_get_object(builder,"b1_slider"));
	b2_slider= GTK_SCALE(gtk_builder_get_object(builder,"b2_slider"));
	color_threshold_slider= GTK_SCALE(gtk_builder_get_object(builder,"color_threshold_slider"));

	// Create events
	gtk_widget_add_events(draw_area, GDK_POINTER_MOTION_MASK);
	gtk_widget_add_events(draw_area, GDK_BUTTON_PRESS_MASK);
	gtk_widget_add_events(draw_area, GDK_BUTTON_RELEASE_MASK);

	// Connecting signals
	gtk_builder_connect_signals(builder,NULL);
	g_signal_connect(window,"destroy",G_CALLBACK(gtk_main_quit),NULL);
	g_signal_connect (G_OBJECT (draw_area), "draw", G_CALLBACK (draw_callback), NULL);
	g_signal_connect (draw_area, "motion-notify-event", G_CALLBACK(on_mouse_move), NULL);
	g_signal_connect (draw_area, "button-press-event", G_CALLBACK(on_mouse_press), NULL);
	g_signal_connect (draw_area, "button-release-event", G_CALLBACK(on_mouse_release), NULL);
	g_signal_connect(brush, "clicked", G_CALLBACK(on_tool_clicked), (gpointer *) BRUSH);
	g_signal_connect(bucket, "clicked", G_CALLBACK(on_tool_clicked), (gpointer *)BUCKET);
	g_signal_connect(rectangle_button, "clicked", G_CALLBACK(on_tool_clicked), (gpointer *)RECTANGLE);
	g_signal_connect(triangle_button, "clicked", G_CALLBACK(on_tool_clicked), (gpointer *)TRIANGLE);
	g_signal_connect(circle_button, "clicked", G_CALLBACK(on_tool_clicked), (gpointer *) CIRCLE);
	g_signal_connect(color_button, "color-set", G_CALLBACK(on_Color_set), NULL);
    g_signal_connect(previous, "clicked", G_CALLBACK(on_previous), NULL);
    g_signal_connect(next, "clicked", G_CALLBACK(on_next), NULL);
    g_signal_connect (G_OBJECT (window), "key_press_event", G_CALLBACK (on_key_press), NULL);
    g_signal_connect(scale_glider, "value_changed", G_CALLBACK(update_scale_val), NULL);
	g_signal_connect(window, "destroy", G_CALLBACK(epipaintor_free), NULL);
    g_signal_connect(new_file, "activate", G_CALLBACK(on_new_file), NULL);
    g_signal_connect(open_file, "activate", G_CALLBACK(on_open_file), NULL);
    g_signal_connect(save_file, "activate", G_CALLBACK(on_save_file), NULL);
    g_signal_connect(quit, "activate", G_CALLBACK(epipaintor_free), NULL);
	g_signal_connect(gaussian_blur_slider, "value_changed", G_CALLBACK(gaussian_blur_value), NULL);
	g_signal_connect(apply_button, "clicked", G_CALLBACK(gaussian_blur_apply), NULL);
	g_signal_connect(apply_button_gamma, "clicked", G_CALLBACK(gamma_apply), NULL); 
	g_signal_connect(gamma_slider, "value_changed", G_CALLBACK(gamma_value), NULL);
	g_signal_connect(apply_button_threshold, "clicked", G_CALLBACK(threshold_apply), NULL);
	g_signal_connect(threshold_slider, "value_changed", G_CALLBACK(threshold_value), NULL);
	g_signal_connect(color_threshold_slider, "value_changed", G_CALLBACK(color_threshold_value), NULL);
	g_signal_connect(r1_slider, "value_changed", G_CALLBACK(r1_value), NULL);
	g_signal_connect(r2_slider, "value_changed", G_CALLBACK(r2_value), NULL);
	g_signal_connect(g1_slider, "value_changed", G_CALLBACK(g1_value), NULL);
	g_signal_connect(g2_slider, "value_changed", G_CALLBACK(g2_value), NULL);
	g_signal_connect(b1_slider, "value_changed", G_CALLBACK(b1_value), NULL);
	g_signal_connect(b2_slider, "value_changed", G_CALLBACK(b2_value), NULL); 
	g_signal_connect(apply_button_color, "clicked", G_CALLBACK(color_apply), NULL);





	// Window settings
	gtk_window_set_default_size(GTK_WINDOW(window),1920,1080);//keep it like this please.
	gtk_window_set_resizable(GTK_WINDOW(window),FALSE);
	gtk_window_set_icon_from_file(GTK_WINDOW(window),"../assets/logo_200x200.png",NULL);	// Stacks Initialization
    before = shared_stack_new();
    after = shared_stack_new();

	/*      Modification before this line */
	gtk_widget_show(window); // shows the window
	gtk_main();
	return EXIT_SUCCESS;
}

void epipaintor_free(gpointer user_data)
{
	// To avoid compilation warning
    data = user_data;

    shared_stack_destroy(before);
    shared_stack_destroy(after);

    SDL_FreeSurface(img_buff);
    //cairo_destroy (cr);

    gtk_main_quit();
}

gboolean draw_callback(GtkWidget* widget, cairo_t *cr, gpointer data)
{
	//Unused parameters :
	(void) widget;
	(void) data;

	if (!img_buff) return FALSE;

	//Actual function :
	if (pre_visualisation) {
		SDL_SaveBMP(pre_visualisation, "../cache/img_buff.bmp");
		SDL_FreeSurface(pre_visualisation);
		pre_visualisation = NULL;
	}
	else {
		SDL_SaveBMP(img_buff, "../cache/img_buff.bmp");
	}

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

gboolean on_mouse_press(GtkWidget* self, GdkEvent* event, gpointer user_data)
{
	// Cast unused variables to void to avoid warning
	(void) self;
	(void) event;
	(void) user_data;

	is_pressed = TRUE;
	on_press_x = curr_x;
	on_press_y = curr_y;

	//printf("(x, y) = (%i, %i)\n", on_press_x, on_press_y);

	if (!img_buff || selected_tool == NONE)
		return FALSE;

	shared_stack_push(before, img_buff);
	shared_stack_empty(after);


	switch (selected_tool) {
		case BRUSH:
			break;
		case BUCKET:
			img_buff = bucket_fill(img_buff, on_press_x, on_press_y, (Uint8) selected_color.r, \
					(Uint8) selected_color.g, (Uint8) selected_color.b,70);
			gtk_widget_queue_draw_area(draw_area, 0, 0, img_buff->w, img_buff->h);
			break;
	}

	return FALSE;
}

gboolean on_mouse_move(GtkWidget *widget,GdkEvent *event, gpointer user_data) 
{
	// Unused parameters :
	(void) widget;
	(void) user_data;
	
	if (!(event->type==GDK_MOTION_NOTIFY))
		return TRUE;

	GdkEventMotion* e = (GdkEventMotion*) event;
	last_x = curr_x;
	last_y = curr_y;
	curr_x = (guint) e->x;
	curr_y = (guint) e->y;


	if (!img_buff || !is_pressed) return FALSE;

	switch (selected_tool) {
		case BRUSH:
			drawline(img_buff, selected_color, last_x, last_y, curr_x, curr_y, brush_size);
			break;
		case RECTANGLE:
			pre_visualisation = copy_image(img_buff);
			pre_visualisation = rectangle(pre_visualisation, on_press_x, 
										  on_press_y, curr_x, curr_y, 
										  selected_color.r, selected_color.g, 
										  selected_color.b, brush_size);
			break;
		case TRIANGLE:
			pre_visualisation = copy_image(img_buff);
			pre_visualisation = triangle(pre_visualisation, on_press_x, 
										  on_press_y, curr_x, curr_y, 
										  selected_color.r, selected_color.g, 
										  selected_color.b, brush_size);
			break;
		case CIRCLE:
			pre_visualisation = copy_image(img_buff);
			pre_visualisation = circle(pre_visualisation, on_press_x, 
										  on_press_y, curr_x, curr_y, 
										  selected_color.r, selected_color.g, 
										  selected_color.b, brush_size);
			break;
	}


	gtk_widget_queue_draw_area(draw_area, 0, 0, img_buff->w, img_buff->h);
	return TRUE;
}

gboolean on_mouse_release(GtkWidget* self, GdkEvent* event, gpointer user_data)
{
	// Used to avoid compilations warning.
	widget = self;
	event = event;
	data = user_data;

	is_pressed = FALSE;
	// on_press_x = curr_x;
	// on_press_y = curr_y;

	if (pre_visualisation)
	{
		SDL_FreeSurface(pre_visualisation);
		pre_visualisation = NULL;
	}

	switch (selected_tool) {
		case RECTANGLE:
			img_buff = rectangle(img_buff, on_press_x, 
										  on_press_y, curr_x, curr_y, 
										  selected_color.r, selected_color.g, 
										  selected_color.b, brush_size);
			break;
		case TRIANGLE:
			img_buff = triangle(img_buff, on_press_x, 
										  on_press_y, curr_x, curr_y, 
										  selected_color.r, selected_color.g, 
										  selected_color.b, brush_size);
			break;
		case CIRCLE:
			img_buff = circle(img_buff, on_press_x, 
										  on_press_y, curr_x, curr_y, 
										  selected_color.r, selected_color.g, 
										  selected_color.b, brush_size);
			break;
	}

	gtk_widget_queue_draw_area(draw_area, 0, 0, img_buff->w, img_buff->h);
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

gboolean on_tool_clicked(GtkButton *self, gpointer user_data) {
	// Unused variables
	(void) self;

	selected_tool = (intptr_t) user_data;

	return FALSE;
}


void on_open_file(GtkMenuItem *menu_item, gpointer user_data)
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

// void on_save_file(GtkButton *b)
// {
// 	// Unused variables
// 	widget = (GtkWidget *) b;

// 	SDL_Surface * img_buff = SDL_LoadBMP("../cache/img_buff.bmp");
// 	SDL_SaveBMP(img_buff,"../save/saved_img.bmp");

// }

void on_save_file(GtkMenuItem *menu_item, gpointer user_data)
{
	// Used to avoid compilations warning
	widget = (GtkWidget *) menu_item;
	data = user_data;
	
  	GtkWidget *file_saver_dialog = gtk_file_chooser_dialog_new("Save File", NULL,
                                                              GTK_FILE_CHOOSER_ACTION_SAVE,
                                                              "gtk-cancel", GTK_RESPONSE_CANCEL,
                                                              "gtk-save", GTK_RESPONSE_OK,
                                                              NULL);
	gint response = gtk_dialog_run(GTK_DIALOG(file_saver_dialog));
	if (response == GTK_RESPONSE_OK)
	{
		char *filename = gtk_file_chooser_get_filename(GTK_FILE_CHOOSER(file_saver_dialog));
		g_print("Selected file: %s\n", filename);
		g_free(filename);
	}
	gtk_widget_destroy(file_saver_dialog);
}

void on_grayscale_activate(GtkMenuItem *self)
{
	if(img_buff==NULL)
		return;
	widget = (GtkWidget *) self;
	img_buff = grayscale(img_buff);
	gtk_widget_queue_draw_area(draw_area,0,0,img_buff->w,img_buff->h);
}


void on_otsu_threshold_activate(GtkMenuItem *self)
{
	shared_stack_push(before, img_buff);
	shared_stack_empty(after);
	if(img_buff==NULL)
		return;
	widget = (GtkWidget *) self;
	img_buff = otsu_threshold(img_buff);
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




gboolean gaussian_blur_apply(GtkScale *self, gpointer user_data)
{
	shared_stack_push(before, img_buff);
	shared_stack_empty(after);
	unsigned char value = gaussian_blur_value(gaussian_blur_slider);
	img_buff = gaussian(img_buff,value);
	widget = (GtkWidget *) self;
	user_data = user_data;
	gtk_widget_queue_draw_area(draw_area,0,0,img_buff->w,img_buff->h);
	return TRUE;
}
unsigned char gaussian_blur_value(GtkScale *self)
{
	widget = (GtkWidget *) self;
	return gtk_range_get_value(&(self->range));
}
void on_color_filter_activate(GtkMenuItem *self)
{	if(img_buff==NULL)
		return;
	widget = (GtkWidget *) self;
	GtkWidget *window = color_window;
	gtk_window_set_title(GTK_WINDOW(window), "Color Threshold");
    gtk_widget_show_all(window);

}


unsigned char  r1_value(GtkScale *self)
{
	widget = (GtkWidget *) self;
	return gtk_range_get_value(&(self->range));
}
unsigned char  color_threshold_value(GtkScale *self)
{
	widget = (GtkWidget *) self;
	return gtk_range_get_value(&(self->range));

}
unsigned char  r2_value(GtkScale *self)
{
	widget = (GtkWidget *) self;
	return gtk_range_get_value(&(self->range));
}
unsigned char  g1_value(GtkScale *self)
{
	widget = (GtkWidget *) self;
	return gtk_range_get_value(&(self->range));
}
unsigned char  g2_value(GtkScale *self)
{
	widget = (GtkWidget *) self;
	return gtk_range_get_value(&(self->range));
}
unsigned char  b1_value(GtkScale *self)
{
	widget = (GtkWidget *) self;
	return gtk_range_get_value(&(self->range));
}
unsigned char  b2_value(GtkScale *self)
{
	widget = (GtkWidget *) self;
	return gtk_range_get_value(&(self->range));
}
gboolean color_apply(GtkScale *self, gpointer user_data)
{
	printf("check");
	shared_stack_push(before, img_buff);
	shared_stack_empty(after);
	unsigned char value = color_threshold_value(color_threshold_slider);
	unsigned char value_r1 = r1_value(r1_slider);
	unsigned char value_g1 = g1_value(g1_slider);
	unsigned char value_b1 = b1_value(b1_slider);
	unsigned char value_r2 = r2_value(r2_slider);
	unsigned char value_g2 = g2_value(g2_slider);
	unsigned char value_b2 = b2_value(b2_slider);

	img_buff = color_threshold(img_buff,value,value_r1,value_g1,value_b1,value_r2,value_g2,value_b2);
	widget = (GtkWidget *) self;
	user_data = user_data;
	gtk_widget_queue_draw_area(draw_area,0,0,img_buff->w,img_buff->h);
	return TRUE;
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

void on_gaussian_blur_activate(GtkMenuItem *self)
{
    
	if(img_buff==NULL)
		return;
	widget = (GtkWidget *) self;
	
	GtkWidget *window = gaussian_blur_window;
	gtk_window_set_title(GTK_WINDOW(window), "Gaussian Blur");
    gtk_widget_show_all(window);
}

void on_gamma_activate(GtkMenuItem *self)
{
    
	if(img_buff==NULL)
		return;
	widget = (GtkWidget *) self;
	
	GtkWidget *window = gamma_window;
	gtk_window_set_title(GTK_WINDOW(window), "Gamma");
    gtk_widget_show_all(window);
   
}



unsigned char threshold_value(GtkScale *self)
{
	widget = (GtkWidget *) self;
	return (gtk_range_get_value(&(self->range)));
}

gboolean threshold_apply(GtkScale *self, gpointer user_data)
{
	shared_stack_push(before, img_buff);
	shared_stack_empty(after);
	widget = (GtkWidget *) self;
	unsigned char value = threshold_value(threshold_slider);
	img_buff = threshold(img_buff,value);
	user_data = user_data;

	gtk_widget_queue_draw_area(draw_area,0,0,img_buff->w,img_buff->h);
	return TRUE;

}


unsigned char gamma_value(GtkScale *self)
{
	widget = (GtkWidget *) self;
	return (gtk_range_get_value(&(self->range)));
}

gboolean gamma_apply(GtkScale *self, gpointer user_data)
{

	shared_stack_push(before, img_buff);
	shared_stack_empty(after);
	widget = (GtkWidget *) self;
	unsigned char value = gamma_value(gamma_slider);
	img_buff = gam(img_buff,value);
		user_data = user_data;

	gtk_widget_queue_draw_area(draw_area,0,0,img_buff->w,img_buff->h);
	return TRUE;
}

void on_new_file(GtkMenuItem *self)
{

	widget = (GtkWidget *) self;
	char *image_path = "../assets/blank.png";
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

                // past_selection(img, copy_crop_img, curr_x, pos_y);
                // gtk_widget_queue_draw_area(image,0,0,img->w,img->h);
            return FALSE;
    }

    return FALSE;
}
void on_threshold_activate(GtkMenuItem *self)
{
	if(img_buff==NULL)
		return;
	widget = (GtkWidget *) self;
	GtkWidget *window = threshold_window;
	gtk_window_set_title(GTK_WINDOW(window), "Threshold");
    gtk_widget_show_all(window);
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

