#include "read_utils.h"

// TODO: Free correctly the memory allocated for the previous image

void LOAD(PLACEHOLDER **data)
{
	free(*data);
	(*data) = calloc(1, sizeof(PLACEHOLDER));
	(*data)->magic_word = -1;
	char *file_name = calloc(100, sizeof(char));
	char magic_word[3];


	scanf("%s", file_name);
	FILE *fptr = fopen(file_name, "r");
	if (fptr == NULL) {
		printf("Failed to load %s\n", file_name);
		return;
	}

	magic_word[0] = '#';
	while (magic_word[0] == '#') {
		fscanf(fptr, "%s", magic_word);
	}

	if (strcmp(magic_word, "P2") == 0) {
		read_p2(data, fptr);
		(*data)->magic_word = 2;
		printf("Loaded %s\n", file_name);
	} else if (strcmp(magic_word, "P3") == 0) {
		read_p3(data, fptr);
		(*data)->magic_word = 3;
		printf("Loaded %s\n", file_name);
	}
//	else if (strcmp(magic_word, "P5") == 0)
//		read_p5(data, fptr);
//	else if (strcmp(magic_word, "P6") == 0)
//		read_p6(data, fptr);
	else {
		free(*data);
		(*data) = calloc(1, sizeof(PLACEHOLDER));
		(*data)->magic_word = -1;
		printf("Failed to load %s\n", file_name);
	}
	fclose(fptr);
}

void read_p2(PLACEHOLDER **data, FILE *fptr)
{
	(*data)->image = calloc(1, sizeof(ACTUAL_IMAGE));

	int i, j;
	int count = 0, done = 0;
	char word[4];
	char to_brazil[10000];


	while (fscanf(fptr, "%s", word) != EOF) {
		if (word[0] == '#') {
			fgets(to_brazil, 10000, fptr);
			continue;
		}
		switch (count) {
		case 0:
			(*data)->width = strtol(word, NULL, 10);
			(*data)->x2 = (*data)->width;
			break;

		case 1:
			(*data)->height = strtol(word, NULL, 10);
			(*data)->y2 = (*data)->height;
			break;

		case 2:
			(*data)->scale = strtol(word, NULL, 10);
			break;

		case 3:
			(*data)->image->grayscale = calloc((*data)->height,
											   sizeof(int *));
			(*data)->image->grayscale[0] = calloc((*data)->width,
												  sizeof(int));
			(*data)->image->grayscale[0][0] = strtol(word, NULL, 10);
			done = 1;
			break;
		}

		if (done == 1)
			break;
		count++;
	}

	for (i = 0; i < (*data)->height; i++) {
		if (i != 0)
			(*data)->image->grayscale[i] = calloc((*data)->width,
												  sizeof(int));
		for (j = 0; j < (*data)->width; j++)
			if (i != 0 || j != 0)
				fscanf(fptr, "%d", &((*data)->image->grayscale[i][j]));
	}
}

void read_p3(PLACEHOLDER **data, FILE *fptr)
{
	(*data)->image = calloc(1, sizeof(ACTUAL_IMAGE));
	int i, j, k;
	int count = 0, done = 0;
	char word[4];
	char to_brazil[10000];

	while (fscanf(fptr, "%s", word) != EOF) {
		if (word[0] == '#') {
			fgets(to_brazil, 10000, fptr);
			continue;
		}

		switch (count) {
		case 0:
			(*data)->width = strtol(word, NULL, 10);
			(*data)->x2 = (*data)->width;
			break;

		case 1:
			(*data)->height = strtol(word, NULL, 10);
			(*data)->y2 = (*data)->height;
			break;

		case 2:
			(*data)->scale = strtol(word, NULL, 10);
			break;

		case 3:
			(*data)->image->color = calloc((*data)->height, sizeof(int **));
			(*data)->image->color[0] = calloc((*data)->width, sizeof(int *));
			(*data)->image->color[0][0] = calloc(3, sizeof(int));
			(*data)->image->color[0][0][0] = strtol(word, NULL, 10);
			done = 1;
			break;
		}

		if (done == 1)
			break;
		count++;
	}

	for (i = 0; i < (*data)->height; i++) {
		if (i != 0)
			(*data)->image->color[i] = calloc((*data)->width, sizeof(int *));
		for (j = 0; j < (*data)->width; j++) {
			if (i != 0 || j != 0)
				(*data)->image->color[i][j] = calloc(3, sizeof(int));
			for (k = 0; k < 3; k++)
				if (i != 0 || j != 0 || k != 0)
					fscanf(fptr, "%d", &((*data)->image->color[i][j][k]));

		}
	}
}

//void read_p5(PLACEHOLDER **image, FILE *fptr)
//{
//	free(*image);
//	(*image) = calloc(1, sizeof(IMAGE_GRAYSCALE));
//
//	int i, j;
//	int count = 0, done = 0;
//	char line[1000];
//
//	fgets(line, 1000, fptr);
//	while (fgets(line, 1000, fptr) != NULL) {
//		if (line[0] == '#')
//			continue;
//
//		char *token = strtok(line, " ");
//
//		while (token != NULL) {
//
//			switch (count) {
//			case 0:
//				(*image)->width = strtol(token, NULL, 10);
//				(*image)->x2 = (*image)->width;
//				break;
//
//			case 1:
//				(*image)->height = strtol(token, NULL, 10);
//				(*image)->y2 = (*image)->height;
//				break;
//
//			case 2:
//				(*image)->scale = strtol(token, NULL, 10);
//				break;
//
//			case 3:
//				(*image)->array = calloc((*image)->height, sizeof(int *));
//				(*image)->array[0] = calloc((*image)->width, sizeof(int));
//				j = 0;
//				while (token != NULL) {
//					(*image)->array[0][j] = strtol(token, NULL, 10);
//					token = strtok(NULL, " ");
//				}
//				done = 1;
//				break;
//			}
//
//			if (done == 1)
//				break;
//
//			count++;
//			token = strtok(NULL, " ");
//		}
//
//		if (done == 1)
//			break;
//	}
//
//	for (i = 1; i < (*image)->height; i++) {
//		(*image)->array[i] = calloc((*image)->width, sizeof(int));
//		for (j = 0; j < (*image)->width; j++)
//			fscanf(fptr, "%d", &((*image)->array[i][j]));
//	}
//}

//void read_p6(IMAGE **image, FILE *fptr)
//{
//}