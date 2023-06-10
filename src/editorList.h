/*
	Writen by: Oscar Bergström
	https://github.com/OSCARJFB

	MIT License
	Copyright (c) 2023 Oscar Bergström
*/

#include <stdbool.h>

#ifndef EDITORLIST_H
#define EDITORLIST_H

#define ESC_KEY 27
#define NO_KEY -1

typedef struct bufList
{
	int ch, x, y;
	struct bufList *next;
	struct bufList *prev;
} bufList;

typedef struct coordinates
{
	int x, y;
} coordinates;

typedef struct dataCopied
{
	bufList *cpy_List;
	coordinates cpy_start, cpy_end;
	bool isStart, isEnd;
} dataCopied;

extern int leftMargin;
extern int rightMargin;

enum lineLimit
{
	one_hundred = 100,
	one_thousand = 1000,
	ten_thousand = 10000,
	hundred_thousand = 100000,
};

enum marginSize
{
	three = 3,
	four = 4,
	five = 5,
	six = 6,
};

bufList *createNodesFromBuffer(char *buffer, bufList *head, long fileSize);

bufList *createNewNode(int ch);

coordinates addNode(bufList **head, int ch, coordinates xy);

coordinates deleteNode(bufList **head, coordinates xy);

coordinates getEndNodeCoordinates(bufList *head);

dataCopied getCopyStart(dataCopied cp_data, coordinates xy);

dataCopied getCopyEnd(dataCopied cp_data, coordinates xy);

bufList *saveCopiedText(bufList *head, coordinates cp_start, coordinates cp_end);

void pasteCopiedlist(bufList **head, bufList *cpy_List, coordinates xy);

void editTextFile(bufList *head, const char *fileName);

void save(bufList *head, int size, const char *fileName);

char *saveListToBuffer(bufList *head, int size);

void deleteAllNodes(bufList *head);

void updateCoordinates(bufList **head);

void setLeftMargin(bufList *head);

int printNodes(bufList *head);

#endif // EDITORLIST_H