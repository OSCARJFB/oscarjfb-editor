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
	coordinates xy = {0, 0};

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

	updateCoordinates(&head);
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
		free(buffer);
		buffer = NULL;
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
bufList *saveCopiedText(bufList *head, coordinates cpy_start, coordinates cp_end)
{
	bufList *cpy_List = NULL;
	bool start_found = false;
	while (head != NULL)
	{
		// Start were copy point is found, add every node until the end of the list is found.
		if (((head->x == cpy_start.x && head->y == cpy_start.y) || start_found))
		{
			bufList *next_node = createNewNode(head->ch);
			if (next_node == NULL)
			{
				return NULL;
			}

			if (cpy_List == NULL)
			{
				cpy_List = next_node;
				start_found = true;
			}
			else
			{
				bufList *last_node = cpy_List, *prev_node = cpy_List;
				while (last_node->next != NULL)
				{
					last_node = last_node->next;
				}

				last_node->next = next_node;
				last_node->prev = prev_node;
			}
		}

		// If true end of list is found.
		if (head->x == cp_end.x && head->y == cp_end.y)
		{
			break;
		}

		head = head->next;
	}

	return cpy_List;
}

void pastecpy_List(bufList **head, bufList *cpy_List, coordinates xy)
{
	if (*head == NULL || cpy_List == NULL)
	{
		return;
	}

	// First the "paste" location should be found.
	bufList *preList = *head;
	while (preList->next != NULL)
	{
		if (preList->x == xy.x && preList->y == xy.y)
		{
			break;
		}

		preList = preList->next;
	}

	// The new line character needs to be moved after the printed list.
	if (preList->ch == '\n' && preList->prev != NULL)
	{
		preList = preList->prev;
	}

	// This node will be connected to the end of the copied list.
	bufList *postList = preList->next;

	// Connect the start of the copied list.
	preList->next = cpy_List;
	cpy_List->prev = preList;

	// connect the remaining list if it exists.
	if (postList != NULL)
	{

		cpy_List->next = postList;
		postList->prev = cpy_List;
	}
}

void deleteAllNodes(bufList *head)
{
	if (head == NULL)
	{
		return;
	}

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

bufList *createNewNode(int ch)
{
	bufList *next_node = malloc(sizeof(bufList));
	if (next_node == NULL)
	{
		return NULL;
	}

	next_node->ch = ch;
	next_node->next = NULL;
	next_node->prev = NULL;
	return next_node;
}

void addNode(bufList **head, int ch, coordinates xy)
{
	// Currently there is no list existing.
	if (*head == NULL)
	{
		*head = createNewNode(ch);
		return;
	}

	// Create a new node and add base values, depending on parameter input.
	bufList *next_node = createNewNode(ch);
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
			return;
		}
		else if (last_node->x == xy.x && last_node->y == xy.y && last_node->prev == NULL)
		{
			last_node->prev = next_node;
			*head = next_node;
			next_node->next = last_node;
			return;
		}

		last_node = last_node->next;
	}

	// Add the new node to the end of the list.
	prev_node = last_node;
	last_node->next = next_node;
	next_node->prev = prev_node;
	return;
}

void deleteNode(bufList **head, coordinates xy)
{
	// We can't free/delete a node which is NULL.
	if (*head == NULL)
	{
		return;
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
		free(*head);
		*head = NULL;
		return;
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
	}

	if (del_node != NULL)
	{
		free(del_node);
		del_node = NULL;
	}
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

void updateCoordinates(bufList **head)
{
	if (*head == NULL)
	{
		return;
	}

	int x = 0, y = 0;
	bufList *node = *head;
	while (node != NULL)
	{
		node->x = x;
		node->y = y;
		++x;
		if (node->ch == '\n')
		{
			x = 0;
			++y;
		}
		node = node->next;
	}
}

dataCopied getCopyStart(dataCopied cpy_data, coordinates xy)
{
	if(cpy_data.isStart)
	{
		return cpy_data;
	}

	cpy_data.cpy_start.x = xy.x;
	cpy_data.cpy_start.y = xy.y;
	cpy_data.isStart = true; 

	return cpy_data;
}

dataCopied getCopyEnd(dataCopied cpy_data, coordinates xy)
{
	if(cpy_data.isStart && cpy_data.isEnd)
	{
		cpy_data.cpy_end.x = xy.x;
		cpy_data.cpy_end.y = xy.y;
		cpy_data.isStart = cpy_data.isEnd = false;
	}

	if(cpy_data.isStart)
	{
		cpy_data.isEnd = true;
	}

	return cpy_data;
}


void editTextFile(bufList *head, const char *fileName)
{
	coordinates xy = getEndNodeCoordinates(head);
	dataCopied cpy_data = {NULL, {0, 0}, {0, 0}, false, false};
	int ch = NO_KEY, size = 0, mode = EDIT;

	initscr();
	noecho();
	nodelay(stdscr, true);
	curs_set(1);
	keypad(stdscr, true);

	updateCoordinates(&head);
	size = printNodes(head);
	while ((ch = wgetch(stdscr)))
	{
		if (ch == NO_KEY)
		{
			continue;
		}

		if (ch == ESC_KEY)
		{
			mode = setMode();
		}

		if (mode == EDIT)
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
				deleteNode(&head, xy);
				break;
			default:
				addNode(&head, ch, xy);
				break;
			}
		}
		else if(mode == SAVE)
		{
			save(head, size, fileName);
		}
		else if(mode == COPY)
		{
			cpy_data = getCopyStart(cpy_data, xy); 
			cpy_data = getCopyEnd(cpy_data, xy);

			if(!cpy_data.isStart && !cpy_data.isEnd)
			{
				cpy_data.cpy_List = saveCopiedText(head, cpy_data.cpy_start, cpy_data.cpy_end); 
			}
		}
		else if(mode == PASTE)
		{
			pasteCopiedText(&head, cpy_data.cpy_List, xy); 
			mode = EDIT;
		}

		if(mode == EDIT)
		{
			updateCoordinates(&head);
			size = printNodes(head);
			wmove(stdscr, xy.y, xy.x);
			wrefresh(stdscr);
		}
		mode = EDIT; 
	}

	deleteAllNodes(head);
	endwin();
}
