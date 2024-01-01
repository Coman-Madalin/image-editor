#include "structure_header.h"
#include "main_menu.h"
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
	char command[50];
	PLACEHOLDER *data = calloc(1, sizeof(PLACEHOLDER));
	char to_brazil[100];
	data->magic_word = -1;
	while (1 != 0) {
		fgets(command, 50, stdin);
		char *token = strtok(command, " \n");
		if (strcmp(token, "LOAD") == 0) {
			token = strtok(NULL, " \n");
			LOAD(&data, token);
		} else if (strcmp(token, "SELECT") == 0) {
			int i;
			int x1, y1, x2, y2;
			int is_all = 0;
			if(is_loaded(data, 1) == 0)
				continue;
			for (i = 0; i < 4; i++) {
				token = strtok(NULL, " \n");
				if (i == 0) {
					if (strncmp(token, "ALL", 3) == 0) {
						data->x1 = 0;
						data->y1 = 0;
						data->x2 = data->width;
						data->y2 = data->height;
						printf("Selected ALL\n");
						is_all = 1;
						break;
					} else
						x1 = strtol(token, NULL, 10);
				} else if (i == 1)
					y1 = strtol(token, NULL, 10);
				else if (i == 2)
					x2 = strtol(token, NULL, 10);
				else
					y2 = strtol(token, NULL, 10);
			}
			if (is_all == 0)
				SELECT(&data, x1, y1, x2, y2);

		} else if (strcmp(token, "HISTOGRAM") == 0) {
			int i;
			int bins, stars;
			for (i = 0; i < 2; i++) {
				token = strtok(NULL, " \n");
				if (i == 0)
					bins = strtol(token, NULL, 10);
				else
					stars = strtol(token, NULL, 10);
			}
			HISTOGRAM(data, bins, stars);
		} else if (strcmp(token, "EQUALIZE") == 0)
			EQUALIZE(&data);
		else if (strcmp(token, "CROP") == 0)
			CROP(&data);
		else if (strcmp(token, "APPLY") == 0) {
			token = strtok(NULL, " \n");
			APPLY(&data, token);
		} else if (strcmp(token, "SAVE") == 0) {
			token = strtok(NULL, " \n");
			char *file_name = token;
			token = strtok(NULL, " \n");
			SAVE(data, file_name, token);
		} else if (strcmp(token, "PRINT") == 0)
			print_image(data);
		else if (strncmp(token, "ANY", 3) == 0) {
			if (any_off_limits(data) == 0)
				printf("OK\n");
		} else if (strcmp(command, "EXIT") == 0) {
			is_loaded(data, 1);
			break;
		} else if (strcmp(command, "ascii") == 0)
			continue;
		else {
			printf("Invalid command\n");
		}

	}
	return 0;
}
