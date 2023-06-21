/*
	Writen by: Oscar Bergström
	https://github.com/OSCARJFB

	MIT License
	Copyright (c) 2023 Oscar Bergström
*/

#ifndef EDITORMODE_H
#define EDITORMODE_H

#include <stdbool.h>

#define ESC_KEY 27
#define NO_KEY -1
#define CPY_BUFFER_SIZE 1000

typedef struct coordinates
{
	int x, y;
} coordinates;

typedef struct bufList
{
	int ch, x, y;
	struct bufList *next;
	struct bufList *prev;
} bufList;

typedef struct dataCopied
{
	char *cpy_List;
	coordinates cpy_start, cpy_end;
	bool isStart, isEnd;
} dataCopied;

extern int _leftMargin;
extern int _rightMargin;
extern int _tabSize;
extern int _copySize; 

enum lineLimit
{
	ten = 10,
	one_hundred = 100,
	one_thousand = 1000,
	ten_thousand = 10000,
	hundred_thousand = 100000,
};

enum marginSize
{
	two = 2,
	three = 3,
	four = 4,
	five = 5,
	six = 6,
};

enum mode
{
	EDIT = 0,
	SAVE = 1,
	COPY = 2,
	PASTE = 3,
	EXIT = 4
};

bufList *createNodesFromBuffer(char *buffer, long fileSize);
bufList *createNewNode(int ch);
coordinates addNode(bufList **head, int ch, coordinates xy);
coordinates deleteNode(bufList **head, coordinates xy);
coordinates getEndNodeCoordinates(bufList *head);
dataCopied getCopyStart(dataCopied cp_data, coordinates xy);
dataCopied getCopyEnd(dataCopied cp_data, coordinates xy);
char *saveCopiedText(bufList *head, coordinates cp_start, coordinates cp_end);
void pasteCopiedlist(bufList **head, char *cpy_List, coordinates xy);
void save(bufList *head, int size, const char *fileName);
char *saveListToBuffer(bufList *head, int size);
void deleteAllNodes(bufList *head);
void updateCoordinates(bufList **head);
void setLeftMargin(bufList *head);
int printNodes(bufList *head);
int setMode(int ch);
coordinates moveArrowKeys(int ch, coordinates xy);
coordinates edit(bufList **head, coordinates xy, int ch);
dataCopied copy(dataCopied cpy_data, bufList *head, coordinates xy);
void editTextFile(bufList *head, const char *fileName);
void TEST_LIST_LINKING(bufList *head);

#endif // EDITORMODE_H