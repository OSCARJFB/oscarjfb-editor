#include "textList.h"
#include <stdlib.h>
#include <stdio.h>
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

void deleteNode(bufList **head)
{
	if (*head == NULL)
	{
		return;
	}

	bufList *end_node = *head;

	while (end_node != NULL)
	{
		if (end_node->next == NULL)
		{
			break;
		}

		end_node = end_node->next;
	}

	if (end_node->prev != NULL)
	{
		bufList *prev_node = end_node->prev;
		prev_node->next = NULL;
	}
	else
	{
		free(*head);
		*head = NULL;
		return;
	}

	free(end_node);
	end_node = NULL;
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

void getLastCoordinates(bufList *head, int *x, int *y)
{
	while(head != NULL)
	{
		if(head->next == NULL)
		{
			break; 
		}

		head = head->next;
	}

	*x = head->x + 1;
	*y = head->y;
}

void edit(bufList *head)
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

	while ((ch = getch()) != 0x1b)
	{
		if (ch > 0x00)
		{
			switch (ch)
			{
			case KEY_UP:
				break;
			case KEY_DOWN:
				break;
			case KEY_LEFT:
				break;
			case KEY_RIGHT:
				break;
			case KEY_BACKSPACE:
				deleteNode(&head);
				printNodes(head);
				break;
			default:
				addNode(ch, &head, x++, y);
				printNodes(head);
				if(ch == '\n')
				{
					++y;
					x = 0;  
				}
			}
		}
	}

	endwin();
}