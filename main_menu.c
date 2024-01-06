#include "main_menu.h"

void PRELOAD(PLACEHOLDER_t **data, char *parameters)
{
	parameters = strtok(NULL, "\n ");
	if (!parameters) {
		printf("Invalid command\n");
		return;
	}

	// Free the memory allocated for the previous image
	if ((*data)->magic_word != -1) {
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
		(*data)->magic_word = -1;
		(*data)->height = (*data)->width = (*data)->scale = 0;
		(*data)->x1 = (*data)->y1 = (*data)->x2 = (*data)->y2 = 0;
	}

	LOAD(data, parameters);
}

void PRESELECT(PLACEHOLDER_t **data, char *parameters)
{
	if (is_loaded(*data) == 0)
		return;

	int i, is_all = 0;
	int x1, y1, x2, y2;
	char *is_word = NULL;
	for (i = 0; i < 4; i++) {
		parameters = strtok(NULL, "\n ");
		if (!parameters) {
			printf("Invalid command\n");
			return;
		}
		if (i == 0) {
			if (strncmp(parameters, "ALL", 3) != 0) {
				x1 = strtol(parameters, &is_word, 10);
			} else {
				(*data)->x1 = 0;
				(*data)->y1 = 0;
				(*data)->x2 = (*data)->width;
				(*data)->y2 = (*data)->height;
				printf("Selected ALL\n");
				is_all = 1;
				break;
			}
		} else if (i == 1) {
			y1 = strtol(parameters, &is_word, 10);
		} else if (i == 2) {
			x2 = strtol(parameters, &is_word, 10);
		} else {
			y2 = strtol(parameters, &is_word, 10);
		}
		if (is_word == parameters) {
			printf("Invalid command\n");
			return;
		}
	}
	if (is_all == 0)
		SELECT(data, x1, y1, x2, y2);
}

void PREHISTOGRAM(PLACEHOLDER_t *data, char *parameters)
{
	if (is_loaded(data) == 0)
		return;

	int i;
	int bins, stars;
	for (i = 0; i < 2; i++) {
		parameters = strtok(NULL, "\n ");
		if (!parameters) {
			printf("Invalid command\n");
			return;
		}

		if (i == 0)
			stars = strtol(parameters, NULL, 10);
		else
			bins = strtol(parameters, NULL, 10);
	}
	parameters = strtok(NULL, "\n ");
	if (parameters)
		printf("Invalid command\n");
	else
		HISTOGRAM(data, bins, stars);
}

void PREROTATE(PLACEHOLDER_t **data, char *parameters)
{
	if (is_loaded(*data) == 0)
		return;

	int angle;
	parameters = strtok(NULL, "\n ");
	if (!parameters) {
		printf("Invalid command\n");
		return;
	}
	angle = strtol(parameters, NULL, 10);
	parameters = strtok(NULL, "\n ");
	if (parameters)
		printf("Invalid command\n");
	else
		ROTATE(data, angle);
}

void PREAPPLY(PLACEHOLDER_t **data, char *parameters)
{
	parameters = strtok(NULL, "\n ");
	APPLY(data, parameters);
}

void PRESAVE(PLACEHOLDER_t *data, char *parameters)
{
	parameters = strtok(NULL, "\n ");
	char *file_name = parameters;
	parameters = strtok(NULL, "\n ");
	SAVE(data, file_name, parameters);
}

void PREEXIT(PLACEHOLDER_t **data)
{
	if (is_loaded(*data) == 0) {
		free(*data);
		return;
	}
	EXIT(data);
}
