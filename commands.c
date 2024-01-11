// Tema3 - Image Editor
// Coman Andrei-Madalin
// 315 CA
// 2023-2024

#include "commands.h"

void SELECT(DATA_t **data, int x1, int y1, int x2, int y2)
{
	if (is_loaded(*data) == 0)
		return;

	if (x1 == x2 || y1 == y2) {
		printf("Invalid set of coordinates\n");
		return;
	}

	if (x1 > x2) {
		int aux = x1;
		x1 = x2;
		x2 = aux;
	}
	if (y1 > y2) {
		int aux = y1;
		y1 = y2;
		y2 = aux;
	}

	if (x1 < 0) {
		printf("Invalid set of coordinates\n");
		return;
	}
	if (y1 < 0) {
		printf("Invalid set of coordinates\n");
		return;
	}
	if (x2 > (*data)->width) {
		printf("Invalid set of coordinates\n");
		return;
	}
	if (y2 > (*data)->height) {
		printf("Invalid set of coordinates\n");
		return;
	}

	(*data)->x1 = x1;
	(*data)->y1 = y1;
	(*data)->x2 = x2;
	(*data)->y2 = y2;

	printf("Selected %d %d %d %d\n", x1, y1, x2, y2);
}

void HISTOGRAM(DATA_t *data, int bins, int max_stars)
{
	if (data->magic_word == 3) {
		printf("Black and white image needed\n");
		return;
	}

	int vf[256] = {0};
	int i, j;

	for (i = data->y1; i < data->y2; i++)
		for (j = data->x1; j < data->x2; j++) {
			int value = data->image->grayscale[i][j];
			vf[value]++;
		}

	int *bin = calloc(bins, sizeof(int));
	int values_per_bin = (data->scale + 1) / bins;
	// Could have still used the same vf array, but it would have added
	// unnecessary complexity to the code, so I decided to use a new array,
	// that has the maximum size of 256, so it's not a big deal.
	for (i = 0; i <= data->scale; i++) {
		int index = i / values_per_bin;
		bin[index] += vf[i];
	}

	int max_freq = 0;
	for (i = 0; i < bins; i++) {
		if (bin[i] > max_freq)
			max_freq = bin[i];
	}

	for (i = 0; i < bins; i++) {
		int nr_stars = trunc((double)bin[i] * max_stars / max_freq);
		printf("%d\t|\t", nr_stars);
		for (j = 0; j < nr_stars; j++)
			printf("*");
		printf("\n");
	}
	free(bin);
}

void EQUALIZE(DATA_t **data)
{
	if (is_loaded(*data) == 0)
		return;

	if ((*data)->magic_word == 3) {
		printf("Black and white image needed\n");
		return;
	}

	int vf[256][3] = {0};
	int i, j;
	// The first column of vf will store the frequency of each value
	for (i = 0; i < (*data)->height; i++)
		for (j = 0; j < (*data)->width; j++) {
			int value = (*data)->image->grayscale[i][j];
			vf[value][0]++;
		}

	int cumulative_freq = 0;
	int min_cdf;
	for (i = 0; i <= (*data)->scale; i++)
		if (vf[i]) {
			if (cumulative_freq == 0)
				min_cdf = vf[i][0];
			cumulative_freq += vf[i][0];
			// The second column of vf will store the cumulative frequency
			vf[i][1] = cumulative_freq;
		}

	for (i = 0; i <= (*data)->scale; i++)
		if (vf[i])
			// The third column of vf will store the new value
			vf[i][2] = round((double)(vf[i][1] - min_cdf) /
							 (((*data)->width * (*data)->height) -
							  min_cdf) * (*data)->scale);

	for (i = 0; i < (*data)->height; i++)
		for (j = 0; j < (*data)->width; j++) {
			int value = (*data)->image->grayscale[i][j];
			(*data)->image->grayscale[i][j] = vf[value][2];
		}

	printf("Equalize done\n");
}

void ROTATE_ALL(DATA_t **data, int angle)
{
	// Swapping width and height of the new image if needed
	int new_width, new_height;
	switch (angle) {
	case 90:
		new_width = (*data)->height;
		new_height = (*data)->width;
		break;
	case 180:
		new_width = (*data)->width;
		new_height = (*data)->height;
		break;
	case 270:
		new_width = (*data)->height;
		new_height = (*data)->width;
		break;
	}

	ACTUAL_IMAGE_t *new_image = calloc(1, sizeof(ACTUAL_IMAGE_t));
	int i, j, k;
	if ((*data)->magic_word == 2) {
		new_image->grayscale = calloc(new_height, sizeof(int *));
		for (i = 0; i < new_height; i++) {
			new_image->grayscale[i] = calloc(new_width, sizeof(int));
			for (j = 0; j < new_width; j++) {
				if (angle == 90)
					new_image->grayscale[i][j] = (*data)->image->grayscale
					[new_width - 1 - j][i];
				else if (angle == 180)
					new_image->grayscale[i][j] = (*data)->image->grayscale
					[new_height - 1 - i][new_width - 1 - j];
				else
					new_image->grayscale[i][j] = (*data)->image->grayscale[j]
					[new_height - 1 - i];
			}
		}

		// Free grayscale specific memory
		for (i = 0; i < (*data)->height; i++)
			free((*data)->image->grayscale[i]);
		free((*data)->image->grayscale);
	} else {
		new_image->color = calloc(new_height, sizeof(int **));
		for (i = 0; i < new_height; i++) {
			new_image->color[i] = calloc(new_width, sizeof(int *));
			for (j = 0; j < new_width; j++) {
				new_image->color[i][j] = calloc(3, sizeof(int));
				for (k = 0; k < 3; k++)
					if (angle == 90) {
						new_image->color[i][j][k] = (*data)->image->color
						[new_width - j - 1][i][k];
					} else if (angle == 180) {
						new_image->color[i][j][k] = (*data)->image->color
						[new_height - i - 1][new_width - j - 1][k];
					} else {
						new_image->color[i][j][k] = (*data)->image->color
						[j][new_height - i - 1][k];
					}
			}
		}
		// Free color specific memory
		for (i = 0; i < (*data)->height; i++) {
			for (j = 0; j < (*data)->width; j++)
				free((*data)->image->color[i][j]);
			free((*data)->image->color[i]);
		}
		free((*data)->image->color);
	}
	// Free common memory
	free((*data)->image);
	(*data)->image = new_image;
	(*data)->width = new_width;
	(*data)->height = new_height;
	(*data)->x1 = (*data)->y1 = 0;
	(*data)->x2 = new_width;
	(*data)->y2 = new_height;
}

void ROTATE_ZONE(DATA_t **data, int angle)
{
	ACTUAL_IMAGE_t *new_image = calloc(1, sizeof(ACTUAL_IMAGE_t));
	int i, j, k;
	int old_i, old_j;
	// Using auxiliary variables because the formula for the new coordinates
	// is quite lengthy and I think it's more readable this way.
	if ((*data)->magic_word == 2) {
		new_image->grayscale = calloc((*data)->height, sizeof(int *));
		for (i = 0; i < (*data)->height; i++) {
			new_image->grayscale[i] = calloc((*data)->width, sizeof(int));
			for (j = 0; j < (*data)->width; j++)
				new_image->grayscale[i][j] = (*data)->image->grayscale[i][j];
		}

		for (i = (*data)->y1; i < (*data)->y2; i++)
			for (j = (*data)->x1; j < (*data)->x2; j++) {
				if (angle == 90) {
					old_i = (*data)->y2 - 1 - (j - (*data)->x1);
					old_j = (*data)->x1 + (i - (*data)->y1);
				} else if (angle == 180) {
					old_i = (*data)->y2 - 1 - (i - (*data)->y1);
					old_j = (*data)->x2 - 1 - (j - (*data)->x1);
				} else {
					old_i = (*data)->y1 + (j - (*data)->x1);
					old_j = (*data)->x2 - 1 - (i - (*data)->y1);
				}
				new_image->grayscale[i][j] =
						(*data)->image->grayscale[old_i][old_j];
			}

		// Free old image
		for (i = 0; i < (*data)->height; i++)
			free((*data)->image->grayscale[i]);
		free((*data)->image->grayscale);
	} else {
		new_image->color = calloc((*data)->height, sizeof(int **));
		for (i = 0; i < (*data)->height; i++) {
			new_image->color[i] = calloc((*data)->width, sizeof(int *));
			for (j = 0; j < (*data)->width; j++) {
				new_image->color[i][j] = calloc(3, sizeof(int));
				for (k = 0; k < 3; k++)
					new_image->color[i][j][k] = (*data)->image->color[i][j][k];
			}
		}

		for (i = (*data)->y1; i < (*data)->y2; i++)
			for (j = (*data)->x1; j < (*data)->x2; j++) {
				if (angle == 90) {
					old_i = (*data)->y2 - 1 - (j - (*data)->x1);
					old_j = (*data)->x1 + (i - (*data)->y1);
				} else if (angle == 180) {
					old_i = (*data)->y2 - 1 - (i - (*data)->y1);
					old_j = (*data)->x2 - 1 - (j - (*data)->x1);
				} else {
					old_i = (*data)->y1 + (j - (*data)->x1);
					old_j = (*data)->x2 - 1 - (i - (*data)->y1);
				}
				for (k = 0; k < 3; k++)
					new_image->color[i][j][k] =
							(*data)->image->color[old_i][old_j][k];
			}

		// Free old image
		for (i = 0; i < (*data)->height; i++) {
			for (j = 0; j < (*data)->width; j++)
				free((*data)->image->color[i][j]);
			free((*data)->image->color[i]);
		}
		free((*data)->image->color);
	}
	free((*data)->image);
	(*data)->image = new_image;
}

void ROTATE(DATA_t **data, int angle)
{
	if (is_loaded(*data) == 0)
		return;
	int square = is_square(*data);
	if (square == 0)
		return;
	int positive_angle = angle;
	if (angle < 0)
		positive_angle = 360 + angle;
	if (positive_angle == 0 || positive_angle == 360) {
		printf("Rotated %d\n", angle);
		return;
	}
	if (positive_angle % 90 != 0 || positive_angle > 270 ||
		positive_angle < 0) {
		printf("Unsupported rotation angle\n");
		return;
	}
	if (square == 2)
		ROTATE_ALL(data, positive_angle);
	else
		ROTATE_ZONE(data, positive_angle);
	printf("Rotated %d\n", angle);
}

void CROP(DATA_t **data)
{
	if (is_loaded(*data) == 0)
		return;
	if ((*data)->x1 == 0 && (*data)->x2 == (*data)->width &&
		(*data)->y1 == 0 && (*data)->y2 == (*data)->height) {
		printf("Image cropped\n");
		return;
	}

	ACTUAL_IMAGE_t *new_image = calloc(1, sizeof(ACTUAL_IMAGE_t));

	// I prefer to allocate memory for the new image and then copy the selected
	// pixels from the old image because it much more readable.
	// I used the second method for a color image because it seems a little
	// more efficient, using only one for loop instead of two sequential ones.
	// I think the difference is insignificant, and the compromise on
	// readability is not worth it.
	// I would love to get some feedback on this.

	int i, j, k;
	if ((*data)->magic_word == 2) {
		// Allocate memory for new image
		new_image->grayscale =
				calloc((*data)->y2 - (*data)->y1, sizeof(int *));
		for (i = (*data)->y1; i < (*data)->y2; i++) {
			int new_width = (*data)->x2 - (*data)->x1;
			new_image->grayscale[i - (*data)->y1] =
					calloc(new_width, sizeof(int));
			if (!new_image->grayscale[i - (*data)->y1]) {
				printf("Error allocating memory\n");
				return;
			}

			// Copy selected pixels from old image
			for (j = (*data)->x1; j < (*data)->x2; j++) {
				new_image->grayscale[i - (*data)->y1][j - (*data)->x1] =
						(*data)->image->grayscale[i][j];
			}
		}
		// Free old image
		for (i = 0; i < (*data)->height; i++)
			free((*data)->image->grayscale[i]);
		free((*data)->image->grayscale);

	} else {
		// Allocate memory AND copy selected pixels from old image
		new_image->color = calloc((*data)->y2 - (*data)->y1, sizeof(int **));
		for (i = (*data)->y1; i < (*data)->y2; i++) {
			int new_width = (*data)->x2 - (*data)->x1;
			new_image->color[i - (*data)->y1] =
					calloc(new_width, sizeof(int *));
			for (j = (*data)->x1; j < (*data)->x2; j++) {
				new_image->color[i - (*data)->y1][j - (*data)->x1] =
						calloc(3, sizeof(int));
				for (k = 0; k < 3; k++)
					new_image->color[i - (*data)->y1][j - (*data)->x1][k] =
							(*data)->image->color[i][j][k];
			}
		}
		// Free old image
		for (i = 0; i < (*data)->height; i++) {
			for (j = 0; j < (*data)->width; j++)
				free((*data)->image->color[i][j]);
			free((*data)->image->color[i]);
		}
		free((*data)->image->color);
	}

	free((*data)->image);
	(*data)->image = new_image;
	(*data)->width = (*data)->x2 - (*data)->x1;
	(*data)->height = (*data)->y2 - (*data)->y1;
	(*data)->x1 = (*data)->y1 = 0;
	(*data)->x2 = (*data)->width;
	(*data)->y2 = (*data)->height;
	printf("Image cropped\n");
}

void APPLY(DATA_t **data, char *parameter)
{
	if (!parameter) {
		if (is_loaded(*data) != 0)
			printf("Invalid command\n");
		return;
	}
	if (is_loaded(*data) == 0)
		return;
	if (is_chaplin(*data) == 1)
		return;

	int kernel[3][3];
	int coefficient;
	// I printed the message before modifying the image because I already did
	// verify the restrictions so everything should be fine. Doing another
	// nested if, just to print the message seems redundant.
	if (strcmp(parameter, "EDGE") == 0) {
		EDGE(kernel, &coefficient);
		printf("APPLY EDGE done\n");
	} else if (strcmp(parameter, "SHARPEN") == 0) {
		SHARPEN(kernel, &coefficient);
		printf("APPLY SHARPEN done\n");
	} else if (strcmp(parameter, "BLUR") == 0) {
		BLUR(kernel, &coefficient);
		printf("APPLY BLUR done\n");
	} else if (strcmp(parameter, "GAUSSIAN_BLUR") == 0) {
		GAUSSIAN_BLUR(kernel, &coefficient);
		printf("APPLY GAUSSIAN_BLUR done\n");
	} else {
		printf("APPLY parameter invalid\n");
		return;
	}
	APPLY_UTIL(coefficient, kernel, data);
}

int save_ascii(DATA_t *data, char *filename)
{
	FILE *f = fopen(filename, "w");
	if (!f) {
		printf("Error opening file\n");
		return 1;
	}

	fprintf(f, "P%d\n", data->magic_word);
	fprintf(f, "%d %d\n", data->width, data->height);
	fprintf(f, "%d\n", data->scale);

	int i, j, k;
	if (data->magic_word == 2) {
		for (i = 0; i < data->height; i++) {
			for (j = 0; j < data->width; j++)
				fprintf(f, "%d ", data->image->grayscale[i][j]);
			fprintf(f, "\n");
		}
	} else if (data->magic_word == 3) {
		for (i = 0; i < data->height; i++) {
			for (j = 0; j < data->width; j++)
				for (k = 0; k < 3; k++)
					fprintf(f, "%d ",
							data->image->color[i][j][k]);
			fprintf(f, "\n");
		}
	}
	fclose(f);
	return 0;
}

int save_binary(DATA_t *data, char *filename)
{
	FILE *f = fopen(filename, "wb");
	if (!f) {
		printf("Error opening file\n");
		return 1;
	}

	// Change the magic word from ascii image to binary image, but don't change
	// the value in memory
	if (data->magic_word == 2)
		fprintf(f, "P5\n");
	else
		fprintf(f, "P6\n");
	fprintf(f, "%d %d\n", data->width, data->height);
	fprintf(f, "%d\n", data->scale);

	int i, j, k;
	if (data->magic_word == 2) {
		for (i = 0; i < data->height; i++)
			for (j = 0; j < data->width; j++)
				fwrite(&data->image->grayscale[i][j], 1, 1, f);

	} else if (data->magic_word == 3) {
		for (i = 0; i < data->height; i++)
			for (j = 0; j < data->width; j++)
				for (k = 0; k < 3; k++)
					fwrite(&data->image->color[i][j][k], 1, 1, f);
	}
	fclose(f);
	return 0;
}

void SAVE(DATA_t *data, char *filename, char *ascii)
{
	if (is_loaded(data) == 0)
		return;

	if (!ascii) {
		if (save_binary(data, filename) == 0)
			printf("Saved %s\n", filename);
	} else if (strcmp(ascii, "ascii") == 0) {
		if (save_ascii(data, filename) == 0)
			printf("Saved %s\n", filename);
	} else {
		printf("Invalid command\n");
	}
}

void EXIT(DATA_t **data)
{
	// Erase everything from memory
	if ((*data)->magic_word == 2) {
		int i;
		for (i = 0; i < (*data)->height; i++)
			free((*data)->image->grayscale[i]);
		free((*data)->image->grayscale);
	} else if ((*data)->magic_word == 3) {
		int i, j;
		for (i = 0; i < (*data)->height; i++) {
			for (j = 0; j < (*data)->width; j++)
				free((*data)->image->color[i][j]);
			free((*data)->image->color[i]);
		}
		free((*data)->image->color);
	}
	free((*data)->image);
	free(*data);
}
