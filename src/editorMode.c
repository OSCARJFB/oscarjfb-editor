/*
	Writen by: Oscar Bergström
	https://github.com/OSCARJFB

	MIT License
	Copyright (c) 2023 Oscar Bergström
*/

#include "editorMode.h"
#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>
#include <ncurses.h>

int _leftMargin = 0;
int _rightMargin = 0;
int _tabSize = 6;

bufList *createNodesFromBuffer(char *buffer, long fileSize)
{
	if (buffer == NULL)
	{
		return NULL;
	}

	coordinates xy = {0, 0};
	bufList *head = NULL;

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

	if (fileName == NULL)
	{
		/* Ask for a new file name here! */
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
		return NULL;
	}

	for (int i = 0; head != NULL && i < size; head = head->next)
	{
		buffer[i++] = head->ch;
	}

	buffer[size] = '\0';

	return buffer;
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

coordinates addNode(bufList **head, int ch, coordinates xy)
{
	// Currently there is no list existing.
	if (*head == NULL)
	{
		*head = createNewNode(ch);
		xy.x = _leftMargin + 1;
		xy.y += ch == '\n' ? 1 : 0;
		return xy;
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

			xy.x = ch == '\n' ? _leftMargin : last_node->x + 1;
			xy.y += ch == '\n' ? 1 : 0;
			return xy;
		}
		else if (last_node->x == xy.x && last_node->y == xy.y && last_node->prev == NULL)
		{
			last_node->prev = next_node;
			*head = next_node;
			next_node->next = last_node;

			xy.x = ch == '\n' ? _leftMargin : last_node->x + 1;
			xy.y += ch == '\n' ? 1 : 0;
			return xy;
		}

		last_node = last_node->next;
	}

	// Add the new node to the end of the list.
	prev_node = last_node;
	last_node->next = next_node;
	next_node->prev = prev_node;

	xy.x = ch == '\n' ? _leftMargin : last_node->x + 2;
	xy.y += ch == '\n' ? 1 : 0;
	return xy;
}

coordinates deleteNode(bufList **head, coordinates xy)
{
	// We can't free/delete a node which is NULL or if at end of coordinates.
	if (*head == NULL || (xy.x == _leftMargin && xy.y == 0))
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
		xy.x = _leftMargin;
		xy.y = 0;
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

	setLeftMargin(*head);

	int x = _leftMargin, y = 0;
	bufList *node = *head;
	while (node != NULL)
	{
		node->x = x;
		node->y = y;

		if (node->ch == '\t')
		{
			x += _tabSize;
		}
		else
		{
			++x;
		}

		if (node->ch == '\n')
		{
			x = _leftMargin;
			++y;
		}

		node = node->next;
	}
}

dataCopied getCopyStart(dataCopied cpy_data, coordinates xy)
{
	if (cpy_data.isStart)
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
	if (cpy_data.isStart && cpy_data.isEnd)
	{
		cpy_data.cpy_end.x = xy.x;
		cpy_data.cpy_end.y = xy.y;
		cpy_data.isStart = cpy_data.isEnd = false;
	}

	if (cpy_data.isStart)
	{
		cpy_data.isEnd = true;
	}

	return cpy_data;
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
				bufList *last_node = cpy_List, *prev_node = NULL;
				while (last_node->next != NULL)
				{
					last_node = last_node->next;
				}

				// Add the new node to the end of the list.
				prev_node = last_node;
				last_node->next = next_node;
				next_node->prev = prev_node;
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

void pasteCopiedlist(bufList **head, bufList *cpy_List, coordinates xy)
{
	if (*head == NULL || cpy_List == NULL)
	{
		return;
	}

	// First find the paste start location should.
	bufList *preList = *head;
	for (; preList->next != NULL; preList = preList->next)
	{
		if (preList->x == xy.x && preList->y == xy.y)
		{
			break;
		}
	}

	// Last character should not be a newline.
	if (preList->ch == '\n')
	{
		preList = preList->prev;
	}

	// Connect the prelist to the copied list.
	bufList *postList = preList->next;
	preList->next = cpy_List;
	cpy_List->prev = preList;

	// Connect the post list to the copied list if any.
	if (cpy_List != NULL && postList != NULL)
	{
		for (; cpy_List->next != NULL; cpy_List = cpy_List->next)
		{
		};

		cpy_List->next = postList;
		postList->prev = cpy_List;
	}
}

void setLeftMargin(bufList *head)
{
	int newlines = 0;

	// Count all the newlines found in the text.
	while (head != NULL)
	{
		if (head->ch == '\n')
		{
			++newlines;
		}

		head = head->next;
	}

	// Set margin depending on the amount of newlines
	if (newlines < one_hundred)
	{
		_leftMargin = three;
	}
	else if (newlines < one_thousand)
	{
		_leftMargin = four;
	}
	else if (newlines < ten_thousand)
	{
		_leftMargin = five;
	}
	else if (newlines < hundred_thousand)
	{
		_leftMargin = six;
	}
}

int printNodes(bufList *head)
{
	int size = 0, newlines = 1;
	bool nlFlag = true;

	// We need to clear terminal screen (empty) if no characters exists.
	if (head == NULL)
	{
		wclear(stdscr);
		printw("%d:", newlines);
		wrefresh(stdscr);
		return size;
	}

	// Print the nodes at x and y position.
	wclear(stdscr);
	while (head != NULL)
	{
		if (nlFlag)
		{
			nlFlag = false;
			printw("%d:", newlines);
		}

		if (head->ch == '\n')
		{
			nlFlag = true;
			++newlines;
		}

		mvwaddch(stdscr, head->y, head->x, head->ch);
		head = head->next;
		++size;
	}

	if (nlFlag == true)
	{
		printw("%d:", newlines);
	}

	wrefresh(stdscr);
	return size;
}

int setMode(int ch)
{
	if (ch != ESC_KEY)
	{
		return EDIT;
	}

	ch = wgetch(stdscr);
	switch (ch)
	{
	case 's':
		return SAVE;
	case 'c':
		return COPY;
	case 'v':
		return PASTE;
	case 'e':
		return EXIT;
	}

	return EDIT;
}

coordinates moveArrowKeys(int ch, coordinates xy)
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
		xy.x += xy.x != _leftMargin ? -1 : 0;
		break;
	case KEY_RIGHT:
		++xy.x;
		break;
	}

	return xy;
}

coordinates edit(bufList **head, coordinates xy, int ch)
{
	if (ch == KEY_BACKSPACE)
	{
		xy = deleteNode(head, xy);
	}
	else if ((ch >= ' ' && ch <= '~') || (ch == '\t' || ch == '\n'))
	{
		xy = addNode(head, ch, xy);
	}

	return xy;
}

dataCopied copy(dataCopied cpy_data, bufList *head, coordinates xy)
{
	if (cpy_data.cpy_List != NULL)
	{
		deleteAllNodes(cpy_data.cpy_List);
		cpy_data.cpy_List = NULL;
	}

	cpy_data = getCopyStart(cpy_data, xy);
	cpy_data = getCopyEnd(cpy_data, xy);

	if (!cpy_data.isStart && !cpy_data.isEnd)
	{
		cpy_data.cpy_List = saveCopiedText(head, cpy_data.cpy_start, cpy_data.cpy_end);
	}

	return cpy_data;
}

void editTextFile(bufList *head, const char *fileName)
{
	dataCopied cpy_data = {NULL, {0, 0}, {0, 0}, false, false};
	updateCoordinates(&head);
	coordinates xy = getEndNodeCoordinates(head);
	int size = printNodes(head);
	bool is_running = true;

	while (is_running)
	{
		int ch = wgetch(stdscr), mode = setMode(ch);
		xy = moveArrowKeys(ch, xy);

		switch (mode)
		{
		case EDIT:
			xy = edit(&head, xy, ch);
			break;
		case SAVE:
			save(head, size, fileName);
			break;
		case COPY:
			cpy_data = copy(cpy_data, head, xy);
			break;
		case PASTE:
			pasteCopiedlist(&head, cpy_data.cpy_List, xy);
			break;
		case EXIT:
			is_running = false;
			break;
		}

		updateCoordinates(&head);
		size = printNodes(head);
		wmove(stdscr, xy.y, xy.x);
	}
}