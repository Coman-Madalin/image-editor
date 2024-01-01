#include "options_utils.h"

int is_loaded(PLACEHOLDER *data, int option)
{
	if (data->magic_word == -1) {
		if(option == 1)
			printf("No image loaded\n");
		return 0;
	}
	return 1;
}

void clamp(int *value, int min, int max)
{
	if (*value < min)
		*value = min;
	if (*value > max)
		*value = max;
}

void clamp_double(double *value, int min, int max)
{
	if (*value < min)
		*value = min;
	if (*value > max)
		*value = max;
}

int is_Chaplin(PLACEHOLDER *data)
{
	if (data->magic_word == 2) {
		printf("Easy, Charlie Chaplin\n");
		return 1;
	}
	return 0;
}

void EDGE(int kernel[3][3])
{
	kernel[0][0] = -1;
	kernel[0][1] = -1;
	kernel[0][2] = -1;
	kernel[1][0] = -1;
	kernel[1][1] = 8;
	kernel[1][2] = -1;
	kernel[2][0] = -1;
	kernel[2][1] = -1;
	kernel[2][2] = -1;
}

void SHARPEN(int kernel[3][3])
{
	kernel[0][0] = 0;
	kernel[0][1] = -1;
	kernel[0][2] = 0;
	kernel[1][0] = -1;
	kernel[1][1] = 5;
	kernel[1][2] = -1;
	kernel[2][0] = 0;
	kernel[2][1] = -1;
	kernel[2][2] = 0;
}

void BLUR(int kernel[3][3])
{
	kernel[0][0] = 1;
	kernel[0][1] = 1;
	kernel[0][2] = 1;
	kernel[1][0] = 1;
	kernel[1][1] = 1;
	kernel[1][2] = 1;
	kernel[2][0] = 1;
	kernel[2][1] = 1;
	kernel[2][2] = 1;
}

void GAUSSIAN_BLUR(int kernel[3][3])
{
	kernel[0][0] = 1;
	kernel[0][1] = 2;
	kernel[0][2] = 1;
	kernel[1][0] = 2;
	kernel[1][1] = 4;
	kernel[1][2] = 2;
	kernel[2][0] = 1;
	kernel[2][1] = 2;
	kernel[2][2] = 1;
}

void calculate_sum(double *sum, int which, int i, int j, int coef,
				   int kernel[3][3], int ***color)
{
	*sum += (color[i - 1][j - 1][which] * kernel[0][0]);
	*sum += (color[i - 1][j][which] * kernel[0][1]);
	*sum += (color[i - 1][j + 1][which] * kernel[0][2]);
	*sum += (color[i][j - 1][which] * kernel[1][0]);
	*sum += (color[i][j][which] * kernel[1][1]);
	*sum += (color[i][j + 1][which] * kernel[1][2]);
	*sum += (color[i + 1][j - 1][which] * kernel[2][0]);
	*sum += (color[i + 1][j][which] * kernel[2][1]);
	*sum += (color[i + 1][j + 1][which] * kernel[2][2]);
	*sum /= coef;
	clamp_double(sum, 0, 255);
}

void APPLY_UTIL(int coef, int kernel[3][3], PLACEHOLDER **data)
{
	int i, j, k;
	ACTUAL_IMAGE *new_image = calloc(1, sizeof(ACTUAL_IMAGE));
	new_image->color = calloc((*data)->height, sizeof(int **));
	for (i = 0; i < (*data)->height; i++) {
		new_image->color[i] = calloc((*data)->width, sizeof(int *));
		for (j = 0; j < (*data)->width; j++) {
			new_image->color[i][j] = calloc(3, sizeof(int));
			for (k = 0; k < 3; k++)
				new_image->color[i][j][k] = (*data)->image->color[i][j][k];
		}
	}

	for (i = (*data)->y1; i < (*data)->y2; i++) {
		if (i == 0 || i == (*data)->height - 1)
			continue;
		for (j = (*data)->x1; j < (*data)->x2; j++) {
			if (j == 0 || j == (*data)->width - 1)
				continue;
			double sum0 = 0, sum1 = 0, sum2 = 0;
			calculate_sum(&sum0, 0, i, j, coef, kernel, (*data)->image->color);
			calculate_sum(&sum1, 1, i, j, coef, kernel, (*data)->image->color);
			calculate_sum(&sum2, 2, i, j, coef, kernel, (*data)->image->color);

			new_image->color[i][j][0] = (int) sum0;
			new_image->color[i][j][1] = (int) sum1;
			new_image->color[i][j][2] = (int) sum2;
		}
	}

	// Free the old image
	for (i = 0; i < (*data)->height; i++) {
		for (j = 0; j < (*data)->width; j++) {
			free((*data)->image->color[i][j]);
		}
		free((*data)->image->color[i]);
	}
	free((*data)->image->color);
	free((*data)->image);
	(*data)->image = new_image;
}
