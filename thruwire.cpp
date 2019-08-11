////////////////////////////////////////////////////////////////////////////////
//
// Filename: 	thruwire.cpp
//
// Project:	Verilog Tutorial Example file
//
// Purpose:	Main Verilator simulation script for the thruwire design
//
//	In this script, we toggle the input, and (hopefully) watch the output
//	toggle.
//
// Creator:	Dan Gisselquist, Ph.D.
//		Gisselquist Technology, LLC
//
////////////////////////////////////////////////////////////////////////////////
//
// Written and distributed by Gisselquist Technology, LLC
//
// This program is hereby granted to the public domain.
//
// This program is distributed in the hope that it will be useful, but WITHOUT
// ANY WARRANTY; without even the implied warranty of MERCHANTIBILITY or
// FITNESS FOR A PARTICULAR PURPOSE.
//
////////////////////////////////////////////////////////////////////////////////
//
//
#include <stdio.h>
#include <stdlib.h>
#include "Vthruwire.h"
#include "verilated.h"
#include <iostream>
#include <map>
#include <string>
#include <mutex>
#include <thread>

std::mutex mtx;
std::thread t1, t2;
Vthruwire *tb;

void evaluate_on_change();
void set_ports_value();

int main(int argc, char **argv) {
	Verilated::commandArgs(argc, argv);

	tb = new Vthruwire;
	t1 = std::thread(evaluate_on_change);
	t2 = std::thread(set_ports_value);
	t1.join();
	t2.join();
}

void evaluate_on_change()
{
	while (1) {
		mtx.lock();
		tb->eval();
		printf("sw = %d, ", tb->i_sw);
		printf("led = %d\n", tb->o_led);
	}
}

void set_ports_value()
{
	int value;
	std::string port_name;

	while (1) {
		std::cin >> port_name >> value;

		if (port_name ==  "i_sw")
			tb->i_sw = value;
		mtx.unlock();
	}
}

