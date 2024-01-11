// Tema3 - Image Editor
// Coman Andrei-Madalin
// 315 CA
// 2023-2024

#ifndef OPTIONS_UTILS_H
#define OPTIONS_UTILS_H

#include "structure.h"

// Because the APPLY command is used for multiple filters, I decided to make a
// header that contains every desired filter + the APPLY procedure
// This way, the code is more readable and easier to maintain

void EDGE(int kernel[3][3], int *coefficient);

void SHARPEN(int kernel[3][3], int *coefficient);

void BLUR(int kernel[3][3], int *coefficient);

void GAUSSIAN_BLUR(int kernel[3][3], int *coefficient);

void APPLY_UTIL(int coefficient, int kernel[3][3], DATA_t **data);

#endif

