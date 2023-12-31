#ifndef READ_UTILS_H
#define READ_UTILS_H

#include "structure_header.h"

void LOAD(PLACEHOLDER **data);

void read_common(PLACEHOLDER **data, FILE *fptr);

void read_p2(PLACEHOLDER **data, FILE *fptr);

void read_p3(PLACEHOLDER **data, FILE *fptr);

void read_p5(PLACEHOLDER **data, FILE *fptr);

void read_p6(PLACEHOLDER **data, FILE *fptr);

#endif
