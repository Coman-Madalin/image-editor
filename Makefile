# compiler setup
CC=gcc
CFLAGS=-Wall -Wextra -std=c99

# define targets
TARGETS = image_editor

build: $(TARGETS)

image_editor: image_editor.c
	$(CC) $(CFLAGS) image_editor.c structure.h main_menu.c commands.c checks.c read_utils.c apply_utils.c -lm -o image_editor

clean:
	rm -f $(TARGETS)

.PHONY: pack clean
