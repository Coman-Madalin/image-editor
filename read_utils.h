#ifndef READ_UTILS_H
#define READ_UTILS_H

#include "structure_header.h"
#include "options_header.h"

void LOAD(PLACEHOLDER **data, char *file_name);

int read_common(PLACEHOLDER **data, FILE *fptr);

void read_p2(PLACEHOLDER **data, FILE *fptr, int first_element);

void read_p3(PLACEHOLDER **data, FILE *fptr, int first_element);

void read_p5(PLACEHOLDER **data, FILE *fptr, int first_element);

void read_p6(PLACEHOLDER **data, FILE *fptr, int first_element);

#endif
