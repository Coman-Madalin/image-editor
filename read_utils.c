#include "read_utils.h"

// TODO: Free correctly the memory allocated for the previous image

void LOAD(PLACEHOLDER **data)
{
	free(*data);
	(*data) = calloc(1, sizeof(PLACEHOLDER));
	(*data)->magic_word = -1;
	char *file_name = calloc(100, sizeof(char));

	scanf("%s", file_name);
	FILE *fptr = fopen(file_name, "r");
	if (fptr == NULL) {
		printf("Failed to load %s\n", file_name);
		return;
	}
	read_common(data, fptr);

	if ((*data)->magic_word == 2) {
		read_p2(data, fptr);
		printf("Loaded %s\n", file_name);

	} else if ((*data)->magic_word == 3) {
		read_p3(data, fptr);
		printf("Loaded %s\n", file_name);

	} else if ((*data)->magic_word == 5) {
		int pos = ftell(fptr);
		fclose(fptr);
		fptr = fopen(file_name, "rb");
		fseek(fptr, pos, SEEK_SET);
		read_p5(data, fptr);
		(*data)->magic_word = 2;
		printf("Loaded %s\n", file_name);

	} else if ((*data)->magic_word == 6) {
		int pos = ftell(fptr);
		fclose(fptr);
		fptr = fopen(file_name, "rb");
		fseek(fptr, pos, SEEK_SET);
		read_p6(data, fptr);
		(*data)->magic_word = 3;
		printf("Loaded %s\n", file_name);

	} else {
		free(*data);
		(*data) = calloc(1, sizeof(PLACEHOLDER));
		(*data)->magic_word = -1;
		printf("Failed to load %s\n", file_name);
	}
	fclose(fptr);
}

void read_common(PLACEHOLDER **data, FILE *fptr)
{
	int count = 0, pos = -1;
	char word[5];
	char to_brazil[10000];


	while (fgets(word, 5, fptr)) {
		if (word[0] == '#') {
			fgets(to_brazil, 10000, fptr);
			continue;
		}
		switch (count) {
		case 0:
			if (strncmp(word, "P2", 2) == 0)
				(*data)->magic_word = 2;
			else if (strncmp(word, "P3", 2) == 0)
				(*data)->magic_word = 3;
			else if (strncmp(word, "P5", 2) == 0)
				(*data)->magic_word = 5;
			else if (strncmp(word, "P6", 2) == 0)
				(*data)->magic_word = 6;
			else {
				(*data)->magic_word = -1;
				return;
			}
			break;
		case 1:
			(*data)->width = strtol(word, NULL, 10);
			(*data)->x2 = (*data)->width;
			break;

		case 2:
			(*data)->height = strtol(word, NULL, 10);
			(*data)->y2 = (*data)->height;
			break;

		case 3:
			(*data)->scale = strtol(word, NULL, 10);
			break;
		case 4:
			fseek(fptr, pos, SEEK_SET);
			return;
		default:
			break;
		}
		pos = ftell(fptr);
		count++;
	}

}

void read_p2(PLACEHOLDER **data, FILE *fptr)
{
	(*data)->image = calloc(1, sizeof(ACTUAL_IMAGE));
	(*data)->image->grayscale = calloc((*data)->height, sizeof(int *));

	int i, j;
	for (i = 0; i < (*data)->height; i++) {
		(*data)->image->grayscale[i] = calloc((*data)->width, sizeof(int));
		for (j = 0; j < (*data)->width; j++)
			fscanf(fptr, "%d", &((*data)->image->grayscale[i][j]));
	}
}

void read_p3(PLACEHOLDER **data, FILE *fptr)
{
	(*data)->image = calloc(1, sizeof(ACTUAL_IMAGE));
	(*data)->image->color = calloc((*data)->height, sizeof(int **));

	int i, j, k;
	for (i = 0; i < (*data)->height; i++) {
		(*data)->image->color[i] = calloc((*data)->width, sizeof(int *));
		for (j = 0; j < (*data)->width; j++) {
			(*data)->image->color[i][j] = calloc(3, sizeof(int));
			for (k = 0; k < 3; k++)
				fscanf(fptr, "%d", &((*data)->image->color[i][j][k]));
		}
	}
}

void read_p5(PLACEHOLDER **data, FILE *fptr)
{
	(*data)->image = calloc(1, sizeof(ACTUAL_IMAGE));
	(*data)->image->grayscale = calloc((*data)->height, sizeof(int *));
	int i, j;
	for (i = 0; i < (*data)->height; i++) {
		(*data)->image->grayscale[i] = calloc((*data)->width, sizeof(int));
		for (j = 0; j < (*data)->width; j++)
			(*data)->image->grayscale[i][j] = fgetc(fptr);
	}
}

void read_p6(PLACEHOLDER **data, FILE *fptr)
{
	(*data)->image = calloc(1, sizeof(ACTUAL_IMAGE));
	(*data)->image->color = calloc((*data)->height, sizeof(int **));
	int i, j, k;
	for (i = 0; i < (*data)->height; i++) {
		(*data)->image->color[i] = calloc((*data)->width, sizeof(int *));
		for (j = 0; j < (*data)->width; j++) {
			(*data)->image->color[i][j] = calloc(3, sizeof(int));
			for (k = 0; k < 3; k++)
				(*data)->image->color[i][j][k] = fgetc(fptr);
		}
	}
}