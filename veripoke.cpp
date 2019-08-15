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

	includeDirectory= " -I/usr/share/verilator/include -I./obj_dir ";
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

	outfile.open(modName + ".cpp", std::ofstream::out | std::ofstream::trunc);

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

	outfile << "void setPortValue(std::string, int);" << std::endl;

	outfile << std::endl;

	outfile << "int main(int argc, char **argv)" << std::endl;
	outfile << "{" << std::endl;
	outfile << "\tstd::string portname;" << std::endl;
	outfile << "\tint value;" << std::endl;
	outfile << "\tVerilated::commandArgs(argc, argv);" << std::endl;
	outfile << "\ttb = new V" << modName  << ";"<< std::endl;

	outfile << std::endl;

	outfile << "\twhile (1) {" << std::endl;
	outfile << "\t\tstd::cin >> portname >> value;" << std::endl;
	outfile << "\t\tsetPortValue(portname, value);" << std::endl;
	outfile << "\t}" << std::endl;
	outfile << "}" << std::endl;

	outfile << std::endl;

	outfile << "void setPortValue(std::string portname, int value)" << std::endl;
	outfile << "{" << std::endl;

	for (it = inPort.begin(); it != inPort.end(); it++) {
		outfile << "\tif ( portname == \"" << it->second.name << "\")" << std::endl;
		outfile << "\t\t tb->" << it->second.name << " = value;" << std::endl;
	}

	outfile << std::endl;

	outfile << "\ttb->eval();" << std::endl;


	outfile << "}" << std::endl;

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
