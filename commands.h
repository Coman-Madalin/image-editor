#ifndef OPTIONS_HEADER_H
#define OPTIONS_HEADER_H

#include "structure.h"
#include "apply_utils.h"
#include "checks.h"

void SELECT(PLACEHOLDER_t **data, int x1, int y1, int x2, int y2);

void HISTOGRAM(PLACEHOLDER_t *data, int bins, int max_stars);

void EQUALIZE(PLACEHOLDER_t **data);

void ROTATE(PLACEHOLDER_t **data, int angle);

void ROTATE_ALL(PLACEHOLDER_t **data, int angle);

void CROP(PLACEHOLDER_t **data);

void APPLY(PLACEHOLDER_t **data, char *parameter);

void SAVE(PLACEHOLDER_t *data, char *filename, char *ascii);

void EXIT(PLACEHOLDER_t **data);

#endif
