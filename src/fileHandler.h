/*
    Writen by: Oscar Bergström
    https://github.com/OSCARJFB

    MIT License
    Copyright (c) 2023 Oscar Bergström
*/

#ifndef FILEHANDLER_H
#define FILEHANDLER_H

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include "editorMode.h"

FILE *getFileFromArg(int argc, char **argv);
FILE *getFile(const char *path);
void closeFile(FILE *FP);
int getFileSize(FILE *FP);
char *allocateBuffer(int fileSize);
void freeBuffer(char *buffer);
void loadBuffer(char *buffer, FILE *FP, long fileSize);
void startUp(int argc, char **argv);

#endif // FILEHANDLER_H