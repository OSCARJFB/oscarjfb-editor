#ifndef TEXTLIST_H
#define TEXTLIST_H

typedef struct bufList
{
	int ch, x, y;
	struct bufList *next;
	struct bufList *prev;
} bufList;

bufList *createNodesFromBuffer(char *buffer, bufList *head, long fileSize);

void updateXYNodesDel(bufList **head);

void updateXYNodesAdd(bufList **head, int x, int y);

void addNode(int ch, bufList **head,
			 int x, int y);

void printNodes(bufList *head);

void deleteNode(bufList **head, int x, int y);

void getLastCoordinates(bufList *head, int *x, int *y);

void editTextFile(bufList *head);

void deleteAllNodes(bufList *head);

void tempPrintAllNode(bufList *head);
#endif // TEXTLIST_H