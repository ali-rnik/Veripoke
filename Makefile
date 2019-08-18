FLAGS=-g -Wall
CC=g++
INSTALL_PATH="/usr/local/bin"
DATA_PATH="/usr/local/share/data/"

all: veripoke

veripoke: veripoke.cpp ports_extraction.pl
	$(CC) $(FLAGS) $< -o $@
install: veripoke ports_extraction.pl module.png
	sudo mkdir -p $(INSTALL_PATH)
	sudo mkdir -p $(DATA_PATH)
	sudo install veripoke ports_extraction.pl $(INSTALL_PATH)
	sudo install module.png $(DATA_PATH)
	
unistall:
	-sudo rm $(INSTALL_PATH)/veripoke $(INSTALL_PATH)/ports_extraction.pl
	-sudo rm $(DATA_PATH)/module.png
clean:
	-rm veripoke

