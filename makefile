all	: tetris.o
	gcc -o run tetris.c -lncurses 

clean:
	rm run *.o
