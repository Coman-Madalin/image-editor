// Image Editor
// Coman Andrei-Madalin
// 2023-2024

#include "read_utils.h"

void LOAD(DATA_t **data, char *file_name)
{
	FILE *fptr = NULL;
	fptr = fopen(file_name, "r");
	if (!fptr) {
		printf("Failed to load %s\n", file_name);
		return;
	}

	// Will remember the first digit because is read in the read_common
	// function to check if it is a comment and I found out that using
	// fseek(fptr, -1, SEEK_CUR) has some undefined behaviour while reading.
	// If the image is P2 or P3, I will add the first digit to the
	// front of the first pixel.
	// If the image is P5 or P6, I will make the first pixel equal to the
	// first digit.
	int first_digit = read_common(data, fptr);

	if ((*data)->magic_word == 2) {
		read_p2(data, fptr, first_digit - '0');
		printf("Loaded %s\n", file_name);

	} else if ((*data)->magic_word == 3) {
		read_p3(data, fptr, first_digit - '0');
		printf("Loaded %s\n", file_name);

	} else if ((*data)->magic_word == 5) {
		int pos = ftell(fptr);
		fclose(fptr);
		// I need to reopen the file because I need to read in binary mode
		fptr = fopen(file_name, "rb");
		// I need to go back to the position where I was before closing the file
		fseek(fptr, pos, SEEK_SET);

		read_p5(data, fptr, first_digit);
		(*data)->magic_word = 2;
		printf("Loaded %s\n", file_name);

	} else if ((*data)->magic_word == 6) {
		int pos = ftell(fptr);
		fclose(fptr);
		// I need to reopen the file because I need to read in binary mode
		fptr = fopen(file_name, "rb");
		// I need to go back to the position where I was before closing the file
		fseek(fptr, pos, SEEK_SET);

		read_p6(data, fptr, first_digit);
		(*data)->magic_word = 3;
		printf("Loaded %s\n", file_name);

	} else {
		(*data) = calloc(1, sizeof(DATA_t));
		(*data)->magic_word = -1;
		printf("Failed to load %s\n", file_name);
	}
	fclose(fptr);
}

int read_common(DATA_t **data, FILE *fptr)
{
	// count is used to keep track of what we currently read about the image
	int count = 0;
	int digit;
	// A buffer to discard the comments
	char to_brazil[10000];

	// Read one character, that is a cast to an int, one by one
	while ((digit = fgetc(fptr)) != EOF) {
		if (digit == 'P')
			continue;

		// count is incremented after we read a space or a new line, which means
		// that we read the previous number completely
		if (digit == '\n' || digit == ' ') {
			count++;
			continue;
		}

		// If we read a comment, we discard it
		if (digit == '#') {
			fgets(to_brazil, 10000, fptr);
			continue;
		}

		// If we read a digit, we add it to the corresponding variable
		switch (count) {
		case 0:
			if (digit >= '2' && digit <= '6' && digit != '4') {
				(*data)->magic_word = digit - '0';
			} else {
				(*data)->magic_word = -1;
				return -1;
			}
			break;
		case 1:
			(*data)->width = (*data)->width * 10 + (digit - '0');
			(*data)->x2 = (*data)->width;
			break;

		case 2:
			(*data)->height = (*data)->height * 10 + (digit - '0');
			(*data)->y2 = (*data)->height;
			break;

		case 3:
			(*data)->scale = (*data)->scale * 10 + (digit - '0');
			break;
		case 4:
			return digit;
		default:
			break;
		}
	}
	return -1;
}

void read_p2(DATA_t **data, FILE *fptr, int first_element)
{
	(*data)->image = calloc(1, sizeof(ACTUAL_IMAGE_t));
	(*data)->image->grayscale = calloc((*data)->height, sizeof(int *));

	int i, j;
	for (i = 0; i < (*data)->height; i++) {
		(*data)->image->grayscale[i] = calloc((*data)->width, sizeof(int));
		for (j = 0; j < (*data)->width; j++) {
			int value;
			fscanf(fptr, "%d", &value);
			(*data)->image->grayscale[i][j] = value;
		}
	}
	(*data)->image->grayscale[0][0] =
			first_element * 10 + (*data)->image->grayscale[0][0];
}

void read_p3(DATA_t **data, FILE *fptr, int first_element)
{
	(*data)->image = calloc(1, sizeof(ACTUAL_IMAGE_t));
	(*data)->image->color = calloc((*data)->height, sizeof(int **));

	int i, j, k;
	for (i = 0; i < (*data)->height; i++) {
		(*data)->image->color[i] = calloc((*data)->width, sizeof(int *));
		for (j = 0; j < (*data)->width; j++) {
			(*data)->image->color[i][j] = calloc(3, sizeof(int));
			for (k = 0; k < 3; k++) {
				int value;
				fscanf(fptr, "%d", &value);
				(*data)->image->color[i][j][k] = value;
			}
		}
	}
	(*data)->image->color[0][0][0] =
			first_element * 10 + (*data)->image->color[0][0][0];
}

void read_p5(DATA_t **data, FILE *fptr, int first_element)
{
	(*data)->image = calloc(1, sizeof(ACTUAL_IMAGE_t));
	(*data)->image->grayscale = calloc((*data)->height, sizeof(int *));
	int i, j;
	for (i = 0; i < (*data)->height; i++) {
		(*data)->image->grayscale[i] = calloc((*data)->width, sizeof(int));
		for (j = 0; j < (*data)->width; j++)
			if (i != 0 || j != 0) {
				int value = fgetc(fptr);
				(*data)->image->grayscale[i][j] = value;
			}
	}
	(*data)->image->grayscale[0][0] = first_element;
}

void read_p6(DATA_t **data, FILE *fptr, int first_element)
{
	(*data)->image = calloc(1, sizeof(ACTUAL_IMAGE_t));
	(*data)->image->color = calloc((*data)->height, sizeof(int **));
	int i, j, k;
	for (i = 0; i < (*data)->height; i++) {
		(*data)->image->color[i] = calloc((*data)->width, sizeof(int *));
		for (j = 0; j < (*data)->width; j++) {
			(*data)->image->color[i][j] = calloc(3, sizeof(int));
			for (k = 0; k < 3; k++)
				if (i != 0 || j != 0 || k != 0) {
					int value = fgetc(fptr);
					(*data)->image->color[i][j][k] = value;
				}
		}
	}
	(*data)->image->color[0][0][0] = first_element;
}
