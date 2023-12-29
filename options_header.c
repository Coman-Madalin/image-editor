#include "options_header.h"

void SELECT(PLACEHOLDER **image)
{
	if((*image)->magic_word == -1)
	{
		printf("No image loaded\n");
		return;
	}

	char *all = calloc(10, sizeof(char));
	scanf("%s", all);
	if(strncmp(all, "ALL", 3) == 0)
	{

	}
}
