// Image Editor
// Coman Andrei-Madalin
// 2023-2024

#ifndef CHECKS_HEADER_H
#define CHECKS_HEADER_H

#include "structure.h"

// Different checks needed for commands
// They are used in main_menu.c and options
// This way, the code is more readable and easier to maintain

int is_loaded(DATA_t *data);

int is_square(DATA_t *data);

int is_chaplin(DATA_t *data);

#endif
