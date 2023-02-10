#include <gtk/gtk.h>

GtkBuilder *builder;
GtkWidget *window;
GtkWidget *openfilebutton;
GtkWidget *menubar;
GtkWidget *menubar_new;
int main(int argc,char**argv)
{
	gtk_init(&argc,&argv);
	builder = gtk_builder_new_from_file("mainpage.glade");
	window = GTK_WIDGET(gtk_builder_get_object(builder,"mainpage"));
	openfilebutton = GTK_WIDGET(gtk_builder_get_object(builder,"open_file"));
	gtk_window_set_title(GTK_WINDOW(window),"EPIPAINTOR");
	gtk_window_set_default_size(GTK_WINDOW(window),1920,1080);//keep it like this please.
	gtk_window_set_resizable(GTK_WINDOW(window),TRUE);

	/*      Modification before this line */
	gtk_widget_show(window); // shows the window
	g_signal_connect(window,"destroy",G_CALLBACK(gtk_main_quit),NULL);	
	gtk_main();
	return 0;
}
void on_menubar_new_activate(GtkMenuItem *m)
{

}
void on_open_file_set(char* path)
{



}
