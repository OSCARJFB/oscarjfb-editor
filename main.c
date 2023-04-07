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

char *allocateBuffer(char *buffer, int fileSize)
{
	buffer = malloc(fileSize);
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

typedef struct bufList
{
	char character; 
	int isEdit; 
	struct bufList *next; 
	struct bufList *prev;  
} bufList; 

void addBufferToList(char character, bufList **head)
{
	if((*head) == NULL)
	{
		(*head) = malloc(sizeof(bufList));
		if((*head) == NULL)
		{
			puts("addBufferToList: malloc(), couldn't allocate memory for node");
		} 

		(*head)->character = character; 
		(*head)->isEdit = 0; 
		(*head)->next = NULL; 
		(*head)->prev = NULL; 	

		return; 
	}

	bufList *n_head = malloc(sizeof(bufList));
	if((n_head) == NULL)
	{
		puts("addBufferToList: malloc(), couldn't allocate memory for node");
	} 

	n_head->character = character; 
	n_head->isEdit = 0; 
	n_head->next = NULL; 
	n_head->prev = NULL; 	

	while((*head) != NULL)
	{
		(*head) = (*head)->next; 
	}

	(*head) = n_head; 
}

void freeBufferList(bufList *head)
{
	bufList *temp = NULL; 
	while(head != NULL)
	{
		temp = head;
		head = head->next;
		free(temp); 
	}

	temp = NULL; 
	head = NULL; 
}	

int main(int argc, char **argv)
{
	long fileSize = 0;
	char *buffer = NULL;
	
	FILE *FP = getFileFromArg(argc, argv);
	if (FP != NULL)
	{
		fileSize = getFileSize(FP);
		buffer = allocateBuffer(buffer, fileSize); 
		loadBuffer(buffer, FP, fileSize);
		closeFile(FP);
	}	
	
	// TEST
	bufList *head; 
	for(int i = 0; i < fileSize; ++i)
	{
		addBufferToList(buffer[i], &head); 
	}	

	for(int i = 0; i < fileSize; ++i)
	{
		addBufferToList(buffer[i], &head); 
	}	

	while(head != NULL)
	{
		printf("%c", head->character); 
		head = head->next; 
	}

	printf("%s", buffer); 
	freeBuffer(buffer);

	return 0;
}
