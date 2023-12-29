#ifndef STRUCTURE_HEADER_H
#define STRUCTURE_HEADER_H

#include <stdio.h>
#include <string.h>
#include <stdlib.h>

typedef struct {
	int scale;
	int **array;
} IMAGE_GRAYSCALE;

typedef struct {
	int **array;
} IMAGE_BLACK_WHITE;

typedef struct {
	int scale;
	int ***array;
} IMAGE_COLOR;

typedef union {
	IMAGE_BLACK_WHITE black_white;
	IMAGE_GRAYSCALE grayscale;
	IMAGE_COLOR color;
} ACTUAL_IMAGE;

typedef struct {
	ACTUAL_IMAGE *image;
	int magic_word;
	int width;
	int height;
	int x1, x2, y1, y2;
} PLACEHOLDER;

#endif
