#include <stdio.h>
#include <stdlib.h>
#include <ncurses.h>

/*
typedef struct ERROR
{

} ERROR;
*/

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
	while (fread(buffer, fileSize, 1, FP) > 0)
		;
}

typedef struct bufList
{
	int ch;
	struct bufList *next;
	struct bufList *prev;
} bufList;

void addNode(int ch, bufList **head)
{
	if ((*head) == NULL)
	{
		(*head) = malloc(sizeof(bufList));
		if ((*head) == NULL)
		{
			puts("addNode: malloc(), couldn't allocate memory for node");
		}

		(*head)->ch = ch;
		(*head)->next = NULL;
		(*head)->prev = NULL;

		return;
	}

	bufList *n_head = malloc(sizeof(bufList));
	if (n_head == NULL)
	{
		puts("addNode: malloc(), couldn't allocate memory for node");
	}

	n_head->ch = ch;
	n_head->next = NULL;
	n_head->prev = NULL;

	bufList *c_node = *head;
	bufList *temp = NULL;
	while (c_node->next != NULL)
	{
		temp = c_node;
		c_node = c_node->next;
	}

	c_node->next = n_head;
	n_head->prev = temp;
}

void printBufferList(bufList *head)
{
	while (head != NULL)
	{
		addch(head->ch);
		head = head->next;
	}
}

void freeBufferList(bufList *head)
{
	bufList *temp = NULL;
	while (head != NULL)
	{
		temp = head;
		head = head->next;
		free(temp);
	}

	temp = NULL;
	head = NULL;
}

void deleteNode(bufList *head)
{
	if(head == NULL)
	{
		return;
	}

	while(head != NULL)
	{
		if(head->next == NULL)
		{
			break; 
		}

		head = head->next; 
	}

	if(head->prev != NULL)
	{
		bufList *p_head = head->prev;  
		p_head->next = NULL; 
	}

	free(head); 
	head = NULL; 
}

bufList *createBufferList(char *buffer, bufList *head, long fileSize)
{
	for (int i = 0; i < fileSize; ++i)
	{
		addNode(buffer[i], &head);
	}

	return head;
}

void edit(bufList *head)
{
	int ch = 0;

	initscr();
	noecho();
	cbreak();
	curs_set(1);
	keypad(stdscr, 1);

	while (ch != 0x1b)
	{
		printBufferList(head);
		if ((ch = getch()) > 0x00)
		{
			switch (ch)
			{
			case KEY_UP:
				break;
			case KEY_DOWN:
				break;
			case KEY_LEFT:
				break;
			case KEY_RIGHT:
				break;
			case KEY_BACKSPACE:
				deleteNode(head); 
				break; 
			default:
				addNode(ch, &head);
			}

			clear();
			refresh();
		}

		endwin();
	}
}

int main(int argc, char **argv)
{
	system("clear");
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

	head = createBufferList(buffer, head, fileSize);
	if (head == NULL)
	{
		exit(-1);
	}

	edit(head);
	freeBuffer(buffer);
	closeFile(FP);

	return 0;
}
