// Image Editor
// Coman Andrei-Madalin
// 2023-2024

#include "structure.h"
#include "main_menu.h"
#include "commands.h"

int main(void)
{
	char command[50];
	DATA_t *data = calloc(1, sizeof(DATA_t));
	data->magic_word = -1;

	while (1 != 0) {
		fgets(command, 50, stdin);
		char *token = strtok(command, "\n ");

		if (!token)
			continue;
		if (strcmp(token, "LOAD") == 0) {
			PRELOAD(&data, token);
		} else if (strcmp(token, "SELECT") == 0) {
			PRESELECT(&data, token);
		} else if (strcmp(token, "HISTOGRAM") == 0) {
			PREHISTOGRAM(data, token);
		} else if (strcmp(token, "EQUALIZE") == 0) {
			EQUALIZE(&data);
		} else if (strcmp(token, "ROTATE") == 0) {
			PREROTATE(&data, token);
		} else if (strcmp(token, "CROP") == 0) {
			CROP(&data);
		} else if (strcmp(token, "APPLY") == 0) {
			PREAPPLY(&data, token);
		} else if (strcmp(token, "SAVE") == 0) {
			PRESAVE(data, token);
		} else if (strcmp(command, "EXIT") == 0) {
			PREEXIT(&data);
			exit(0);
		} else {
			printf("Invalid command\n");
		}
	}
}
