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

bufList *createNodesFromBuffer(char *buffer, bufList *head, long fileSize);

void deleteAllNodes(bufList *head);

void updateXYNodesAdd(bufList **head, int *x, int *y);

void updateXYNodesDel(bufList **head, int *x, int *y);

void addNode(bufList **head, int ch, 
			 int x, int y);

void deleteNode(bufList **head, int *x, int *y);

void printNodes(bufList *head);

void getEndNodeCoordinates(bufList *head, int *x, int *y);

void editTextFile(bufList *head);

void testFunctionPrintAllNode(bufList *head);

#endif // TEXTLIST_H