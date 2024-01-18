all: cmacs

cmacs: cmacs.c
	gcc -o cmacs cmacs.c buffer.c files.c -lncurses

debug:
	gcc -g -o cmacs cmacs.c buffer.c files.c -lncurses

global:
	gcc -o /usr/bin/cmacs cmacs.c buffer.c files.c -lncurses

clean:
	rm cmacs

clean_global:
	rm /usr/bin/cmacs
