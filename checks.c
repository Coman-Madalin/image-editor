#include "checks.h"

int is_loaded(PLACEHOLDER_t *data)
{
	if (data->magic_word == -1) {
		printf("No image loaded\n");
		return 0;
	}
	return 1;
}

int is_square(PLACEHOLDER_t *data)
{
	if (data->x1 == 0 && data->x2 == data->width)
		if (data->y1 == 0 && data->y2 == data->height)
			return 2;
	if (data->x2 - data->x1 == data->y2 - data->y1)
		return 1;
	printf("The selection must be a square\n");
	return 0;
}

int is_chaplin(PLACEHOLDER_t *data)
{
	if (data->magic_word == 2) {
		printf("Easy, Charlie Chaplin\n");
		return 1;
	}
	return 0;
}
