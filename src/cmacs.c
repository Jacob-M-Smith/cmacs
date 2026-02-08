#include <stdlib.h>
#include <ncurses.h>
#include "../inc/cmacs.h"

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
    detab();

    initscr();
    noecho();
    raw();

    clear();
    addstr(buffers[curr_buffer]->text);
    refresh();

    move(0, 0);
    int key = 0;

    while(1)
    {
        key = getch();
        if (key == BCKSPCE)
            key = CTRL('h');
        
        if (key == CTRL('x'))
        {
            key = getch();
            
            if (key == CTRL('c'))
                break;
            ctrl_x_sub_commands(key); 
        }
        else if (key == 27)                 // key is either escape or meta + some key.
        {
            key = getch();
            if (key != 27)                  // This means the 27 was a meta key. Call the appropriate function.
                meta_commands(key);
            // Otherwise 27 key was escape. Do nothing.
        }
        else if (key < 0x1f && key != TAB)
            ctrl_commands(key);
        else
            process_keystroke(key);

        refresh();
    }

    clear();
    endwin();
    dealloc_all_buffers();

    return 1;
}
