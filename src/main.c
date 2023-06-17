/*
	Writen by: Oscar Bergström
	https://github.com/OSCARJFB

	MIT License
	Copyright (c) 2023 Oscar Bergström
*/

#include <stdio.h>
#include <stdlib.h>
#include "curseSettings.h"
#include "fileHandler.h"
#include "editorMode.h"

int main(int argc, char **argv)
{
	curseMode();

	FILE *FP = getFileFromArg(argc, argv);
	long fileSize = getFileSize(FP);
	char *buffer = allocateBuffer(fileSize);
	
	loadBuffer(buffer, FP, fileSize);
	bufList *head = createNodesFromBuffer(buffer, fileSize);
	freeBuffer(buffer);

	closeFile(FP);
	editTextFile(head, argv[1]);
	
	curseMode();

	return 0;
}
