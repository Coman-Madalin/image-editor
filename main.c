#include "structure_header.h"
#include "main_menu.h"
#include "options_header.h"

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
			printf("\n\n");
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
			printf("\n\n");
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

void M(PLACEHOLDER *data)
{
	printf("MAGIC NUMBER: %d\n", data->magic_word);
}

int main(void)
{
	char command[50];
	PLACEHOLDER *data = calloc(1, sizeof(PLACEHOLDER));
	data->magic_word = -1;
	while (1 != 0) {
		fgets(command, 50, stdin);
		char *token = strtok(command, "\n ");
		if (!token)
			continue;
		if (strcmp(token, "LOAD") == 0)
			PRELOAD(&data, token);
		else if (strcmp(token, "SELECT") == 0)
			PRESELECT(&data, token);
		else if (strcmp(token, "HISTOGRAM") == 0)
			PREHISTOGRAM(data, token);
		else if (strcmp(token, "EQUALIZE") == 0)
			EQUALIZE(&data);
		else if (strcmp(token, "ROTATE") == 0)
			PREROTATE(&data, token);
		else if (strcmp(token, "CROP") == 0)
			CROP(&data);
		else if (strcmp(token, "APPLY") == 0)
			PREAPPLY(&data, token);
		else if (strcmp(token, "SAVE") == 0)
			PRESAVE(data, token);
		else if (strcmp(token, "PRINT") == 0)
			print_image(data);
		else if (strcmp(token, "M") == 0)
			M(data);
		else if (strncmp(token, "ANY", 3) == 0) {
			if (any_off_limits(data) == 0)
				printf("OK\n");
		} else if (strcmp(command, "EXIT") == 0) {
			PREEXIT(&data);
			return 0;
		} else {
			printf("Invalid command\n");
		}
	}
}
