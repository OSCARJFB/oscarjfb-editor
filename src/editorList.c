/*
	Writen by: Oscar Bergström
	https://github.com/OSCARJFB

	MIT License
	Copyright (c) 2023 Oscar Bergström
*/

#include "editorList.h"
#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>
#include <ncurses.h>
#include "editorMode.h"

bufList *createNodesFromBuffer(char *buffer, bufList *head, long fileSize)
{
	coordinates xy;
	xy.x = xy.y = 0;

	// Add each character from the read file to the list.
	for (int i = 0; i < fileSize; ++i)
	{
		addNode(&head, buffer[i], xy);

		if (buffer[i] == '\n')
		{
			++xy.y;
			xy.x = 0;
			continue;
		}

		++xy.x;
	}

	return head;
}

void save(bufList *head, int size, const char *fileName)
{
	char *buffer = saveListToBuffer(head, size);
	if(buffer == NULL)
	{
		return; 
	}

	FILE *fp = fopen(fileName, "w"); 
	if(fp == NULL)
	{
		return; 
	}

	fprintf(fp, "%s", buffer); 
	fclose(fp); 
	free(buffer); 
}

char *saveListToBuffer(bufList *head, int size)
{
	char *buffer = malloc((size * sizeof(char)) + 1); 
	if(buffer == NULL)
	{
		puts("saveListToBuffer: malloc failed.");
		return NULL;  
	}

	for(int i = 0; head != NULL && i < size; head = head->next)
	{
		buffer[i++] = head->ch; 
	}

	buffer[size] = '\0';

	return buffer; 
}

void deleteAllNodes(bufList *head)
{
	// Delete and free every single node.
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

coordinates updateXYNodesAdd(bufList **head)
{
	bufList *next_node = (*head)->next; 
	int lx = (*head)->x, ly = (*head)->y;

	// Update x and y of all remaining nodes meanwhile the node is not NULL.
	for (;(*head) != NULL; (*head) = (*head)->next)
	{
		(*head)->x = lx;
		(*head)->y = ly;

		if ((*head)->ch == '\n')
		{
			++ly;
			lx = 0;
		}
		else
		{
			++lx;
		}
	}
	coordinates xy = {next_node->x, next_node->y};
	return xy; 
}

void updateXYNodesDel(bufList **head)
{
	int lx = (*head)->x;
	int ly = (*head)->y;

	// This flow will execute if it is a newline character
	if ((*head)->ch == '\n')
	{
		lx = (*head)->x;
		for (*head = (*head)->next; *head != NULL; *head = (*head)->next)
		{
			if ((*head)->ch == '\n')
			{
				++ly;
				lx = 0;
				for (bufList *line_node = (*head)->next;
					 line_node != NULL && line_node->y == ly;
					 line_node = line_node->next)
				{
					++lx;
				}
			}

			(*head)->y = ly;
			(*head)->x = lx;
			++lx;
		}

		return;
	}

	// This flow will be triggered if the change is on the same line.
	for (*head = (*head)->next;
		 *head != NULL && (*head)->ch;
		 *head = (*head)->next)
	{
		(*head)->x = lx;
		(*head)->y = ly;

		if ((*head)->ch == '\n')
		{
			++ly;
			lx = 0;
		}
		else
		{
			++lx;
		}
	}
}

bufList *createNewNode(coordinates xy, int ch)
{
	bufList *new_node = malloc(sizeof(bufList));
	if(new_node == NULL)
	{
		return NULL;
	}

	new_node->x = xy.x;
	new_node->y = xy.y;
	new_node->ch = ch;
	new_node->next = NULL;
	new_node->prev = NULL;
	
	return new_node; 
}

coordinates addNode(bufList **head, int ch, coordinates xy)
{
	// Currently there is no list existing.
	if (*head == NULL)
	{
		*head = createNewNode(xy, ch); 
		return xy;
	}

	// Create a new node and add base values, depending on parameter input.
	bufList *new_node = createNewNode(xy, ch); 
	bufList *last_node = *head, *prev_node = NULL;

	// Find the last node in the list.
	while (last_node->next != NULL)
	{
		// If added at cursor position, link the new node inbetween the old nodes.
		if (last_node->x == xy.x && last_node->y == xy.y && last_node->prev != NULL)
		{
			last_node->prev->next = new_node;
			new_node->prev = last_node->prev;
			last_node->prev = new_node;
			new_node->next = last_node;
			return updateXYNodesAdd(&new_node); 
		}
		else if(last_node->x == xy.x && last_node->y == xy.y && last_node->prev == NULL)
		{
			last_node->prev = new_node; 
			*head = new_node;
			new_node->next = last_node; 
			return updateXYNodesAdd(&new_node);
		}

		last_node = last_node->next;
	}

	// Add the new node to the end of the list.
	prev_node = last_node;
	last_node->next = new_node;
	new_node->prev = prev_node;
	xy.x = new_node->ch == '\n' ? 0 : new_node->x + 1;
	xy.y += new_node->ch == '\n' ? 1 : 0;
	return xy;
}

coordinates deleteNode(bufList **head, coordinates xy)
{
	// We can't free/delete a node which is NULL.
	if (*head == NULL || (xy.x == 0 && xy.y == 0))
	{
		return xy;
	}

	bool isEndNode = true;
	bufList *del_node = *head, *temp_node = NULL;

	// Find the node to be deleted.
	while (del_node->next != NULL)
	{
		// Is a node in the middle of the list.
		if (del_node->x == xy.x && del_node->y == xy.y)
		{
			del_node = del_node->prev;
			isEndNode = false;
			break;
		}

		// Is the node just before the last node in the list.
		if (del_node->next->x == xy.x && del_node->next->y == xy.y && del_node->next->next == NULL)
		{
			isEndNode = false;
			break;
		}

		del_node = del_node->next;
	}

	// If both prev and next are NULL this is the only node in the list.
	if (del_node->prev == NULL && del_node->next == NULL)
	{
		xy.x = (*head)->x;
		xy.y = (*head)->y;
		free(*head);
		*head = NULL;
		return xy;
	}

	// Adjust the linking of nodes depending on it being the last node or a node in the middle of the list.
	if (isEndNode)
	{
		del_node->prev->next = NULL;
	}
	else if (!isEndNode)
	{
		temp_node = del_node;

		if (del_node->prev != NULL && del_node->next != NULL)
		{
			temp_node->prev->next = temp_node->next;
			temp_node->next->prev = temp_node->prev;
		}
		else if (del_node->prev == NULL && del_node->next != NULL)
		{
			temp_node->next->prev = NULL;
			*head = temp_node->next;
		}

		updateXYNodesDel(&temp_node);
	}

	if (del_node != NULL)
	{
		xy.x = del_node->x;
		xy.y = del_node->y;
		free(del_node);
		del_node = NULL;
	}

	return xy;
}

int printNodes(bufList *head)
{
	int size = 0; 

	if(head == NULL)
	{
		return size;
	}

	// Print the nodes at x and y position.
	wclear(stdscr);

	while (head != NULL)
	{
		mvwaddch(stdscr, head->y, head->x, head->ch);
		head = head->next;
		++size; 
	}

	wrefresh(stdscr);

	return size;   
}
coordinates getEndNodeCoordinates(bufList *head)
{
	coordinates xy = {0, 0};

	// Will find the last node and set its x and y value to be the cursor position.
	while (head != NULL)
	{
		if (head->next == NULL)
		{
			break;
		}
		head = head->next;
	}

	if (head != NULL)
	{
		xy.x = head->x + 1;
		xy.y = head->y;
	}

	return xy;
}

void editTextFile(bufList *head, const char *fileName)
{
	coordinates xy = getEndNodeCoordinates(head);
	int ch = NULL_KEY, size = 0;
	int mode = EDIT; 

	initscr();
	noecho();
	nodelay(stdscr, true);
	curs_set(true);
	keypad(stdscr, true);

	size = printNodes(head);
	while ((ch = wgetch(stdscr)))
	{
		if(ch == ESC_KEY)
		{
			mode = setMode(); 
		}

		if (ch > NULL_KEY && mode == EDIT)
		{
			switch (ch)
			{
			case KEY_UP:
				xy.y += xy.y != 0 ? -1 : 0;
				break;
			case KEY_DOWN:
				++xy.y;
				break;
			case KEY_LEFT:
				xy.x += xy.x != 0 ? -1 : 0;
				break;
			case KEY_RIGHT:
				++xy.x;
				break;
			case KEY_BACKSPACE:
				xy = deleteNode(&head, xy);
				size = printNodes(head);
				break;
			default:
				xy = addNode(&head, ch, xy);
				size = printNodes(head);
				break;
			}
		}
		else if(mode == SAVE)
		{
			save(head, size, fileName);
		}
		else if(mode == COPY)
		{
			// Copy pase goes here!
		}

		wmove(stdscr, xy.y, xy.x);
	}

	deleteAllNodes(head);
	endwin();
}
