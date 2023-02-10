#include <gtk/gtk.h>

GtkBuilder *builder;
GtkWidget *window;
GtkWidget *fixed1;
GtkWidget *openfilebutton;
GtkWidget *image;
GtkWidget *label;
GtkWidget *button;
GtkWidget *fixed2;
GSList * image_path;
int main(int argc,char**argv)
{
	gtk_init(&argc,&argv);
	builder = gtk_builder_new_from_file("mainpage.glade");
	window = GTK_WIDGET(gtk_builder_get_object(builder,"mainpage"));
	g_signal_connect(window,"destroy",G_CALLBACK(gtk_main_quit),NULL);	
	gtk_builder_connect_signals(builder,NULL);
	fixed1 = GTK_WIDGET(gtk_builder_get_object(builder,"fixed1"));
	fixed2 = GTK_WIDGET(gtk_builder_get_object(builder,"fixed2"));
	openfilebutton = GTK_WIDGET(gtk_builder_get_object(builder,"open_file"));
	button = GTK_WIDGET(gtk_builder_get_object(builder,"button"));
	image = GTK_WIDGET(gtk_builder_get_object(builder,"image_window"));
	label = GTK_WIDGET(gtk_builder_get_object(builder,"label"));
	gtk_window_set_default_size(GTK_WINDOW(window),1920,1080);//keep it like this please.
	gtk_window_set_resizable(GTK_WINDOW(window),TRUE);

	/*      Modification before this line */
	gtk_widget_show(window); // shows the window
	gtk_main();
	return EXIT_SUCCESS;
}

char* on_open_file_file_activated(GtkFileChooserButton * b)
{
	image_path = gtk_file_chooser_get_filenames(GTK_FILE_CHOOSER(b));
	char* path = (char*)image_path->data;
	return path;
}


