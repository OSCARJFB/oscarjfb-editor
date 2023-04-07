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
	fclose(FP);
	FP = NULL;
}

int getFileSize(FILE *FP)
{
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

/** 
 *	If the buffer pointer is NULL, allocateBuffer  will set an initial size to the buffer.
 * 	Else it will increament the file size with a 1000 bytes and reallocate the buffer into the sum of the new size. 
 */ 
int allocateBuffer(char **buffer, int fileSize)
{
	const int BUFF_INC = 1000;

	if (buffer == NULL)
	{
		(*buffer) = malloc(fileSize += BUFF_INC);
		if ((*buffer) == NULL)
		{
			puts("allocateBuffer: malloc() error");
			return -1;
		}
		return fileSize;
	}
	
	(*buffer) = realloc((*buffer), (fileSize += BUFF_INC)); 
	if ((*buffer) == NULL)
	{
		puts("allocateBuffer: malloc() error");
		return -1;
	}

	return fileSize; 
}

void freeBuffer(char *buffer)
{
	if (buffer == NULL)
	{
		puts("Buffer was NULL, no deallocation was needed");
		return;
	}

	free(buffer);
	buffer = NULL;
}

void loadBuffer(char *buffer, FILE *FP, long fileSize)
{
	rewind(FP); 
	while(fread(buffer, fileSize, 1, FP) > 0);
}

void saveBufferToFile(const char *fileName)
{

}

int main(int argc, char **argv)
{
	long fileSize = 0;
	char *buffer = NULL;
	
	FILE *FP = getFileFromArg(argc, argv);
	if (FP != NULL)
	{
		fileSize = getFileSize(FP);
		fileSize += allocateBuffer(&buffer, fileSize); 
		loadBuffer(buffer, FP, fileSize);
		closeFile(FP);
	}	
	printf("%s", buffer); 
	freeBuffer(buffer);

	return 0;
}
