all: cmacs

cmacs: cmacs.c
	gcc -o cmacs cmacs.c buffer.c -lncurses

clean:
	rm cmacs
