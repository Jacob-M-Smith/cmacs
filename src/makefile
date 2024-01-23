all: cmacs

cmacs: cmacs.c
	gcc -o cmacs cmacs.c buffer.c files.c stringman.c memman.c -lncurses

debug:
	gcc -g -o cmacs cmacs.c buffer.c files.c stringman.c memman.c -lncurses

global:
	gcc -o /usr/bin/cmacs cmacs.c buffer.c files.c stringman.c memman.c -lncurses

clean:
	rm cmacs

clean_global:
	rm /usr/bin/cmacs
