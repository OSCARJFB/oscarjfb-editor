/*
	Writen by: Oscar Bergström
	https://github.com/OSCARJFB
*/

#ifndef TEXTLIST_H
#define TEXTLIST_H

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

void deleteAllNodes(bufList *head);

void updateXYNodesAdd(bufList **head);

void updateXYNodesDel(bufList **head);

bufList *createNewNode(coordinates xy, int ch);

coordinates addNode(bufList **head, int ch, coordinates xy);

coordinates deleteNode(bufList **head, coordinates xy);

coordinates getEndNodeCoordinates(bufList *head);

void printNodes(bufList *head);

void editTextFile(bufList *head);

void printValue(); 

// DEBUG functions, used to examine the linked list.

void DEBUG_PRINT_ALL_NODES_POINTER(bufList *head);

void DEBUG_PRINT_ALL_NODES_VALUE(bufList *head);

void DEBUG_PRINT_ALL_NODES_VALUES_AND_CURSOR_NO_EXIT(bufList *head, int x, int y);

#endif // TEXTLIST_H