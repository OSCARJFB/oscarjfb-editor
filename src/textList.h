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

void save(bufList *head, int size);

char *saveListToBuffer(bufList *head, int size);

void deleteAllNodes(bufList *head);

coordinates updateXYNodesAdd(bufList **head);

void updateXYNodesDel(bufList **head);

bufList *createNewNode(coordinates xy, int ch);

coordinates addNode(bufList **head, int ch, coordinates xy);

coordinates deleteNode(bufList **head, coordinates xy);

coordinates getEndNodeCoordinates(bufList *head);

void printNodes(bufList *head);

void editTextFile(bufList *head);

#endif // TEXTLIST_H