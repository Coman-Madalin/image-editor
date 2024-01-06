#ifndef MAIN_MENU_H
#define MAIN_MENU_H

#include "structure.h"
#include "commands.h"
#include "read_utils.h"
#include "checks.h"

// Used to process the input from the user, get the parameters and validate them

void PRELOAD(PLACEHOLDER_t **data, char *parameters);

void PRESELECT(PLACEHOLDER_t **data, char *parameters);

void PREHISTOGRAM(PLACEHOLDER_t *data, char *parameters);

void PREROTATE(PLACEHOLDER_t **data, char *parameters);

void PREAPPLY(PLACEHOLDER_t **data, char *parameters);

void PRESAVE(PLACEHOLDER_t *data, char *parameters);

void PREEXIT(PLACEHOLDER_t **data);

#endif
