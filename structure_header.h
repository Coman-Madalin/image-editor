#ifndef STRUCTURE_HEADER_H
#define STRUCTURE_HEADER_H

#include <stdio.h>
#include <string.h>
#include <stdlib.h>

typedef union {
	int **grayscale;
	int ***color;
} ACTUAL_IMAGE;

typedef struct {
	ACTUAL_IMAGE *image;
	int magic_word;
	int scale;
	int width;
	int height;
	int x1, x2, y1, y2;
} PLACEHOLDER;

#endif
