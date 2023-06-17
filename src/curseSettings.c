/*
	Writen by: Oscar Bergström
	https://github.com/OSCARJFB

	MIT License
	Copyright (c) 2023 Oscar Bergström
*/

#include "curseSettings.h"
#include <ncurses.h>

void curseMode(void)
{
	static bool isCurse = false;

	if (!isCurse)
	{
		initscr();
		cbreak();
		noecho();
		curs_set(1);
		keypad(stdscr, TRUE);
		isCurse = true;
	}
	else
	{
		endwin();
	}
}