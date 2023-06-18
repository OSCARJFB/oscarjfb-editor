/*
	Writen by: Oscar Bergström
	https://github.com/OSCARJFB

	MIT License
	Copyright (c) 2023 Oscar Bergström
*/

#include "tests.h"
#include "editorMode.h"
#include <stdlib.h>
#include <stdio.h>

void printAllNodesAndExit(bufList *head)
{
	endwin();

	puts("Forwards");

	for (; head != NULL; head = head->next)
	{
		if (head->ch != '\n')
		{
			printf("char: %c", head->ch);
		}
		else
		{
			printf("newline");
		}
		if (head->prev == NULL)
			printf(" prev is null");
		else
			printf(" prev is not null");

		if (head->next == NULL)
			printf(" next is null");
		else
			printf(" next is not null");

		printf("\n");

		if (head->next == NULL)
		{
			break;
		}
	}

	puts("Backwards");
	for (; head != NULL; head = head->prev)
	{
		if (head->ch != '\n')
		{
			printf("char: %c", head->ch);
		}
		else
		{
			printf("newline");
		}
		if (head->prev == NULL)
			printf(" prev is null");
		else
			printf(" prev is not null");

		if (head->next == NULL)
			printf(" next is null");
		else
			printf(" next is not null");

		printf("\n");
	}
}