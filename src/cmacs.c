#include <stdio.h>
#include <stdlib.h>
#include <ncurses.h>
#include <errno.h>
#include <sys/stat.h>
#include <string.h>
#include "cmacs.h"

int main (int argc, char** argv)
{
    char* fname = argv[1];

    open_file(fname);
    // screen loop goes here

    initscr();
    noecho();
    raw();

    clear();
    printw(buffer_list[current_buffer]->text);
    refresh();

    move(0, 0);

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
