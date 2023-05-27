/*
	Writen by: Oscar Bergström
	https://github.com/OSCARJFB

	MIT License
	Copyright (c) 2023 Oscar Bergström
*/

#ifndef EDITORMODE_H
#define EDITORMODE_H

enum mode
{
	EDIT = 0,
	SAVE = 1,
	COPY = 2
};

int setMode(void);


#endif // EDITORMODE_H