#ifndef MAIN_MENU_H
#define MAIN_MENU_H

#include "structure_header.h"
#include "options_header.h"
#include "read_utils.h"

void PRELOAD(PLACEHOLDER **data, char *token);

void PRESELECT(PLACEHOLDER **data, char *token);

void PREROTATE(PLACEHOLDER **data, char *token);

void PREHISTOGRAM(PLACEHOLDER *data, char *token);

void PREAPPLY(PLACEHOLDER **data, char *token);

void PRESAVE(PLACEHOLDER *data, char *token);

void PREEXIT(PLACEHOLDER **data);

#endif
