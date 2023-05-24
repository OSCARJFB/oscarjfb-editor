#	Writen by: Oscar Bergström
#   https://github.com/OSCARJFB
#	
#	MIT License
#	Copyright (c) 2023 Oscar Bergström

cc = gcc
cflags = -Wall -Wextra -Werror -pedantic -std=c99 #-fsanitize=address

main: main.c
	$(cc) main.c textList.c $(cflags) -lncurses -o main.o

debug: 
	$(cc) main.c textList.c $(cflags) -g -lncurses -o main.o

assembly: 
	$(cc) main.c textList.c -lncurses -S assembly.s

clean:
	rm -rf *.o && rm -rf *.s