#	Writen by: Oscar Bergström
#   https://github.com/OSCARJFB

cc = gcc
#cflags = -Wall -Wextra -Werror -pedantic -std=c99
cflags = -std=c99

main: main.c
	$(cc) main.c textList.c $(cflags) -lncurses -o main.o

debug: 
	$(cc) main.c textList.c $(cflags) -g -lncurses -o main.o

assembly: 
	$(cc) main.c textList.c -lncurses -S assembly.s

clean:
	rm -rf *.o && rm -rf *.s