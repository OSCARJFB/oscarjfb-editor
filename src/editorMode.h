/*
	Writen by: Oscar Bergström
	https://github.com/OSCARJFB

	MIT License
	Copyright (c) 2023 Oscar Bergström
*/

#ifndef EDITORMODE_H
#define EDITORMODE_H

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <stdbool.h>
#include <ncurses.h>

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

enum lineLimit
{
	LIM_1 = 10,
	LIM_2 = 100,
	LIM_3 = 1000,
	LIM_4 = 10000,
	LIM_5 = 100000,
};

enum marginSize
{
	MARGIN_SPACE_2 = 2,
	MARGIN_SPACE_3 = 3,
	MARGIN_SPACE_4 = 4,
	MARGIN_SPACE_5 = 5,
	MARGIN_SPACE_6 = 6,
};

enum mode
{
	EDIT = 0,
	SAVE = 1,
	COPY = 2,
	PASTE = 3,
	EXIT = 4
};

enum state
{
	ADD_FIRST_NODE = 0, 
	ADD_MIDDLE_NODE = 1,
	ADD_END_NODE = 2,
	DEL_NODE = 3,
	DEL_AT_END = 4
};

extern int _leftMargin;
extern int _rightMargin;
extern int _tabSize;
extern int _copySize;
extern int _currentLine;
extern int _viewStart;
extern int _viewEnd;

bufList *createNodesFromBuffer(char *buffer, long fileSize);
bufList *createNewNode(int ch);
coordinates onEditCoordinates(coordinates xy, int sFlag, int ch, bufList *last_node);
coordinates addNode(bufList **head, int ch, coordinates xy);
coordinates deleteNode(bufList **head, coordinates xy);
coordinates getEndNodeCoordinates(bufList *head);
dataCopied getCopyStart(dataCopied cp_data, coordinates xy);
dataCopied getCopyEnd(dataCopied cp_data, coordinates xy);
char *saveCopiedText(bufList *head, coordinates cp_start, coordinates cp_end);
void pasteCopiedlist(bufList **head, char *cpy_List, coordinates xy);
void save(bufList *head, int size, const char *fileName);
char *saveListToBuffer(bufList *head, int size);
char *newFileName(void);
void deleteAllNodes(bufList *head);
void updateCoordinatesInView(bufList **head);
int countNewLines(bufList *head);
void setLeftMargin(int newLines);
int printNodes(bufList *head);
int setMode(int ch);
coordinates moveArrowKeys(int ch, coordinates xy);
coordinates edit(bufList **head, coordinates xy, int ch);
dataCopied copy(dataCopied cpy_data, bufList *head, coordinates xy);
void editTextFile(bufList *head, const char *fileName);

#endif // EDITORMODE_H