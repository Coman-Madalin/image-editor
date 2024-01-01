# compiler setup
CC=gcc
CFLAGS=-Wall -Wextra -std=c99

# define targets
TARGETS = image_editor

build: $(TARGETS)

image_editor: main.c
	$(CC) $(CFLAGS) main.c main_menu.c options_header.c options_utils.c structure_header.h read_utils.c -lm -o image_editor

pack:
	zip -FSr 315CA_ComanAndreiMadalin_Tema3.zip README Makefile *.c *.h

clean:
	rm -f $(TARGETS)

.PHONY: pack clean
