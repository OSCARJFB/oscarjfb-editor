/*
	Writen by: Oscar Bergström
	https://github.com/OSCARJFB

	MIT License
	Copyright (c) 2023 Oscar Bergström
*/

#include "editorMode.h"
#include <ncurses.h>
#include "editorList.h"


int setMode(void)
{	
	int ch;
	while((ch = wgetch(stdscr)))
	{
		switch (ch)
		{
		case 'e':
			return EDIT;
		case 's':
			return SAVE;
		case 'c': 
			return COPY;
		}
	}

	return EDIT;
}
