cc = gcc
cflags = -Wall -Wextra -Werror -pedantic -std=c99

main: main.c
	$(cc) main.c $(cflags) -lncurses -o main.o

debug: 
	$(cc) main.c $(cflags) -g -lncurses -o main.o

assembly: 
	$(cc) main.c -lncurses -S assembly.s

clean:
	rm -rf *.o && rm -rf *.s