#include "options_header.h"

void SELECT(PLACEHOLDER **data, int x1, int y1, int x2, int y2)
{
	if (is_loaded(*data, 1) == 0)
		return;

	if (x1 < 0 || x1 > (*data)->width) {
		printf("Invalid set of coordinates\n");
		return;
	}
	if (y1 < 0 || y1 > (*data)->height) {
		printf("Invalid set of coordinates\n");
		return;
	}
	if (x2 < 0 || x2 > (*data)->width) {
		printf("Invalid set of coordinates\n");
		return;
	}
	if (y2 < 0 || y2 > (*data)->height) {
		printf("Invalid set of coordinates\n");
		return;
	}

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

	(*data)->x1 = x1;
	(*data)->y1 = y1;
	(*data)->x2 = x2;
	(*data)->y2 = y2;

	printf("Selected %d %d %d %d\n", x1, y1, x2, y2);
}

void HISTOGRAM(PLACEHOLDER *data, int bins, int max_stars)
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

void EQUALIZE(PLACEHOLDER **data)
{
	if (is_loaded((*data), 1) == 0)
		return;

	if ((*data)->magic_word == 3) {
		printf("Black and white image needed\n");
		return;
	}

	int vf[256][3] = {0};
	int i, j;

	for (i = 0; i < (*data)->height; i++)
		for (j = 0; j < (*data)->width; j++) {
			int value = (*data)->image->grayscale[i][j];
			vf[value][0]++;
		}

	int current_sum = 0;
	int first_value;
	for (i = 0; i <= (*data)->scale; i++)
		if (vf[i]) {
			if (current_sum == 0)
				first_value = vf[i][0];
			current_sum += vf[i][0];
			vf[i][1] = current_sum;
		}

	for (i = 0; i <= (*data)->scale; i++)
		if (vf[i])
			vf[i][2] = round((double)(vf[i][1] - first_value) /
							 (((*data)->width * (*data)->height) -
							  first_value) *
							 (*data)->scale);

	for (i = 0; i < (*data)->height; i++)
		for (j = 0; j < (*data)->width; j++) {
			int value = (*data)->image->grayscale[i][j];
			(*data)->image->grayscale[i][j] = vf[value][2];
		}

	printf("Equalize done\n");
}

void ROTATE_ALL(PLACEHOLDER **data, int angle)
{
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
	default:
		printf(" Unsupported rotation angle\n");
		return;
	}

	ACTUAL_IMAGE *new_image = calloc(1, sizeof(ACTUAL_IMAGE));
	int i, j;
	if ((*data)->magic_word == 2) {
		new_image->grayscale = calloc(new_height, sizeof(int *));
		for (i = 0; i < new_height; i++) {
			new_image->grayscale[i] = calloc(new_width, sizeof(int));
			for (j = 0; j < new_width; j++) {
				if (angle == 90)
					new_image->grayscale[i][j] = (*data)->image->grayscale[
							new_width - j - 1][i];
				else if (angle == 180)
					new_image->grayscale[i][j] = (*data)->image->grayscale[
							new_height - i - 1][new_width - j - 1];
				else
					new_image->grayscale[i][j] = (*data)->image->grayscale[j][
							new_height - i - 1];
			}
		}

		// Free old image
		for (i = 0; i < (*data)->height; i++)
			free((*data)->image->grayscale[i]);
		free((*data)->image->grayscale);
		free((*data)->image);
		(*data)->image = new_image;
		(*data)->width = new_width;
		(*data)->height = new_height;
		(*data)->x1 = 0;
		(*data)->x2 = new_width;
		(*data)->y1 = 0;
		(*data)->y2 = new_height;
		return;
	}
	new_image->color = calloc(new_height, sizeof(int **));
	for (i = 0; i < new_height; i++) {
		new_image->color[i] = calloc(new_width, sizeof(int *));
		for (j = 0; j < new_width; j++) {
			new_image->color[i][j] = calloc(3, sizeof(int));
			if (angle == 90) {
				new_image->color[i][j][0] = (*data)->image->color[new_width -
																  j - 1][i][0];
				new_image->color[i][j][1] = (*data)->image->color[new_width -
																  j - 1][i][1];
				new_image->color[i][j][2] = (*data)->image->color[new_width -
																  j - 1][i][2];
			} else if (angle == 180) {
				new_image->color[i][j][0] = (*data)->image->color[new_height -
																  i - 1][
						new_width - j - 1][0];
				new_image->color[i][j][1] = (*data)->image->color[new_height -
																  i - 1][
						new_width - j - 1][1];
				new_image->color[i][j][2] = (*data)->image->color[new_height -
																  i - 1][
						new_width - j - 1][2];
			} else {
				new_image->color[i][j][0] = (*data)->image->color[j][
						new_height - i - 1][0];
				new_image->color[i][j][1] = (*data)->image->color[j][
						new_height - i - 1][1];
				new_image->color[i][j][2] = (*data)->image->color[j][
						new_height - i - 1][2];

			}
		}
	}

	// Free old image
	for (i = 0; i < (*data)->height; i++) {
		for (j = 0; j < (*data)->width; j++)
			free((*data)->image->color[i][j]);
		free((*data)->image->color[i]);
	}
	free((*data)->image->color);
	free((*data)->image);
	(*data)->image = new_image;
	(*data)->width = new_width;
	(*data)->height = new_height;
	(*data)->x1 = 0;
	(*data)->x2 = new_width;
	(*data)->y1 = 0;
	(*data)->y2 = new_height;
}

void ROTATE(PLACEHOLDER **data, int angle) // in-place
{
	if (is_loaded(*data, 1) == 0)
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
	if (square == 2) {
		ROTATE_ALL(data, positive_angle);
		printf("Rotated %d\n", angle);
		return;
	}

	ACTUAL_IMAGE *new_image = calloc(1, sizeof(ACTUAL_IMAGE));
	int i, j;

	if ((*data)->magic_word == 2) {
		new_image->grayscale = calloc((*data)->height, sizeof(int *));
		for (i = 0; i < (*data)->height; i++) {
			new_image->grayscale[i] = calloc((*data)->width, sizeof(int));
			for (j = 0; j < (*data)->width; j++)
				new_image->grayscale[i][j] = (*data)->image->grayscale[i][j];
		}

		for (i = (*data)->y1; i < (*data)->y2; i++)
			for (j = (*data)->x1; j < (*data)->x2; j++) {
				int old_i, old_j;
				if (positive_angle == 90) {
					old_i = (*data)->y2 - 1 - (j - (*data)->x1);
					old_j = (*data)->x1 + (i - (*data)->y1);
				} else if (positive_angle == 180) {
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
		free((*data)->image);
		(*data)->image = new_image;
		printf("Rotated %d\n", angle);
		return;
	}
	int k;
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
			int old_i, old_j;
			if (positive_angle == 90) {
				old_i = (*data)->y2 - 1 - (j - (*data)->x1);
				old_j = (*data)->x1 + (i - (*data)->y1);
			} else if (positive_angle == 180) {
				old_i = (*data)->y2 - 1 - (i - (*data)->y1);
				old_j = (*data)->x2 - 1 - (j - (*data)->x1);
			} else {
				old_i = (*data)->y1 + (j - (*data)->x1);
				old_j = (*data)->x2 - 1 - (i - (*data)->y1);
			}
			new_image->color[i][j][0] = (*data)->image->color[old_i][old_j][0];
			new_image->color[i][j][1] = (*data)->image->color[old_i][old_j][1];
			new_image->color[i][j][2] = (*data)->image->color[old_i][old_j][2];
		}

	// Free old image
	for (i = 0; i < (*data)->height; i++) {
		for (j = 0; j < (*data)->width; j++)
			free((*data)->image->color[i][j]);
		free((*data)->image->color[i]);
	}
	free((*data)->image->color);
	free((*data)->image);
	(*data)->image = new_image;
	printf("Rotated %d\n", angle);
}

void CROP(PLACEHOLDER **data)
{
	if (is_loaded(*data, 1) == 0)
		return;

	if ((*data)->x1 == 0 && (*data)->x2 == (*data)->width &&
		(*data)->y1 == 0 && (*data)->y2 == (*data)->height) {
		printf("Image cropped\n");
		return;
	}

	ACTUAL_IMAGE *new_image = calloc(1, sizeof(ACTUAL_IMAGE));

	int i, j;
	if ((*data)->magic_word == 2) {
		new_image->grayscale = calloc((*data)->y2 - (*data)->y1,
									  sizeof(int *));

		for (i = (*data)->y1; i < (*data)->y2; i++) {
			int new_width = (*data)->x2 - (*data)->x1;
			new_image->grayscale[i - (*data)->y1] = calloc(
					new_width,
					sizeof(int));
			if (!new_image->grayscale[i - (*data)->y1]) {
				printf("Error allocating memory\n");
				return;
			}

			for (j = (*data)->x1; j < (*data)->x2; j++) {
				new_image->grayscale[i - (*data)->y1][j -
													  (*data)->x1] =
						(*data)->image->grayscale[i][j];
			}
		}
	} else {
		new_image->color = calloc((*data)->y2 - (*data)->y1,
								  sizeof(int **));
		for (i = (*data)->y1; i < (*data)->y2; i++) {
			int new_width = (*data)->x2 - (*data)->x1;
			new_image->color[i - (*data)->y1] = calloc(
					new_width,
					sizeof(int *));
			for (j = (*data)->x1; j < (*data)->x2; j++) {
				new_image->color[i - (*data)->y1][j -
												  (*data)->x1] = calloc(
						3,
						sizeof(int));
				new_image->color[i - (*data)->y1][j -
												  (*data)->x1][0] = (*data)->image->color[i][j][0];
				new_image->color[i - (*data)->y1][j -
												  (*data)->x1][1] = (*data)->image->color[i][j][1];
				new_image->color[i - (*data)->y1][j -
												  (*data)->x1][2] = (*data)->image->color[i][j][2];
			}
		}
	}

	// Free old image
	if ((*data)->magic_word == 2) {
		for (i = 0; i < (*data)->height; i++)
			free((*data)->image->grayscale[i]);
		free((*data)->image->grayscale);
	} else {
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
	(*data)->x1 = 0;
	(*data)->x2 = (*data)->width;
	(*data)->y1 = 0;
	(*data)->y2 = (*data)->height;

	printf("Image cropped\n");
}

void APPLY(PLACEHOLDER **data, char *parameter)
{
	if (!parameter) {
		if (is_loaded(*data, 1) == 0)
			return;
		else {
			printf("Invalid command\n");
			return;
		}
	}
	if (is_loaded(*data, 1) == 0)
		return;
	if (is_chaplin(*data) == 1)
		return;

	int kernel[3][3];
	if (strcmp(parameter, "EDGE") == 0) {
		EDGE(kernel);
		APPLY_UTIL(1, kernel, data);
		printf("APPLY EDGE done\n");

	} else if (strcmp(parameter, "SHARPEN") == 0) {
		SHARPEN(kernel);
		APPLY_UTIL(1, kernel, data);
		printf("APPLY SHARPEN done\n");

	} else if (strcmp(parameter, "BLUR") == 0) {
		int coef = 9;
		BLUR(kernel);
		APPLY_UTIL(coef, kernel, data);
		printf("APPLY BLUR done\n");

	} else if (strcmp(parameter, "GAUSSIAN_BLUR") == 0) {
		int coef = 16;
		GAUSSIAN_BLUR(kernel);
		APPLY_UTIL(coef, kernel, data);
		printf("APPLY GAUSSIAN_BLUR done\n");

	} else {
		printf("APPLY parameter invalid\n");
		return;
	}
}

int save_ascii(PLACEHOLDER *data, char *filename)
{
	FILE *f = fopen(filename, "w");
	if (!f) {
		printf("Error opening file\n");
		return 1;
	}

	fprintf(f, "P%d\n", data->magic_word);
	fprintf(f, "%d %d\n", data->width, data->height);
	fprintf(f, "%d\n", data->scale);

	int i, j;
	if (data->magic_word == 2) {
		for (i = 0; i < data->height; i++) {
			for (j = 0; j < data->width; j++)
				fprintf(f, "%d ", data->image->grayscale[i][j]);
			fprintf(f, "\n");
		}
	} else if (data->magic_word == 3) {
		for (i = 0; i < data->height; i++) {
			for (j = 0; j < data->width; j++)
				fprintf(f, "%d %d %d ",
						data->image->color[i][j][0],
						data->image->color[i][j][1],
						data->image->color[i][j][2]);
			fprintf(f, "\n");
		}
	}
	fclose(f);
	return 0;
}

int save_binary(PLACEHOLDER *data, char *filename)
{
	FILE *f = fopen(filename, "wb");
	if (!f) {
		printf("Error opening file\n");
		return 1;
	}

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
					fwrite(&data->image->color[i][j][k], 1, 1,
						   f);
	}
	fclose(f);
	return 0;
}

void SAVE(PLACEHOLDER *data, char *filename, char *ascii)
{
	if (is_loaded(data, 1) == 0)
		return;

	if (!ascii) {
		if (save_binary(data, filename) == 0)
			printf("Saved %s\n", filename);
		return;
	} else if (strcmp(ascii, "ascii") == 0) {
		if (save_ascii(data, filename) == 0)
			printf("Saved %s\n", filename);
		return;
	} else {
		printf("Invalid command\n");
		return;
	}
}

void EXIT(PLACEHOLDER **data)
{
	// Erase everything that remains
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
