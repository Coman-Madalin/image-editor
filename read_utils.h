#ifndef READ_UTILS_H
#define READ_UTILS_H

#include "structure_header.h"



void LOAD(PLACEHOLDER **image);

void read_p1(IMAGE_BLACK_WHITE **image, FILE *fptr);

void read_p2(IMAGE_GRAYSCALE **image, FILE *fptr);

void read_p3(IMAGE_COLOR **image, FILE *fptr);

//void read_p4(void **image, FILE *fptr);

//void read_p5(void **image, FILE *fptr);

//void read_p6(void **image, FILE *fptr);

#endif
