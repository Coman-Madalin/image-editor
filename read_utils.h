#ifndef READ_UTILS_H
#define READ_UTILS_H

#include "structure.h"

void LOAD(PLACEHOLDER_t **data, char *file_name);

int read_common(PLACEHOLDER_t **data, FILE *fptr);

void read_p2(PLACEHOLDER_t **data, FILE *fptr, int first_element);

void read_p3(PLACEHOLDER_t **data, FILE *fptr, int first_element);

void read_p5(PLACEHOLDER_t **data, FILE *fptr, int first_element);

void read_p6(PLACEHOLDER_t **data, FILE *fptr, int first_element);

#endif
