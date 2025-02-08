// Image Editor
// Coman Andrei-Madalin
// 2023-2024

#ifndef MAIN_MENU_H
#define MAIN_MENU_H

#include "structure.h"
#include "commands.h"
#include "read_utils.h"
#include "checks.h"

// Used to process the input from the user, get the parameters and validate them

void PRELOAD(DATA_t **data, char *parameters);

void PRESELECT(DATA_t **data, char *parameters);

void PREHISTOGRAM(DATA_t *data, char *parameters);

void PREROTATE(DATA_t **data, char *parameters);

void PREAPPLY(DATA_t **data, char *parameters);

void PRESAVE(DATA_t *data, char *parameters);

void PREEXIT(DATA_t **data);

#endif
