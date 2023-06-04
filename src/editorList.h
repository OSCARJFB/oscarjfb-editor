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
	bufList *copiedList;
	coordinates cpy_start, cpy_end;
	bool copy_status;
} dataCopied;


bufList *createNodesFromBuffer(char *buffer, bufList *head, long fileSize);

void save(bufList *head, int size, const char *fileName);

char *saveListToBuffer(bufList *head, int size);

bufList *saveCopiedText(bufList *head, coordinates cp_start, coordinates cp_end);

void pasteCopiedText(bufList **head, bufList *copiedList, coordinates xy);

void deleteAllNodes(bufList *head);

bufList *createNewNode(int ch);

void addNode(bufList **head, int ch, coordinates xy);

void deleteNode(bufList **head, coordinates xy);

coordinates getEndNodeCoordinates(bufList *head);

int printNodes(bufList *head);

void updateCoordinates(bufList **head); 

void initCurseMode(void);

void endCurseMode(void);

dataCopied getCopyStart(dataCopied cp_data, coordinates xy);

dataCopied getCopyEnd(dataCopied cp_data, coordinates xy);

void editTextFile(bufList *head, const char *fileName);

#endif // EDITORLIST_H