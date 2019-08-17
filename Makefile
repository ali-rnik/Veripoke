FLAGS=-g -Wall
CC=g++
INSTALL_PATH="/usr/local/bin"

all: veripoke

veripoke: veripoke.cpp ports_extraction.pl
	$(CC) $(FLAGS) $< -o $@
install: veripoke ports_extraction.pl
	sudo install $^ $(INSTALL_PATH)
unistall:
	sudo rm $(INSTALL_PATH)/veripoke $(INSTALL_PATH)/ports_extraction.pl
clean:
	sudo rm veripoke

