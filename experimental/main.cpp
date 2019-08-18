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

GtkWidget *moduleImage;
GtkWidget *control, *controlE, *in_A, *in_AE, *in_B, *in_BE;
GtkWidget *out, *outA, *equal, *equalA, *greater, *greaterA, *lesser, *lesserA;

void callbackBut(GtkWidget *button, GtkWidget *entry);
void setPortValue(std::string, int);

int main(int argc, char *argv[])
{
	gtk_init(&argc, &argv);
	Verilated::commandArgs(argc, argv);
	tb = new Valu;

	window = gtk_window_new(GTK_WINDOW_TOPLEVEL);
	gtk_window_set_title(GTK_WINDOW(window), "Main");

	grid = gtk_grid_new();
	gtk_container_add(GTK_CONTAINER(window), grid);

	moduleImage = gtk_image_new_from_file("./module.png");
	gtk_grid_attach(GTK_GRID(grid), moduleImage, 2, 0, 3, 3);

	in_A = gtk_button_new_with_label("in_A");
	gtk_widget_set_name(in_A, "in_A");
	gtk_grid_attach(GTK_GRID(grid), in_A, 0, 0, 1, 1);
	in_AE = gtk_entry_new();
	gtk_grid_attach(GTK_GRID(grid), in_AE, 1, 0, 1, 1);

	in_B = gtk_button_new_with_label("in_B");
	gtk_widget_set_name(in_B, "in_B");
	gtk_grid_attach(GTK_GRID(grid), in_B, 0, 1, 1, 1);
	in_BE = gtk_entry_new();
	gtk_grid_attach(GTK_GRID(grid), in_BE, 1, 1, 1, 1);

	control = gtk_button_new_with_label("control");
	gtk_widget_set_name(control, "control");
	gtk_grid_attach(GTK_GRID(grid), control, 0, 2, 1, 1);
	controlE = gtk_entry_new();
	gtk_grid_attach(GTK_GRID(grid), controlE, 1, 2, 1, 1);

	out = gtk_label_new("out: ");
	gtk_grid_attach(GTK_GRID(grid), out, 9, 0, 1, 1);
	outA = gtk_label_new("outA");
	gtk_grid_attach(GTK_GRID(grid), outA, 10, 0, 1, 1);

	equal = gtk_label_new("equal: ");
	gtk_grid_attach(GTK_GRID(grid), equal, 9, 1, 1, 1);
	equalA = gtk_label_new("equalA");
	gtk_grid_attach(GTK_GRID(grid), equalA, 10, 1, 1, 1);

	greater = gtk_label_new("greater: ");
	gtk_grid_attach(GTK_GRID(grid), greater, 9, 2, 1, 1);
	greaterA = gtk_label_new("greaterA");
	gtk_grid_attach(GTK_GRID(grid), greaterA, 10, 2, 1, 1);

	lesser = gtk_label_new("lesser: ");
	gtk_grid_attach(GTK_GRID(grid), lesser, 9, 3, 1, 1);
	lesserA = gtk_label_new("lesserA");
	gtk_grid_attach(GTK_GRID(grid), lesserA, 10, 3, 1, 1);

	g_signal_connect(G_OBJECT(in_A), "clicked", G_CALLBACK(callbackBut), 
		GTK_WIDGET(in_AE));
	g_signal_connect(G_OBJECT(in_B), "clicked", G_CALLBACK(callbackBut), 
		GTK_WIDGET(in_BE));
	g_signal_connect(G_OBJECT(control), "clicked", G_CALLBACK(callbackBut), 
		GTK_WIDGET(controlE));

	gtk_widget_show_all(window);
	gtk_main();

	return 0;
}

void callbackBut(GtkWidget *button, GtkWidget *entry)
{
	int entryValue = atoi(gtk_entry_get_text(GTK_ENTRY(entry)));

	setPortValue((std::string) gtk_widget_get_name(button), entryValue);
	std::cout << gtk_widget_get_name(button) << ":::" <<entryValue << 
		std::endl;
}

void setPortValue(std::string portname, int value)
{
	if (portname == "control")
		tb->control = value;
	if (portname == "in_A")
		tb->in_A = value;
	if (portname == "in_B")
		tb->in_B = value;

	tb->eval();

	gtk_label_set_text(GTK_LABEL(outA), std::to_string(tb->out).data());
	gtk_label_set_text(GTK_LABEL(equalA), std::to_string(tb->equal).data());
	gtk_label_set_text(GTK_LABEL(greaterA), std::to_string(tb->greater).data());
	gtk_label_set_text(GTK_LABEL(lesserA), std::to_string(tb->lesser).data());
}
