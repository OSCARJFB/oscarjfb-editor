/*
	Writen by: Oscar Bergström
	https://github.com/OSCARJFB

	MIT License
	Copyright (c) 2023 Oscar Bergström
*/

#include <stdio.h>
#include <stdlib.h>
#include "curseSettings.h"
#include "fileHandler.h"

int main(int argc, char **argv)
{
	curseMode(true);
	startUp(argc, argv);	
	curseMode(false);
	return 0;
}
