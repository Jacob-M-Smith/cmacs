#include <stdio.h>
#include <stdlib.h>
#include <ncurses.h>
#include <errno.h>

int main (int argc, char** argv)
{
    FILE* fd;
    char buffer[1024];

    char* name = argv[1];

    fd = fopen(name, "r");
    
    if (fd == NULL)
    {
        printf("error number %d\n", errno);
        perror("cmacs");
        return 0;
    }

    fread(buffer, 1024, 1, fd);

    initscr();
    addstr(buffer);

    refresh();

    getch();
    clear();

    getch();
    endwin();

    fclose(fd);
    return 1;
}


char* read_from_file(char* file_name)
{
    // will read data from file and store in a text buffer
    // buffer needs to be accessible outside of function
    // we also need to maintain a list of file descritpors to close later 
    // need to hold a list of buffers to free later also
}


// FUNCTION RESPONSIBILITIES

// Main
// track current buffer
// handle curses
// make scr editable
// save edits to buffer
// render a text cursor
// traps for keyboard shortcuts
// handle command line args

// Get New Buffer
// open a file descriptor
// read its text and store it in a new buffer 
// add the file pointer to a list of file pointers to close later
// add a pointer to the file to the list of pointers to files to close later
// changes current buffer

// Update File
// call when trap for saving is activated
// take active buffer and write it to file

// Shutdown 
// on exit
// close all open files
// free all allocated buffers

