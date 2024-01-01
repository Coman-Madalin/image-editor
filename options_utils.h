#ifndef OPTIONS_UTILS_H
#define OPTIONS_UTILS_H

#include "structure_header.h"

int is_loaded(PLACEHOLDER *data, int option);

void clamp(int *value, int min, int max);

int is_Chaplin(PLACEHOLDER *data);

void EDGE(int kernel[3][3]);

void SHARPEN(int kernel[3][3]);

void BLUR(int kernel[3][3]);

void GAUSSIAN_BLUR(int kernel[3][3]);

void APPLY_UTIL(int coef, int kernel[3][3], PLACEHOLDER **data);

#endif