// Image Editor
// Coman Andrei-Madalin
// 2023-2024

#include "apply_utils.h"

void clamp(double *value, int min, int max)
{
	if (*value < min)
		*value = min;
	if (*value > max)
		*value = max;
}

void EDGE(int kernel[3][3], int *coefficient)
{
	*coefficient = 1;
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

void SHARPEN(int kernel[3][3], int *coefficient)
{
	*coefficient = 1;
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

void BLUR(int kernel[3][3], int *coefficient)
{
	*coefficient = 9;
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

void GAUSSIAN_BLUR(int kernel[3][3], int *coefficient)
{
	*coefficient = 16;
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

void calculate_sum(double *sum, int which, int i, int j, int coefficient,
				   int kernel[3][3], unsigned short int ***color)
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
	*sum /= coefficient;
	clamp(sum, 0, 255);
}

void APPLY_UTIL(int coefficient, int kernel[3][3], DATA_t **data)
{
	// I created a new image because if we modify the old one in place, we would
	// use the modified pixels in the calculations for the next pixels, which
	// would lead to incorrect results.
	ACTUAL_IMAGE_t *new_image = calloc(1, sizeof(ACTUAL_IMAGE_t));
	// Another solution to that problem would be to use a temporary image the
	// size of the selected area, apply the filter over it, and then copy the
	// values back. This would be more efficient, but I decided to go with
	// the first solution because it is more readable.
	int i, j, k;
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
			for (k = 0; k < 3; k++) {
				double sum = 0;
				calculate_sum(&sum, k, i, j,
							  coefficient, kernel, (*data)->image->color);
				new_image->color[i][j][k] = (int)sum;
			}
		}
	}

	// Free the old image
	for (i = 0; i < (*data)->height; i++) {
		for (j = 0; j < (*data)->width; j++)
			free((*data)->image->color[i][j]);
		free((*data)->image->color[i]);
	}
	free((*data)->image->color);
	free((*data)->image);
	(*data)->image = new_image;
}
