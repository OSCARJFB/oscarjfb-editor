/*
	Writen by: Oscar Bergström
	https://github.com/OSCARJFB

	MIT License
	Copyright (c) 2023 Oscar Bergström
*/

#ifndef EDITORMODE_H
#define EDITORMODE_H

#include "editorList.h"

enum mode
{
	EDIT = 0,
	SAVE = 1,
	COPY = 2,
	PASTE = 3
};

int setMode(void);

//void initCurseMode(void);

void checkEditorMode(bufList *head, const char *fileName); 

#endif // EDITORMODE_H