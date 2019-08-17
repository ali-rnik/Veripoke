#!/bin/bash

g++ $1 /usr/share/verilator/include/verilated.cpp obj_dir/Valu__ALL.a -o program -fpermissive `pkg-config --libs --cflags gtk+-3.0` -I/usr/share/verilator/include -I obj_dir
