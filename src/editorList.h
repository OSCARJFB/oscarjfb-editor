/*
	Writen by: Oscar Bergström
	https://github.com/OSCARJFB

	MIT License
	Copyright (c) 2023 Oscar Bergström
*/

#ifndef EDITORLIST_H
#define EDITORLIST_H

#define ESC_KEY 0x1b
#define NULL_KEY 0x00

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

bufList *createNodesFromBuffer(char *buffer, bufList *head, long fileSize);

void save(bufList *head, int size, const char *fileName);

char *saveListToBuffer(bufList *head, int size);

bufList *saveCopiedText(bufList *head, coordinates cp_start, coordinates cp_end);

void pasteCopiedText(bufList **head, bufList *copiedList, coordinates xy);

void deleteAllNodes(bufList *head);

coordinates updateXYNodesAdd(bufList **head);

void updateXYNodesDel(bufList **head);

bufList *createNewNode(coordinates xy, int ch);

coordinates addNode(bufList **head, int ch, coordinates xy);

coordinates deleteNode(bufList **head, coordinates xy);

coordinates getEndNodeCoordinates(bufList *head);

int printNodes(bufList *head);

void editTextFile(bufList *head, const char *fileName);

#endif // EDITORLIST_H