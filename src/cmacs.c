#include <stdio.h>
#include <stdlib.h>
#include <ncurses.h>
#include <errno.h>
#include <sys/stat.h>
#include <string.h>
#include "cmacs.h"

buffer** buffer_list;
uint buffer_list_size = 0;
uint current_buffer = 0;

// cmacs driver
int main (int argc, char** argv)
{
    char* fname = argv[1];

    open_file(fname);

    initscr();
    noecho();
    raw();

    int ty, tx, by, bx;
    int y, x;

    getbegyx(stdscr, by, bx);
    getmaxyx(stdscr, ty, tx);

    clear();
    // NEED TO MAKE DISPLAY CALL HERE
    refresh();

    while(1)
    {
        int key = getch();
        
        if (key == CTRL('x'))
        {
            key = getch();
            
            if (key == CTRL('s'))            
                update_file();
            else if (key == CTRL('c'))                            
                break;
            else
            {
                // go to ctrl+x sub menu when implemented
                continue;
            }
        }
        else
        {           
            process_keystroke(buffer_list[current_buffer], key);
        }

        refresh();
    }
    
    endwin();

    dealloc_all_buffers();

    return 1;
}
