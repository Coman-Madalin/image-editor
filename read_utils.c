#include "read_utils.h"

// TODO: Free correctly the memory allocated for the previous image

void LOAD(PLACEHOLDER **data, char *file_name)
{
	FILE *fptr = NULL;
	fptr = fopen(file_name, "r");
	if (!fptr) {
		printf("Failed to load %s\n", file_name);
		return;
	}

	int first_byte = read_common(data, fptr);

	if ((*data)->magic_word == 2) {
		read_p2(data, fptr, first_byte - '0');
		printf("Loaded %s\n", file_name);

	} else if ((*data)->magic_word == 3) {
		read_p3(data, fptr, first_byte - '0');
		printf("Loaded %s\n", file_name);

	} else if ((*data)->magic_word == 5) {
		int pos = ftell(fptr);
		fclose(fptr);
		fptr = fopen(file_name, "rb");
		fseek(fptr, pos, SEEK_SET);
		read_p5(data, fptr, first_byte);
		(*data)->magic_word = 2;
		printf("Loaded %s\n", file_name);

	} else if ((*data)->magic_word == 6) {
		int pos = ftell(fptr);
		fclose(fptr);
		fptr = fopen(file_name, "rb");
		fseek(fptr, pos, SEEK_SET);
		read_p6(data, fptr, first_byte);
		(*data)->magic_word = 3;
		printf("Loaded %s\n", file_name);

	} else {
		(*data) = calloc(1, sizeof(PLACEHOLDER));
		(*data)->magic_word = -1;
		printf("Failed to load %s\n", file_name);
	}
	fclose(fptr);
}

int read_common(PLACEHOLDER **data, FILE *fptr)
{
	int count = 0;
	int byte;
	char to_brazil[10000];
	while ((byte = fgetc(fptr)) != EOF) {
		if (byte == 'P')
			continue;
		if (byte == '\n' || byte == ' ') {
			count++;
			continue;
		}
		if (byte == '#') {
			fgets(to_brazil, 10000, fptr);
			continue;
		}
		switch (count) {
		case 0:
			if (byte == '2')
				(*data)->magic_word = 2;
			else if (byte == '3')
				(*data)->magic_word = 3;
			else if (byte == '5')
				(*data)->magic_word = 5;
			else if (byte == '6')
				(*data)->magic_word = 6;
			else {
				(*data)->magic_word = -1;
				return -1;
			}
			break;
		case 1:
			(*data)->width = (*data)->width * 10 + byte - '0';
			(*data)->x2 = (*data)->width;
			break;

		case 2:
			(*data)->height = (*data)->height * 10 + byte - '0';
			(*data)->y2 = (*data)->height;
			break;

		case 3:
			(*data)->scale = (*data)->scale * 10 + byte - '0';
			break;
		case 4:
			return byte;
		default:
			break;
		}
	}
	return -1;
}

void read_p2(PLACEHOLDER **data, FILE *fptr, int first_element)
{
	(*data)->image = calloc(1, sizeof(ACTUAL_IMAGE));
	(*data)->image->grayscale = calloc((*data)->height, sizeof(int *));

	int i, j;
	for (i = 0; i < (*data)->height; i++) {
		(*data)->image->grayscale[i] = calloc((*data)->width, sizeof(int));
		for (j = 0; j < (*data)->width; j++) {
			int value;
			fscanf(fptr, "%d", &value);
			clamp(&value, 0, (*data)->scale);
			(*data)->image->grayscale[i][j] = value;
		}
	}
	(*data)->image->grayscale[0][0] = first_element * 10 +
									  (*data)->image->grayscale[0][0];
}

void read_p3(PLACEHOLDER **data, FILE *fptr, int first_element)
{
	(*data)->image = calloc(1, sizeof(ACTUAL_IMAGE));
	(*data)->image->color = calloc((*data)->height, sizeof(int **));

	int i, j, k;
	for (i = 0; i < (*data)->height; i++) {
		(*data)->image->color[i] = calloc((*data)->width, sizeof(int *));
		for (j = 0; j < (*data)->width; j++) {
			(*data)->image->color[i][j] = calloc(3, sizeof(int));
			for (k = 0; k < 3; k++) {
				int value;
				fscanf(fptr, "%d", &value);
				clamp(&value, 0, (*data)->scale);
				(*data)->image->color[i][j][k] = value;
			}
		}
	}
	(*data)->image->color[0][0][0] = first_element * 10 +
									 (*data)->image->color[0][0][0];
}

void read_p5(PLACEHOLDER **data, FILE *fptr, int first_element)
{
	(*data)->image = calloc(1, sizeof(ACTUAL_IMAGE));
	(*data)->image->grayscale = calloc((*data)->height, sizeof(int *));
	int i, j;
	for (i = 0; i < (*data)->height; i++) {
		(*data)->image->grayscale[i] = calloc((*data)->width, sizeof(int));
		for (j = 0; j < (*data)->width; j++)
			if (i != 0 || j != 0) {
				int value = fgetc(fptr);
				clamp(&value, 0, (*data)->scale);
				(*data)->image->grayscale[i][j] = value;
			}
	}
	(*data)->image->grayscale[0][0] = first_element;
}

void read_p6(PLACEHOLDER **data, FILE *fptr, int first_element)
{
	(*data)->image = calloc(1, sizeof(ACTUAL_IMAGE));
	(*data)->image->color = calloc((*data)->height, sizeof(int **));
	int i, j, k;
	for (i = 0; i < (*data)->height; i++) {
		(*data)->image->color[i] = calloc((*data)->width, sizeof(int *));
		for (j = 0; j < (*data)->width; j++) {
			(*data)->image->color[i][j] = calloc(3, sizeof(int));
			for (k = 0; k < 3; k++)
				if (i != 0 || j != 0 || k != 0) {
					int value = fgetc(fptr);
					clamp(&value, 0, (*data)->scale);
					(*data)->image->color[i][j][k] = value;
				}
		}
	}
	(*data)->image->color[0][0][0] = first_element;
}
