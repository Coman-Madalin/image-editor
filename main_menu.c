#include "main_menu.h"

void PRELOAD(PLACEHOLDER **data, char *token)
{
	token = strtok(NULL, "\n ");
	if (!token) {
		printf("Invalid command\n");
		return;
	}

	// Free the memory allocated for the previous image
	if((*data)->magic_word != -1)
	{
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
		(*data)->height = 0;
		(*data)->width = 0;
		(*data)->scale = 0;
		(*data)->x1 = 0;
		(*data)->y1 = 0;
		(*data)->x2 = 0;
		(*data)->y2 = 0;
	}

	LOAD(data, token);
}

void PRESELECT(PLACEHOLDER **data, char *token)
{
	if (is_loaded(*data, 1) == 0)
		return;

	int i, is_all = 0;
	int x1, y1, x2, y2;
	char *is_word = NULL;
	for (i = 0; i < 4; i++) {
		token = strtok(NULL, "\n ");
		if (!token) {
			printf("Invalid command\n");
			return;
		}
		if (i == 0) {
			if (strncmp(token, "ALL", 3) == 0) {
				(*data)->x1 = 0;
				(*data)->y1 = 0;
				(*data)->x2 = (*data)->width;
				(*data)->y2 = (*data)->height;
				printf("Selected ALL\n");
				is_all = 1;
				break;
			} else
				x1 = strtol(token, &is_word, 10);
		} else if (i == 1)
			y1 = strtol(token, &is_word, 10);
		else if (i == 2)
			x2 = strtol(token, &is_word, 10);
		else
			y2 = strtol(token, &is_word, 10);
		if (is_word == token) {
			//free(is_word);
			printf("Invalid command\n");
			return;
		}
	}
	if (is_all == 0)
		SELECT(data, x1, y1, x2, y2);
}

void PREHISTOGRAM(PLACEHOLDER *data, char *token)
{
	int i;
	int bins, stars;
	if (is_loaded(data, 1) == 0)
		return;
	for (i = 0; i < 2; i++) {
		token = strtok(NULL, "\n ");
		if (!token) {
			printf("Invalid command\n");
			return;
		}
		if (i == 0)
			stars = strtol(token, NULL, 10);
		else
			bins = strtol(token, NULL, 10);
	}
	token = strtok(NULL, "\n ");
	if (token)
		printf("Invalid command\n");
	else
		HISTOGRAM(data, bins, stars);
}

void PREAPPLY(PLACEHOLDER **data, char *token)
{
	token = strtok(NULL, "\n ");
	APPLY(data, token);
}

void PREROTATE(PLACEHOLDER **data, char *token)
{
	if (is_loaded(*data, 1) == 0)
		return;

	int angle;
	token = strtok(NULL, "\n ");
	if (!token) {
		printf("Invalid command\n");
		return;
	}
	angle = strtol(token, NULL, 10);
	token = strtok(NULL, "\n ");
	if (token)
		printf("Invalid command\n");
	else
		ROTATE(data, angle);
}

void PRESAVE(PLACEHOLDER *data, char *token)
{
	token = strtok(NULL, "\n ");
	char *file_name = token;
	token = strtok(NULL, "\n ");
	SAVE(data, file_name, token);
}

void PREEXIT(PLACEHOLDER **data)
{
	if(is_loaded(*data, 1) == 0) {
		free(*data);
		return;
	}
	EXIT(data);
}
