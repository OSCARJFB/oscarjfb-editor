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
	if (buffer == NULL)
	{
		return;
	}

	FILE *fp = fopen(fileName, "w");
	if (fp == NULL)
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
	if (buffer == NULL)
	{
		puts("saveListToBuffer: malloc failed.");
		return NULL;
	}

	for (int i = 0; head != NULL && i < size; head = head->next)
	{
		buffer[i++] = head->ch;
	}

	buffer[size] = '\0';

	return buffer;
}
bufList *saveCopiedText(bufList *head, coordinates cp_start, coordinates cp_end)
{
	bufList *copiedList = NULL; 
	bool start_found = false; 
	while(head != NULL)
	{
		// Start of copy found, add every node until ending is found. 
		if(((head->x == cp_start.x && head->y == cp_start.y) || start_found))
		{
			coordinates xy = {head->x, head->y}; 
			bufList *next_node = createNewNode(xy, head->ch); 
			if(next_node == NULL)
			{
				return NULL; 
			}

			if(copiedList == NULL)
			{
				copiedList = next_node; 
				start_found = true; 
			}
			else
			{
				bufList *last_node = copiedList, *prev_node = copiedList; 
				while(last_node->next != NULL)
				{
					last_node = last_node->next;
				}

				last_node->next = next_node; 
				last_node->prev = prev_node; 
			}
		}

		// If true end of list is found.  
		if(head->x == cp_end.x && head->y == cp_end.y)
		{	
			break;
		}

		head = head->next; 
	}

	return copiedList; 
}

void pasteCopiedList(bufList **head, bufList *copiedList, coordinates xy)
{
	bufList *next_node = NULL;

	// Loop the list until paste coordinates are found. 
	while(*head != NULL)
	{		
		// Check for paste target. 
		if((*head)->x == xy.x && (*head)->y == xy.y)
		{
			int y = (*head)->y, x = (*head)->x; 

			// Chain start.
			next_node = (*head)->next; 
			copiedList->prev = *head;

			// Loop the copied list and set its coordinates. 
			while(copiedList->next != NULL)
			{
				if(copiedList->y > y)
				{
					++y; 
				}
				
				if(copiedList->y == (*head)->y)
				{
					copiedList->x = ++x; 
				}

				copiedList->y = y; 
				copiedList = copiedList->next; 
			}

			// Chain end.
			copiedList->next = next_node;
			next_node->prev = copiedList;  
		}

		*head = (*head)->next;
	}
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
	for (; (*head) != NULL; (*head) = (*head)->next)
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
	bufList *next_node = malloc(sizeof(bufList));
	if (next_node == NULL)
	{
		return NULL;
	}

	next_node->x = xy.x;
	next_node->y = xy.y;
	next_node->ch = ch;
	next_node->next = NULL;
	next_node->prev = NULL;

	return next_node;
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
	bufList *next_node = createNewNode(xy, ch);
	bufList *last_node = *head, *prev_node = NULL;

	// Find the last node in the list.
	while (last_node->next != NULL)
	{
		// If added at cursor position, link the new node inbetween the old nodes.
		if (last_node->x == xy.x && last_node->y == xy.y && last_node->prev != NULL)
		{
			last_node->prev->next = next_node;
			next_node->prev = last_node->prev;
			last_node->prev = next_node;
			next_node->next = last_node;
			return updateXYNodesAdd(&next_node);
		}
		else if (last_node->x == xy.x && last_node->y == xy.y && last_node->prev == NULL)
		{
			last_node->prev = next_node;
			*head = next_node;
			next_node->next = last_node;
			return updateXYNodesAdd(&next_node);
		}

		last_node = last_node->next;
	}

	// Add the new node to the end of the list.
	prev_node = last_node;
	last_node->next = next_node;
	next_node->prev = prev_node;
	xy.x = next_node->ch == '\n' ? 0 : next_node->x + 1;
	xy.y += next_node->ch == '\n' ? 1 : 0;
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

	if (head == NULL)
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
	coordinates xy = getEndNodeCoordinates(head), cp_start = {0, 0}, cp_end = {0, 0};
	bufList *copiedList = NULL; 
	int ch = NULL_KEY, size = 0, mode = EDIT;
	bool copy = false; 

	initscr();
	noecho();
	nodelay(stdscr, true);
	curs_set(true);
	keypad(stdscr, true);

	size = printNodes(head);
	while ((ch = wgetch(stdscr)))
	{
		if (ch == ESC_KEY)
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
		else if (mode == SAVE)
		{
			save(head, size, fileName);
		}
		else if (mode == COPY)
		{
			// Get start point of text copy action.
			if(!copy)
			{	
				// If copy action is reused and list is not empty clear old data!
				if(copiedList != NULL)
				{
					deleteAllNodes(copiedList); 
					copiedList = NULL; 
				}
				cp_start.x = xy.x;
				cp_start.y = xy.y;
				copy = true; 
			} // Get end point of text copy action.
			else 
			{
				cp_end.x = xy.x;
				cp_end.y = xy.y;
				copiedList = saveCopiedText(head, cp_start, cp_end); 
				copy = false; 
			}
		}
		else if (mode == PASTE && copiedList != NULL)
		{
			pasteCopiedList(&head, copiedList, xy);
			size = printNodes(head);
			deleteAllNodes(copiedList);
		}

		mode = EDIT; 
		wmove(stdscr, xy.y, xy.x);
	}

	deleteAllNodes(head);
	endwin();
}
