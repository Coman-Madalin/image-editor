// Tema3 - Image Editor
// Coman Andrei-Madalin
// 315 CA
// 2023-2024

#ifndef STRUCTURE_HEADER_H
#define STRUCTURE_HEADER_H

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <math.h>

typedef union {
	unsigned short int **grayscale;
	unsigned short int ***color;
} ACTUAL_IMAGE_t;

typedef struct {
	ACTUAL_IMAGE_t *image;
	short int magic_word;
	unsigned short int scale;
	unsigned short int width;
	unsigned short int height;
	unsigned short int x1, x2, y1, y2;
} DATA_t;

#endif
