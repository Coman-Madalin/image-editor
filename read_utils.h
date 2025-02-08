// Image Editor
// Coman Andrei-Madalin
// 2023-2024

#ifndef READ_UTILS_H
#define READ_UTILS_H

#include "structure.h"

void LOAD(DATA_t **data, char *file_name);

int read_common(DATA_t **data, FILE *fptr);

void read_p2(DATA_t **data, FILE *fptr, int first_element);

void read_p3(DATA_t **data, FILE *fptr, int first_element);

void read_p5(DATA_t **data, FILE *fptr, int first_element);

void read_p6(DATA_t **data, FILE *fptr, int first_element);

#endif
