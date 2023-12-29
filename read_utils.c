#include "read_utils.h"

void LOAD(PLACEHOLDER **image)
{
	char *file_name = calloc(100, sizeof(char));
	char magic_word[3];
	(*image)->magic_word = -1;

	scanf("%s", file_name);
	FILE *fptr = fopen(file_name, "r");
	if (fptr == NULL) {
		printf("Failed to load %s/n", file_name);
		return;
	}

	magic_word[0] = '#';
	while (magic_word[0] == '#') {
		fscanf(fptr, "%s", magic_word);
	}

	if (strcmp(magic_word, "P1") == 0) {
		read_p1((IMAGE_BLACK_WHITE * *)(&((*image)->actual_image)), fptr);
		(*image)->magic_word = 1;
	} else if (strcmp(magic_word, "P2") == 0) {
		read_p2((IMAGE_GRAYSCALE * *)(&((*image)->actual_image)), fptr);
		(*image)->magic_word = 2;
	} else if (strcmp(magic_word, "P3") == 0) {
		read_p3((IMAGE_COLOR * *)(&((*image)->actual_image)), fptr);
		(*image)->magic_word = 3;
	}
//	else if (strcmp(magic_word, "P4") == 0)
//		read_p4(image, fptr);
//	else if (strcmp(magic_word, "P5") == 0)
//		read_p5(image, fptr);
//	else if (strcmp(magic_word, "P6") == 0)
//		read_p6(image, fptr);
	else {
		free(*image);
		(*image) = calloc(1, sizeof(PLACEHOLDER));
		(*image)->magic_word = -1;
		printf("Failed to load %s/n", file_name);
		return;
	}
}

void read_p1(IMAGE_BLACK_WHITE **image, FILE *fptr)
{
	free(*image);
	(*image) = calloc(1, sizeof(IMAGE_BLACK_WHITE));

	int i, j;
	int count = 0, done = 0;
	char line[1000];

	fgets(line, 1000, fptr);
	while (fgets(line, 1000, fptr) != NULL) {
		if (line[0] == '#')
			continue;

		char *token = strtok(line, " ");

		while (token != NULL) {

			switch (count) {
			case 0:
				(*image)->width = strtol(token, NULL, 10);
				break;

			case 1:
				(*image)->height = strtol(token, NULL, 10);
				break;

			case 2:
				(*image)->array = calloc((*image)->height, sizeof(int *));
				(*image)->array[0] = calloc((*image)->width, sizeof(int));
				j = 0;

				while (token != NULL) {
					(*image)->array[0][j] = strtol(token, NULL, 10);
					token = strtok(NULL, " ");
				}
				done = 1;
				break;
			}

			if (done == 1)
				break;

			count++;
			token = strtok(NULL, " ");
		}

		if (done == 1)
			break;
	}

	for (i = 1; i < (*image)->height; i++) {
		(*image)->array[i] = calloc((*image)->width, sizeof(int));
		for (j = 0; j < (*image)->width; j++)
			fscanf(fptr, "%d", &((*image)->array[i][j]));
	}
}

void read_p2(IMAGE_GRAYSCALE **image, FILE *fptr)
{
	free(*image);
	(*image) = calloc(1, sizeof(IMAGE_GRAYSCALE));

	int i, j;
	int count = 0, done = 0;
	char line[1000];

	fgets(line, 1000, fptr);
	while (fgets(line, 1000, fptr) != NULL) {
		if (line[0] == '#')
			continue;

		char *token = strtok(line, " ");

		while (token != NULL) {

			switch (count) {
			case 0:
				(*image)->width = strtol(token, NULL, 10);
				break;

			case 1:
				(*image)->height = strtol(token, NULL, 10);
				break;

			case 2:
				(*image)->scale = strtol(token, NULL, 10);
				break;

			case 3:
				(*image)->array = calloc((*image)->height, sizeof(int *));
				(*image)->array[0] = calloc((*image)->width, sizeof(int));
				j = 0;
				while (token != NULL) {
					(*image)->array[0][j] = strtol(token, NULL, 10);
					token = strtok(NULL, " ");
				}
				done = 1;
				break;
			}

			if (done == 1)
				break;

			count++;
			token = strtok(NULL, " ");
		}

		if (done == 1)
			break;
	}

	for (i = 1; i < (*image)->height; i++) {
		(*image)->array[i] = calloc((*image)->width, sizeof(int));
		for (j = 0; j < (*image)->width; j++)
			fscanf(fptr, "%d", &((*image)->array[i][j]));
	}
}

void read_p3(IMAGE_COLOR **image, FILE *fptr)
{
	free(*image);
	(*image) = calloc(1, sizeof(IMAGE_COLOR));
	int i, j, k;
	int count = 0, done = 0;
	char line[1000];

	fgets(line, 1000, fptr);
	while (fgets(line, 1000, fptr) != NULL) {
		if (line[0] == '#')
			continue;

		char *token = strtok(line, " ");

		while (token != NULL) {

			switch (count) {
			case 0:
				(*image)->width = strtol(token, NULL, 10);
				break;

			case 1:
				(*image)->height = strtol(token, NULL, 10);
				break;

			case 2:
				(*image)->scale = strtol(token, NULL, 10);
				break;

			case 3:
				j = 0;
				k = 0;

				(*image)->array = calloc((*image)->height, sizeof(int **));
				(*image)->array[0] = calloc((*image)->width, sizeof(int *));
				(*image)->array[0][0] = calloc(3, sizeof(int));
				while (token != NULL) {
					(*image)->array[0][j][k] = strtol(token, NULL, 10);
					k++;
					if (k == 3) {
						k = 0;
						j++;
						(*image)->array[0][j] = calloc(3, sizeof(int));
					}

					token = strtok(NULL, " ");
				}
				done = 1;
				break;
			}

			if (done == 1)
				break;

			count++;
			token = strtok(NULL, " ");
		}

		if (done == 1)
			break;
	}

	for (i = 1; i < (*image)->height; i++) {
		(*image)->array[i] = calloc((*image)->width, sizeof(int *));
		for (j = 0; j < (*image)->width; j++) {
			(*image)->array[i][j] = calloc(3, sizeof(int));
			for (k = 0; k < 3; k++)
				fscanf(fptr, "%d", &((*image)->array[i][j][k]));
		}
	}
}

//void read_p4(IMAGE **image, FILE *fptr)
//{
//}

void read_p5(IMAGE_GRAYSCALE **image, FILE *fptr)
{
	free(*image);
	(*image) = calloc(1, sizeof(IMAGE_GRAYSCALE));

	int i, j;
	int count = 0, done = 0;
	char line[1000];

	fgets(line, 1000, fptr);
	while (fgets(line, 1000, fptr) != NULL) {
		if (line[0] == '#')
			continue;

		char *token = strtok(line, " ");

		while (token != NULL) {

			switch (count) {
			case 0:
				(*image)->width = strtol(token, NULL, 10);
				break;

			case 1:
				(*image)->height = strtol(token, NULL, 10);
				break;

			case 2:
				(*image)->scale = strtol(token, NULL, 10);
				break;

			case 3:
				(*image)->array = calloc((*image)->height, sizeof(int *));
				(*image)->array[0] = calloc((*image)->width, sizeof(int));
				j = 0;
				while (token != NULL) {
					(*image)->array[0][j] = strtol(token, NULL, 10);
					token = strtok(NULL, " ");
				}
				done = 1;
				break;
			}

			if (done == 1)
				break;

			count++;
			token = strtok(NULL, " ");
		}

		if (done == 1)
			break;
	}

	for (i = 1; i < (*image)->height; i++) {
		(*image)->array[i] = calloc((*image)->width, sizeof(int));
		for (j = 0; j < (*image)->width; j++)
			fscanf(fptr, "%d", &((*image)->array[i][j]));
	}
}

//void read_p6(IMAGE **image, FILE *fptr)
//{
//}