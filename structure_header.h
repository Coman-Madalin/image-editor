#ifndef STRUCTURE_HEADER_H
#define STRUCTURE_HEADER_H

#include <stdio.h>
#include <string.h>
#include <stdlib.h>

typedef struct {
	void *actual_image;
	int magic_word;
	int x1, x2, y1, y2;
} PLACEHOLDER;

typedef struct {
	int width;
	int height;
	int scale;
	int **array;
} IMAGE_GRAYSCALE;

typedef struct {
	int width;
	int height;
	int **array;
} IMAGE_BLACK_WHITE;

typedef struct {
	int width;
	int height;
	int scale;
	int ***array;
} IMAGE_COLOR;

#endif
