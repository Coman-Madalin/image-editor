// Image Editor
// Coman Andrei-Madalin
// 2023-2024

#ifndef OPTIONS_HEADER_H
#define OPTIONS_HEADER_H

#include "structure.h"
#include "apply_utils.h"
#include "checks.h"

void SELECT(DATA_t **data, int x1, int y1, int x2, int y2);

void HISTOGRAM(DATA_t *data, int bins, int max_stars);

void EQUALIZE(DATA_t **data);

void ROTATE(DATA_t **data, int angle);

void ROTATE_ALL(DATA_t **data, int angle);

void CROP(DATA_t **data);

void APPLY(DATA_t **data, char *parameter);

void SAVE(DATA_t *data, char *filename, char *ascii);

void EXIT(DATA_t **data);

#endif
