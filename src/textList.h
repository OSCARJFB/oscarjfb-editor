#ifndef TEXTLIST_H
#define TEXTLIST_H

typedef struct bufList
{
	int ch, x, y;
	struct bufList *next;
	struct bufList *prev;
} bufList;

bufList *createNodesFromBuffer(char *buffer, bufList *head, long fileSize);

void addNode(int ch, bufList **head,
			 int x, int y);

void printNodes(bufList *head);

void deleteNode(bufList **head);

void deleteAllNodes(bufList *head);

void getLastCoordinates(bufList *head, int *x, int *y);

void edit(bufList *head);

#endif // TEXTLIST_H