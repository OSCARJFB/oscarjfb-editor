#include "textList.h"
#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>
#include <ncurses.h>

bufList *createNodesFromBuffer(char *buffer, bufList *head, long fileSize)
{
	int x = 0, y = 0;

	for (int i = 0; i < fileSize; ++i)
	{
		addNode(buffer[i], &head,
				x, y);

		if (buffer[i] == '\n')
		{
			++y;
			x = 0;
			continue;
		}

		++x;
	}

	return head;
}

void updateXYNodesDel(bufList **head)
{
	// If new line, our x will be set to its position since we want our next ch to be at that location.
	int x = (*head)->ch = '\n' ? (*head)->x : (*head)->x + 1;
	int y = (*head)->y;

	for (*head = (*head)->next;
		 *head != NULL;
		 *head = (*head)->next)
	{
		(*head)->x = x;
		(*head)->y = y;
		++x;
	}
}

void addNode(int ch, bufList **head,
			 int x, int y)
{
	if (*head == NULL)
	{
		*head = malloc(sizeof(bufList));
		if (*head == NULL)
		{
			puts("addNode: malloc(), couldn't allocate memory for node");
			return;
		}

		(*head)->x = x;
		(*head)->y = y;
		(*head)->ch = ch;
		(*head)->next = NULL;
		(*head)->prev = NULL;

		return;
	}

	bufList *new_node = malloc(sizeof(bufList));
	if (new_node == NULL)
	{
		puts("addNode: malloc(), couldn't allocate memory for node");
		return;
	}

	new_node->x = x;
	new_node->y = y;
	new_node->ch = ch;
	new_node->next = NULL;

	bufList *last_node = *head;
	bufList *prev_node = NULL;

	while (last_node->next != NULL)
	{
		last_node = last_node->next;
		if (last_node->next == NULL)
		{
			prev_node = last_node;
		}
	}

	last_node->next = new_node;
	new_node->prev = prev_node;
}

void printNodes(bufList *head)
{
	clear();
	while (head != NULL)
	{
		mvwaddch(stdscr, head->y, head->x, head->ch);
		head = head->next;
	}
	refresh();
}

/*
void tempPrintAllNode(bufList *head)
{
	printf("\n\n");
	while (head != NULL)
	{
		if (head->ch == '\n')
		{
			head->ch = 'N';
		}
		printf("%c(x:%d, y%d)\n", head->ch, head->x, head->y);
		head = head->next;
	}
}
*/

void deleteNode(bufList **head, int *x, int *y)
{
	if (*head == NULL)
	{
		*x = 0; 
		*y = 0;
		return;
	}

	bool isEndNode = true;
	bufList *del_node = *head, *temp_node = NULL;

	// Find node to be deleted.
	while (del_node->next != NULL)
	{
		// If it is a node between start and end at the cursor position.
		if (del_node->x == *x && del_node->y == *y)
		{
			// Set end node to be the node behind the cursor.
			del_node = del_node->prev;
			isEndNode = false;
			break;
		}

		del_node = del_node->next;
	}


	if (del_node->prev == NULL)
	{
		free(*head);
		*head = NULL;
		return;
	}

	if (isEndNode)
	{
		// Make sure our new end node points to NULL.
		del_node->prev->next = NULL;
	}
	else if (!isEndNode)
	{
		// chain the nodes together, prev node -> <- next node
		if (del_node->prev != NULL && del_node->next != NULL)
		{
			temp_node = del_node;
			temp_node->prev->next = temp_node->next;
			temp_node->next->prev = temp_node->prev;
			updateXYNodesDel(&temp_node);
		}
	}

	*x = del_node->x;
	*y = del_node->y;
	free(del_node);
	del_node = NULL;
}

void getLastCoordinates(bufList *head, int *x, int *y)
{
	while (head != NULL)
	{
		if (head->next == NULL)
		{
			break;
		}

		head = head->next;
	}

	*x = head->x + 1;
	*y = head->y;
}

#define ESC 0x1b
#define NUL 0x00

void editTextFile(bufList *head)
{
	int ch = 0x00, x = 0, y = 0;
	getLastCoordinates(head, &x, &y);

	initscr();
	nodelay(stdscr, 1);
	curs_set(1);
	keypad(stdscr, 1);

	if (head != NULL)
	{
		printNodes(head);
	}

	while ((ch = getch()) != ESC)
	{
		if (ch > NUL)
		{
			switch (ch)
			{
			case KEY_UP:
				--y;
				break;
			case KEY_DOWN:
				++y;
				break;
			case KEY_LEFT:
				--x;
				break;
			case KEY_RIGHT:
				++x;
				break;
			case KEY_BACKSPACE:
				deleteNode(&head, &x, &y);
				printNodes(head);
				break;
				/*
				default:
					addNode(ch, &head, x++, y);
					printNodes(head);
					if (ch == '\n')
					{
						++y;
						x = 0;
					}
				*/
			}

			move(y, x);
		}
	}

	endwin();
}

void deleteAllNodes(bufList *head)
{
	bufList *temp = NULL;
	while (head != NULL)
	{
		temp = head;
		head = head->next;
		free(temp);
	}

	temp = NULL;
	head = NULL;
}
