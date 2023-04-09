main: main.c
	cc main.c -lncurses -o main.o

clean:
	rm -rf *.o