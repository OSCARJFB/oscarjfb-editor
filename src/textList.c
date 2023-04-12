#include "textList.h"
#include <stdlib.h>
#include <stdio.h>
#include <ncurses.h>

bufList *createNodesFromBuffer(char *buffer, bufList *head, long fileSize)
{
	for (int i = 0; i < fileSize; ++i)
	{
		addNode(buffer[i], &head);
	}

	return head;
}

void addNode(int ch, bufList **head)
{
	if ((*head) == NULL)
	{
		(*head) = malloc(sizeof(bufList));
		if ((*head) == NULL)
		{
			puts("addNode: malloc(), couldn't allocate memory for node");
			return;
		}

		(*head)->ch = ch;
		(*head)->next = NULL;
		(*head)->prev = NULL; 

		return;
	}

	bufList *n_head = malloc(sizeof(bufList));
	if (n_head == NULL)
	{
		puts("addNode: malloc(), couldn't allocate memory for node");
		return;
	}

	n_head->ch = ch;
	n_head->next = NULL;

	bufList *c_node = *head;
	bufList *temp = NULL; 
	while (c_node->next != NULL)
	{
		c_node = c_node->next;
		if(c_node->next == NULL)
		{
			temp = c_node;
		}
	}

	c_node->next = n_head;
	n_head->prev = temp;
}

void printNodes(bufList *head)
{
	clear(); 
	while (head != NULL)
	{
		addch(head->ch);
		head = head->next;
	}
	refresh();
}

void deleteNode(bufList *head)
{
	if (head == NULL)
	{
		return;
	}

	while (head != NULL)
	{
		if(head->next == NULL)
		{
			break; 
		}
		head = head->next;
	}

	if(head->prev != NULL)
	{
		bufList *p_head = head->prev;  
		p_head->next = NULL; 
	}

	free(head);
	head = NULL;
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

void edit(bufList *head)
{
	int ch = 0x00;

	initscr();
	nodelay(stdscr, 1); 
	curs_set(1);
	keypad(stdscr, 1);

	if(head != NULL)
	{
		printNodes(head);
	}

	while ((ch = getch()) != 0x1b)
	{
		if(ch > 0x00)
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
				deleteNode(head);
				printNodes(head);
				break;
			default:
				addNode(ch, &head);
				printNodes(head);
			}
		}
	}

	endwin();
}