#include <stdlib.h>
#include <ncurses.h>
#include "cmacs.h"

int main (int argc, char** argv)
{
    if (argc == 1)   // graceful failure when we have no file
    {
        printf("Please provide the name of the file to be read or created.\n");
        printf("Example usage: cmacs my_file\n");
        return 0;
    }
    
    char* fname = argv[1];   

    buffers_size = 0;
    open_file(fname);

    initscr();
    noecho();
    //    set_tabsize(4);
    raw();

    clear();
    addstr(buffers[curr_buffer]->text);
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

                // if key ==  27 go to alt menu and read another code 
                // if there is no other code (esc) exit 
                continue;
            }
        }
        else
        {           
            process_keystroke(key);
        }

        refresh();
    }

    clear();
    endwin();
    dealloc_all_buffers();

    return 1;
}
