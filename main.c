#include <stdio.h>
#include <string.h>
#include "structure_header.h"
#include "options_header.h"
#include "read_utils.h"


void print_image(PLACEHOLDER *data)
{
	int i, j;
	if (data->magic_word == -1) {
		printf("No data loaded\n");
		return;
	}

	if (data->magic_word == 2) {
		printf("%d\n", data->magic_word);
		printf("%d %d\n", data->width, data->height);
		printf("%d\n", data->scale);

		for (i = 0; i < data->height; i++) {
			for (j = 0; j < data->width; j++)
				printf("%d ", data->image->grayscale[i][j]);
			printf("\n");
		}
		return;
	}

	if (data->magic_word == 3) {
		printf("%d\n", data->magic_word);
		printf("%d %d\n", data->width, data->height);
		printf("%d\n", data->scale);

		for (i = 0; i < data->height; i++) {
			for (j = 0; j < data->width; j++)
				printf("%d %d %d ", data->image->color[i][j][0],
					   data->image->color[i][j][1],
					   data->image->color[i][j][2]);
			printf("\n");
		}
		return;
	}
}

int main()
{
	char *command = calloc(5, sizeof(char));
	PLACEHOLDER *data = calloc(1, sizeof(PLACEHOLDER));
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
			EQUALIZE(data);
		else if (strcmp(command, "CROP") == 0)
			CROP(&data);
		else if (strcmp(command, "PRINT") == 0)
			print_image(data);
		else if (strcmp(command, "EXIT") == 0)
			break;
		else
			printf("Invalid command\n");
	}
	return 0;
}
