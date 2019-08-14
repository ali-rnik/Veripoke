#include <iostream>
#include <fstream>
#include <map>

#define LINE_SIZE 1000

struct PortProp {
	std::string name;
	int value;
	std::string type;
};

std::map<std::string, PortProp> inPort, outPort;


int getPortsProp(std::string modName);
int codeGen(std::string modName);

int main(int argc, char **argv)
{
	getPortsProp(argv[0]);
	codeGen(argv[0]);

	return 0;
}

int codeGen(std::string modName)
{
	std::ofstream outfile;
	std::map<std::string, PortProp>::iterator it;

	modName += ".cpp";

	outfile.open(modName, std::ofstream::out | std::ofstream::trunc);

	outfile << "#include <stdio.h>" << std::endl;
	outfile << "#include <stdlib.h>" << std::endl;
	outfile << "#include <iostream>" << std::endl;
	outfile << "#include <map>" << std::endl;
	outfile << "#include <string>" << std::endl;
	outfile << "#include <mutex>" << std::endl;
	outfile << "#include <thread>" << std::endl;
	outfile << "#include \"verilated.h\"" << std::endl;
	outfile << "#include \"V" << modName << ".h\"" << std::endl;

	outfile << std::endl;

	outfile << "V" << modName << " *tb;" << std::endl;

	outfile << std::endl;

	outfile << "void set_port_value();" << std::endl;

	outfile << std::endl;

	outfile << "int main(int argc, char **argv)" << std::endl;
	outfile << "{" << std::endl;
	outfile << "\tstd::string portname;" << std::endl;
	outfile << "\tint value;" << std::endl;
	outfile << "\tVerilated::commandArgs(argc, argv);" << std::endl;
	outfile << "\ttb = new Vthruwire;" << std::endl;

	outfile << std::endl;

	outfile << "\twhile (1) {" << std::endl;
	outfile << "\t\tstd::cin >> portname >> value;" << std::endl;
	outfile << "\t\tset_port_value(portname, value);" << std::endl;
	outfile << "\t}" << std::endl;
	outfile << "}" << std::endl;

	outfile << std::endl;

	outfile << "void set_port_value(std::string portname, int value)" << std::endl;
	outfile << "{" << std::endl;

	for (it = outPort.begin(); it != outPort.end(); it++) {
		outfile << "\tif ( portname == \"" << it->second.name << "\")" << std::endl;
		outfile << "\t\t tb->" << it->second.name << " = value;" << std::endl;
	}

	outfile << std::endl;

	outfile << "\ttb->eval();" << std::endl;


	outfile << "}" << std::endl;

	return 0;
}

int getPortsProp(std::string modName)
{
	std::string command, portname, portdir, porttype;
	std::ifstream infile;

	inPort.clear();
	outPort.clear();

	command = "./ports_extraction.pl " + modName + " > ports.txt";

	if (system(command.data()) == -1)
		return -1;

	infile.open("./ports.txt", std::ifstream::in);
	while (!infile.eof()) {
		infile >> portname;
		infile >> portdir;
		infile >> porttype;

		if (portdir == "in") {
			inPort[portname].name = portname;
			inPort[portname].type = porttype;
		} else if (portdir == "out") {
			outPort[portname].name = portname;
			outPort[portname].type = porttype;
		}
	}

	return 0;
}
