#ifndef CHECKS_HEADER_H
#define CHECKS_HEADER_H

#include "structure.h"

// Different checks needed for commands
// They are used in main_menu.c and options
// This way, the code is more readable and easier to maintain

int is_loaded(PLACEHOLDER_t *data);

int is_square(PLACEHOLDER_t *data);

int is_chaplin(PLACEHOLDER_t *data);

#endif
