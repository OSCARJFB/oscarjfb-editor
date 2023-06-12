/*
    Writen by: Oscar Bergström
    https://github.com/OSCARJFB

	MIT License
	Copyright (c) 2023 Oscar Bergström
*/

#include <stdio.h>
#include <stdlib.h>
#include "fileHandler.h"
#include "editorMode.h"

int main(int argc, char **argv)
{
	system("clear"); // replace with ncurses here. 
	long fileSize = 0;
	char *buffer = NULL;
	bufList *head = NULL;

	FILE *FP = getFileFromArg(argc, argv);
	if (FP == NULL)
	{
		exit(-1);
	}

	fileSize = getFileSize(FP);
	if (fileSize == -1)
	{
		exit(-1);
	}

	buffer = allocateBuffer(buffer, fileSize);
	if (buffer == NULL)
	{
		exit(-1);
	}

	loadBuffer(buffer, FP, fileSize);

	head = createNodesFromBuffer(buffer, head, fileSize);
	if (head == NULL)
	{
		exit(-1);
	}

	closeFile(FP);
	editTextFile(head, argv[1]);
	freeBuffer(buffer);

	return 0;
}
