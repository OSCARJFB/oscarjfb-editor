#	Writen by: Oscar Bergström
#   https://github.com/OSCARJFB
#	
#	MIT License
#	Copyright (c) 2023 Oscar Bergström

cc = gcc
cflags := -g -Wall -Wextra -Werror -pedantic -std=c99

cflags_release := -O3 -march=native -mtune=native -flto -fomit-frame-pointer


main: main.c
	$(cc) main.c fileHandler.c editorMode.c curseSettings.c $(cflags) -lncurses -o main.o

debug: 
	$(cc) main.c fileHandler.c editorMode.c curseSettings.c $(cflags) -g -lncurses -o main.o

assembly: 
	$(cc) main.c fileHandler.c editorMode.c curseSettings.c -lncurses -S assembly.s

release: 
	$(cc) main.c fileHandler.c editorMode.c $(cflags_release) -lncurses -o main.o

clean:
	rm -rf *.o && rm -rf *.s