#include <stdio.h>
#include <string.h>
#include "structure_header.h"
#include "options_header.h"
#include "read_utils.h"

void print_image(PLACEHOLDER *data)
{
	int i, j;
	if (is_loaded(data, 1) == 0)
		return;

	if (data->magic_word == 2) {
		printf("%d\n", data->magic_word);
		printf("%d %d\n", data->width, data->height);
		printf("%d\n", data->scale);

		for (i = 0; i < 10; i++) {
			for (j = 0; j < 60; j++)
				printf("%d ", data->image->grayscale[i][j]);
			printf("\n");
		}
		return;
	}

	if (data->magic_word == 3) {
		printf("%d\n", data->magic_word);
		printf("%d %d\n", data->width, data->height);
		printf("%d\n", data->scale);

		for (i = 0; i < 10; i++) {
			for (j = 0; j < 60; j++)
				printf("%d %d %d ", data->image->color[i][j][0],
					   data->image->color[i][j][1],
					   data->image->color[i][j][2]);
			printf("\n");
		}
		return;
	}
}

int any_off_limits(PLACEHOLDER *data)
{
	int i, j;
	if (data->magic_word == 2) {
		for (i = 0; i < data->height; i++)
			for (j = 0; j < data->width; j++)
				if (data->image->grayscale[i][j] < 0 ||
					data->image->grayscale[i][j] > data->scale) {
					printf("Invalid P2 pixel, out of bounds: %d %d, value:%d\n",
						   i, j, data->image->grayscale[i][j]);
					return 1;
				}
		return 0;
	}
	int k;
	for (i = 0; i < data->height; i++)
		for (j = 0; j < data->width; j++)
			for (k = 0; k < 3; k++)
				if (data->image->color[i][j][k] < 0 ||
					data->image->color[i][j][k] > data->scale) {
					printf("Invalid P3,pixel, out of bounds: %d %d %d, value:%d\n",
						   i, j, k, data->image->color[i][j][k]);
					return 1;
				}
	return 0;
}

int main(void)
{
	char command[9];
	PLACEHOLDER *data = calloc(1, sizeof(PLACEHOLDER));
	char to_brazil[100];
	data->magic_word = -1;
	while (1 != 0) {
		scanf("%s", command);
		if (strcmp(command, "LOAD") == 0)
			LOAD(&data);
		else if (strcmp(command, "SELECT") == 0)
			SELECT(&data);
		else if (strcmp(command, "HISTOGRAM") == 0)
			HISTOGRAM(data);
		else if (strcmp(command, "EQUALIZE") == 0)
			EQUALIZE(&data);
		else if (strcmp(command, "CROP") == 0)
			CROP(&data);
		else if (strcmp(command, "APPLY") == 0)
			APPLY(&data);
		else if (strcmp(command, "SAVE") == 0)
			SAVE(data);
		else if (strcmp(command, "PRINT") == 0)
			print_image(data);
		else if (strncmp(command, "ANY", 3) == 0) {
			if (any_off_limits(data) == 0)
				printf("OK\n");
		} else if (strcmp(command, "EXIT") == 0) {
			is_loaded(data, 1);
			break;
		} else if (strcmp(command, "ascii") == 0)
			continue;
		else {
			fgets(to_brazil, 100, stdin);
			printf("Invalid command\n");
		}

	}
	return 0;
}
