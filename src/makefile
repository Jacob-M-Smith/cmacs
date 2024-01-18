all: cmacs

cmacs: cmacs.c
	gcc -o cmacs cmacs.c buffer.c files.c -lncurses

debug:
	gcc -g -o cmacs cmacs.c buffer.c files.c -lncurses

clean:
	rm cmacs
