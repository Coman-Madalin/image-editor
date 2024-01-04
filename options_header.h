#ifndef OPTIONS_HEADER_H
#define OPTIONS_HEADER_H

#include "structure_header.h"
#include "options_utils.h"

void SELECT(PLACEHOLDER **data, int x1, int y1, int x2, int y2);

void HISTOGRAM(PLACEHOLDER *data, int bins, int max_stars);

void EQUALIZE(PLACEHOLDER **data);

void ROTATE(PLACEHOLDER **data, int angle);

void ROTATE_ALL(PLACEHOLDER **data, int angle);

void CROP(PLACEHOLDER **data);

void APPLY(PLACEHOLDER **data, char *parameter);

void SAVE(PLACEHOLDER *data, char *filename, char *ascii);

void EXIT(PLACEHOLDER **data);

#endif
