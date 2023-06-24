/*
	Writen by: Oscar Bergström
	https://github.com/OSCARJFB

	MIT License
	Copyright (c) 2023 Oscar Bergström
*/

#include <stdio.h>
#include <stdlib.h>

FILE *getFileFromArg(int argc, char **argv)
{
	if (argc < 2)
	{
		return NULL;
	}

	FILE *FP = fopen(argv[1], "r");
	if (FP == NULL)
	{
		printf("getFileFromArg: Couldn't read File at path %s\n", argv[1]);
		return NULL;
	}

	return FP;
}

FILE *getFile(const char *path)
{
	FILE *FP = fopen(path, "a");
	if (FP == NULL)
	{
		printf("getFile: Couldn't read File at path %s\n", path);
		return NULL;
	}

	return FP;
}

void closeFile(FILE *FP)
{
	if(FP == NULL)
	{
		return; 
	}
	
	fclose(FP);
	FP = NULL;
}

int getFileSize(FILE *FP)
{
	if(FP == NULL)
	{
		return 0; 
	}
	
	long bufferSize = 0;
	if (fseek(FP, 0, SEEK_END) == -1)
	{
		puts("getFileSize: Couldn't get file size");
		return -1;
	}

	bufferSize = ftell(FP);
	if (bufferSize == -1)
	{
		puts("getFileSize: Couldn't get file size");
		return -1;
	}

	return bufferSize;
}

char *allocateBuffer(int fileSize)
{
	if(fileSize == 0)
	{
		return NULL; 
	}

	char *buffer = malloc(fileSize);
	if (buffer == NULL)
	{
		puts("allocateBuffer: malloc() error");
		return NULL;
	}

	return buffer;
}

void freeBuffer(char *buffer)
{
	if (buffer == NULL)
	{
		return;
	}

	free(buffer);
	buffer = NULL;
}

void loadBuffer(char *buffer, FILE *FP, long fileSize)
{
	if(buffer == NULL)
	{
		return;
	}
	
	rewind(FP);
	while (fread(buffer, fileSize, 1, FP) > 0)
	{
	};
}