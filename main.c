#include <stdio.h>
#include <string.h>
#include "structure_header.h"
#include "commands_utils.h"
#include "read_utils.h"


void print_image(PLACEHOLDER *image)
{
	int i, j;
	if (image->magic_word == 1) {
		IMAGE_BLACK_WHITE *img = (IMAGE_BLACK_WHITE *) image->actual_image;
		printf("%d\n", image->magic_word);
		printf("%d %d\n", img->width, img->height);

		for (i = 0; i < img->height; i++) {
			for (j = 0; j < img->width; j++)
				printf("%d ", img->array[i][j]);
			printf("\n");
		}
		return;
	}

	if (image->magic_word == 2) {
		IMAGE_GRAYSCALE *img = (IMAGE_GRAYSCALE *) image->actual_image;
		printf("%d\n", image->magic_word);
		printf("%d %d\n", img->width, img->height);
		printf("%d\n", img->scale);

		for (i = 0; i < img->height; i++) {
			for (j = 0; j < img->width; j++)
				printf("%d ", img->array[i][j]);
			printf("\n");
		}
		return;
	}

	if (image->magic_word == 3) {
		IMAGE_COLOR *img = (IMAGE_COLOR *) image->actual_image;
		printf("%d\n", image->magic_word);
		printf("%d %d\n", img->width, img->height);
		printf("%d\n", img->scale);

		for (i = 0; i < img->height; i++) {
			for (j = 0; j < img->width; j++)
				printf("%d %d %d ", img->array[i][j][0], img->array[i][j][1],
					   img->array[i][j][2]);
			printf("\n");
		}
		return;
	}
}

int main()
{
	char *command = calloc(5, sizeof(char));
	PLACEHOLDER *image = calloc(1, sizeof(PLACEHOLDER));
	scanf("%s", command);
	while (1 != 0) {
		if (strcmp(command, "LOAD") == 0)
			LOAD(&image);
		else if (strcmp(command, "SELECT") == 0)
			SELECT(&image);
		else if (strcmp(command, "EXIT") == 0)
			break;
		else
			printf("Invalid command\n");
	}

	print_image(image);
	return 0;
}
