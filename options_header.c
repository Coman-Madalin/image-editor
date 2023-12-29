#include "options_header.h"

void SELECT(PLACEHOLDER **data)
{
	if ((*data)->magic_word == -1) {
		printf("No data loaded\n");
		return;
	}

	char all[100];
	gets(all);
	if (strncmp(all, " ALL", 4) == 0) {
		(*data)->x1 = 0;
		(*data)->y1 = 0;
		(*data)->x2 = (*data)->width;
		(*data)->y2 = (*data)->height;
		printf("Selected ALL\n");
		return;
	}

	char *token = strtok(all, " ");
	int count = 1;
	int x1, y1, x2, y2;
	int invalid = 0;
	while (token != NULL) {
		if (count == 1) {
			x1 = strtol(token, NULL, 10);
			if (x1 < 0 || x1 > (*data)->width) {
				printf("Invalid set of coordinates\n");
				invalid = 1;
				break;
			}
		}

		if (count == 2) {
			y1 = strtol(token, NULL, 10);
			if (y1 < 0 || y1 > (*data)->height) {
				printf("Invalid set of coordinates\n");
				invalid = 1;
				break;
			}
		}
		if (count == 3) {
			x2 = strtol(token, NULL, 10);
			if (x2 < 0 || x2 > (*data)->width) {
				printf("Invalid set of coordinates\n");
				invalid = 1;
				break;
			}
		}
		if (count == 4) {
			y2 = strtol(token, NULL, 10);
			if (y2 < 0 || y2 > (*data)->height) {
				printf("Invalid set of coordinates\n");
				invalid = 1;
				break;
			}
		}
		count++;
		token = strtok(NULL, " ");
	}
	if (invalid == 1)
		return;

	if (x1 > x2) {
		int aux = x1;
		x1 = x2;
		x2 = aux;
	}
	if (y1 > y2) {
		int aux = y1;
		y1 = y2;
		y2 = aux;
	}

	(*data)->x1 = x1;
	(*data)->y1 = y1;
	(*data)->x2 = x2;
	(*data)->y2 = y2;

	printf("Selected %d %d %d %d\n", x1, y1, x2, y2);
}
