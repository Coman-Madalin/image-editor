#include "main_menu.h"

void PRELOAD(PLACEHOLDER **data, char *token)
{
	token = strtok(NULL, " \n");
	LOAD(data, token);
}

void PRESELECT(PLACEHOLDER **data, char *token)
{
	int i;
	int x1, y1, x2, y2;
	int is_all = 0;
	if (is_loaded(*data, 1) == 0)
		return;
	for (i = 0; i < 4; i++) {
		token = strtok(NULL, " \n");
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
				x1 = strtol(token, NULL, 10);
		} else if (i == 1)
			y1 = strtol(token, NULL, 10);
		else if (i == 2)
			x2 = strtol(token, NULL, 10);
		else
			y2 = strtol(token, NULL, 10);
	}
	if (is_all == 0)
		SELECT(data, x1, y1, x2, y2);
}

void PREHISTOGRAM(PLACEHOLDER *data, char *token)
{
	int i;
	int bins, stars;
	for (i = 0; i < 2; i++) {
		token = strtok(NULL, " \n");
		if (i == 0)
			bins = strtol(token, NULL, 10);
		else
			stars = strtol(token, NULL, 10);
	}
	HISTOGRAM(data, bins, stars);
}

void PREAPPLY(PLACEHOLDER **data, char *token)
{
	token = strtok(NULL, " \n");
	APPLY(data, token);
}

void PRESAVE(PLACEHOLDER *data, char *token)
{
	token = strtok(NULL, " \n");
	char *file_name = token;
	token = strtok(NULL, " \n");
	SAVE(data, file_name, token);
}

int PREEXIT(PLACEHOLDER **data)
{
	is_loaded(*data, 1);
	//EXIT();
	return 1;
}