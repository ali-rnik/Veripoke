#include <iostream>
#include <fstream>
#include <map>

struct PortProp {
	std::string name;
	int value;
	std::string type;
};

std::map<std::string, PortProp> inPort, outPort;

int getPortsProp(std::string modName);
int codeGen(std::string modName);
int genVerilatorObj(std::string modName);
int genBin(std::string modName);

int main(int argc, char **argv)
{
	getPortsProp(argv[1]);
	codeGen(argv[1]);
	genVerilatorObj(argv[1]);
	genBin(argv[1]);
	return 0;
}

int genBin(std::string modName)
{
	std::string command, includeDirectory, src, output;

	modName.pop_back();
	modName.pop_back();

	includeDirectory = " -I/usr/share/verilator/include -I./obj_dir ";
	includeDirectory += "`pkg-config --libs --cflags gtk+-3.0` ";
	src = " /usr/share/verilator/include/verilated.cpp ";
	src += modName + ".cpp ";
	src += " obj_dir/V" + modName + "__ALL.a ";
	output = " -o " + modName;

	command = "g++ " + includeDirectory + src + output;

	if (system(command.data()) == -1)
		return -1;

	return 0;
}

int genVerilatorObj(std::string modName)
{
	std::string command;

	command = "verilator -Wall -cc " + modName;
	if (system(command.data()) == -1)
		return -1;

	modName.pop_back();
	modName.pop_back();

	command = "make -C obj_dir -f V" + modName + ".mk";
	if (system(command.data()) == -1)
		return -1;

	return 0;
}

int codeGen(std::string modName)
{
	std::ofstream outfile;
	std::map<std::string, PortProp>::iterator it;

	modName.pop_back();
	modName.pop_back();

	outfile.open(modName + ".cpp", std::ofstream::out | 
		std::ofstream::trunc);

	// headers and global variables
	outfile << "#include <gtk/gtk.h>" << std::endl;
	outfile << "#include <stdio.h>" << std::endl;
	outfile << "#include <stdlib.h>" << std::endl;
	outfile << std::endl;
	outfile << "#include <iostream>" << std::endl;
	outfile << "#include <map>" << std::endl;
	outfile << "#include <string>" << std::endl;
	outfile << std::endl;
	outfile << "#include \"verilated.h\"" << std::endl;
	outfile << "#include \"V" << modName << ".h\"" << std::endl;
	outfile << std::endl;

	outfile << "V" << modName << " *tb;" << std::endl;
	outfile << std::endl;
	outfile << "GtkWidget *window, *grid, *moduleImage;" << std::endl;

	for (it = inPort.begin(); it != inPort.end(); it++)
		outfile << "GtkWidget " << "*" << it->second.name << ", *" << 
			it->second.name << "E;" << std::endl;

	for (it = outPort.begin(); it != outPort.end(); it++)
		outfile << "GtkWidget " << "*" << it->second.name << ", *" << 
			it->second.name << "A;" << std::endl;
	outfile << std::endl;

	outfile << "void callbackBut(GtkWidget *button, GtkWidget *entry);" << 
		std::endl;
	outfile << "void setPortValue(std::string, int);" << std::endl;
	outfile << "void guiSetup();" << std::endl;
	outfile << std::endl;

	// main
	outfile << "int main(int argc, char **argv)" << std::endl;
	outfile << "{" << std::endl;
	outfile << "\tgtk_init(&argc, &argv);" << std::endl;
	outfile << "\tVerilated::commandArgs(argc, argv);" << std::endl;
	outfile << "\ttb = new V" << modName  << ";"<< std::endl;
	outfile << "\tguiSetup();"<< std::endl;
	outfile << "\treturn 0;"<< std::endl;
	outfile << "}" << std::endl;
	outfile << std::endl;

	// guiSetup
	outfile << "void guiSetup()" << std::endl;
	outfile << "{" << std::endl;
	outfile << "\twindow = gtk_window_new(GTK_WINDOW_TOPLEVEL);" << 
		std::endl;
	outfile << "\tgtk_window_set_title(GTK_WINDOW(window)," << 
		" \"Veripoke\");" << std::endl;
	outfile << std::endl;

	outfile << "\tgrid = gtk_grid_new();" << std::endl;
	outfile << "\tgtk_container_add(GTK_CONTAINER(window), grid);" << 
		std::endl;
	outfile << std::endl;

	outfile << "\tmoduleImage = gtk_image_new_from_file" << 
		"(\"./module.png\");" << std::endl;
	outfile << "\tgtk_grid_attach(GTK_GRID(grid), " << 
		"moduleImage, 2, 0, 3, 3);" << std::endl;
	outfile << std::endl;

	int rowCnt = 0;
	for (it = inPort.begin(); it != inPort.end(); it++) {
		outfile << "\t" << it->second.name << " = " << 
			"gtk_button_new_with_label(" << "\"" << 
			it->second.name << "\");" << std::endl;
		outfile << "\tgtk_widget_set_name(" << it->second.name << 
			", \"" << it->second.name << "\");" << std::endl;
		outfile << "\tgtk_grid_attach(GTK_GRID(grid), " << 
			it->second.name << ", 0, " << rowCnt << ", 1, 1);" 
			<< std::endl;
		outfile << "\t" << it->second.name << "E = gtk_entry_new();" 
			<< std::endl;
		outfile << "\tgtk_grid_attach(GTK_GRID(grid), " << 
			it->second.name << "E, 1, " << rowCnt << ", 1, 1);" 
			<< std::endl;
		outfile << std::endl;
		rowCnt++;
	}

	rowCnt = 0;
	for (it = outPort.begin(); it != outPort.end(); it++) {
		outfile << "\t" << it->second.name << " = gtk_label_new(\"" << 
			it->second.name << ": \");" << std::endl;
		outfile << "\tgtk_grid_attach(GTK_GRID(grid), " << 
			it->second.name << ", 9, " << rowCnt << ", 1, 1);" 
			<< std::endl;
		outfile << "\t" << it->second.name << "A = gtk_label_new(\"" 
			<< it->second.name << "A\");" << std::endl;
		outfile << "\tgtk_grid_attach(GTK_GRID(grid), " << 
			it->second.name << "A, 10, " << rowCnt << ", 1, 1);" 
			<< std::endl;
		outfile << std::endl;
		rowCnt++;
	}

	for (it = inPort.begin(); it != inPort.end(); it++) {
		outfile << "\tg_signal_connect(G_OBJECT(" << 
			it->second.name << 
			"), \"clicked\", G_CALLBACK(callbackBut), " << 
			"GTK_WIDGET(" << it->second.name << "E));" 
			<< std::endl;
	}
	outfile << std::endl;

	outfile << "\tgtk_widget_show_all(window);" << std::endl;
	outfile << "\tgtk_main();" << std::endl;

	outfile << "}" << std::endl;
	outfile << std::endl;

	// callbackBut
	outfile << "void callbackBut(GtkWidget *button, GtkWidget *entry)" << 
		std::endl;
	outfile << "{" << std::endl;
	outfile << "\tint entryValue = atoi(gtk_entry_get_text" << 
		"(GTK_ENTRY(entry)));" << std::endl;
	outfile << "\tsetPortValue((std::string) " << 
		"gtk_widget_get_name(button), entryValue);" << std::endl;
	outfile << "\tstd::cout << gtk_widget_get_name(button) << \" :: \"" <<
		" << entryValue << std::endl;" << std::endl;
	outfile << "}" << std::endl;
	outfile << std::endl;

	// setPortValue
	outfile << "void setPortValue(std::string portname, int value)" << std::endl;
	outfile << "{" << std::endl;
	for (it = inPort.begin(); it != inPort.end(); it++) {
		outfile << "\tif (portname == \"" << it->second.name << "\")" 
			<< std::endl;
		outfile << "\t\t tb->" << it->second.name << " = value;" << 
			std::endl;
	}
	outfile << std::endl;
	outfile << "\ttb->eval();" << std::endl;
	outfile << std::endl;
	for (it = outPort.begin(); it != outPort.end(); it++) {
		outfile << "\tgtk_label_set_text(GTK_LABEL(" << it->second.name 
			<< "A), std::to_string(tb->" << it->second.name << 
			").data());" << std::endl;
	}
	outfile << "}" << std::endl;
	outfile << std::endl;

	outfile.close();
	return 0;
}

int getPortsProp(std::string modName)
{
	std::string command, portname, portdir, porttype;
	std::ifstream infile;

	inPort.clear();
	outPort.clear();

	command = "ports_extraction.pl " + modName + " > ports.txt";

	if (system(command.data()) == -1)
		return -1;

	infile.open("./ports.txt", std::ifstream::in);
	while (!infile.eof()) {
		infile >> portname;
		infile >> portdir;
		infile >> porttype;

		if (porttype == "reg")
			infile >> porttype;

		if (portdir == "in") {
			inPort[portname].name = portname;
			inPort[portname].type = porttype;
		} else if (portdir == "out") {
			outPort[portname].name = portname;
			outPort[portname].type = porttype;
		}
	}
	infile.close();

	return 0;
}
