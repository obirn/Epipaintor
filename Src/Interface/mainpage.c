#include <gtk/gtk.h>
int main(int argc,char**argv)
{
	gtk_init(&argc,&argv);
	GtkWidget *window;
	GtkWidget *openfilebutton;
	openfilebutton = gtk_button_new_with_label("Open File");
	window = gtk_window_new(GTK_WINDOW_TOPLEVEL);
	//gtk_button_set_default_size(openfilebutton,200,100);
	gtk_container_add(GTK_CONTAINER(window),openfilebutton);
	gtk_window_set_title(GTK_WINDOW(window),"EPINATOR");
	gtk_window_set_default_size(GTK_WINDOW(window),1920,1080);//keep it like this please.
	gtk_window_set_resizable(GTK_WINDOW(window),TRUE);

	/*      Modification before this line */
	gtk_widget_show_all(window); // shows the window
	gtk_widget_show_all(openfilebutton); 
	g_signal_connect(window,"destroy",G_CALLBACK(gtk_main_quit),NULL);	
	gtk_main();
	return 0;
}
