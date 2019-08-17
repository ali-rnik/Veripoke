#include <stdio.h>
#include <stdlib.h>

#include <iostream>
#include <map>
#include <string>
#include <mutex>
#include <thread>

#include "verilated.h"
#include "Valu.h"

#include <gtk/gtk.h>

Valu *tb;

GtkWidget *window;
GtkWidget *grid;
GtkWidget *inBut1, *inBut2, *inBut1E, *inBut2E;
GtkWidget *outLabel1, *outLabel1A;
GtkWidget *moduleImage;

void callbackBut(GtkWidget *button, GtkWidget *entry);
void setPortValue(std::string, int);

int main(int argc, char *argv[])
{
	gtk_init(&argc, &argv);

	window = gtk_window_new(GTK_WINDOW_TOPLEVEL);
	gtk_window_set_title(GTK_WINDOW(window), "Main");

	grid = gtk_grid_new();

	moduleImage = gtk_image_new_from_file("./module.png");

	inBut1 = gtk_button_new_with_label("inBut1");
	gtk_widget_set_name(inBut1, "inBut1");
	inBut1E = gtk_entry_new();

	inBut2 = gtk_button_new_with_label("inBut2");
	gtk_widget_set_name(inBut2, "inBut2");
	inBut2E = gtk_entry_new();

	outLabel1 = gtk_label_new("outLabel1");
	outLabel1A = gtk_label_new("outLabel1A");

	gtk_grid_attach(GTK_GRID(grid), inBut1, 0, 0, 1, 1);
	gtk_grid_attach_next_to(GTK_GRID(grid), inBut1E, inBut1, 
				GTK_POS_RIGHT, 1, 1);

	gtk_grid_attach_next_to(GTK_GRID(grid), inBut2, inBut1, 
				GTK_POS_BOTTOM, 1, 1);
	gtk_grid_attach_next_to(GTK_GRID(grid), inBut2E, inBut2, 
				GTK_POS_RIGHT, 1, 1);

	gtk_grid_attach(GTK_GRID(grid), outLabel1, 10, 0, 1, 1);
	gtk_grid_attach_next_to(GTK_GRID(grid), outLabel1A, outLabel1, 
		GTK_POS_RIGHT, 1, 1);

	gtk_grid_attach(GTK_GRID(grid), moduleImage, 2, 0, 3, 3);
	gtk_container_add(GTK_CONTAINER(window), grid);

	g_signal_connect(G_OBJECT(inBut1), "clicked", G_CALLBACK(callbackBut), 
			GTK_WIDGET(inBut1E));

	gtk_widget_show_all(window);
	gtk_main();

	return 0;
}

void callbackBut(GtkWidget *button, GtkWidget *entry)
{
	int entryValue = atoi(gtk_entry_get_text(GTK_ENTRY(entry)));

//setPortValue((std::string) gtk_widget_get_name(button), entryValue);
	setPortValue("control" , 1);
	std::cout << gtk_widget_get_name(button) << ":::" <<entryValue << std::endl;
}

void setPortValue(std::string portname, int value)
{
	if ( portname == "control")
		 tb->control = value;
	if ( portname == "in_A")
		 tb->in_A = value;
	if ( portname == "in_B")
		 tb->in_B = value;

	tb->eval();
}
