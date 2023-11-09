#include <stdio.h>
#include <stdlib.h>
#include <ncurses.h>
#include <errno.h>
#include <sys/stat.h>
#include "cmacs.h"

buffer_info** buffer_list;
uint buffer_list_size = 0;
uint current_buffer = 0;

int main (int argc, char** argv)
{
    char* fname = argv[1];
    printf(fname);


    open_file(fname);
    // screen loop goes here

/*    initscr();
    addstr(buffer_list[current_buffer]->buffer);

    refresh();

    getch();
    clear();

    getch();
    endwin();

    dealloc_all_buffers();*/

    return 1;
}

// should this return int for errors?
// passed prelim testing (need in depth testing)
void open_file(char* fname)
{

    FILE* fd;
    off_t file_size;

    fd = fopen(fname, "r");

    printf(fname);

    if (fd == NULL)
    {
        printf("error number %d\n", errno);
        printf("failed to open file %s", fname);
        perror("cmacs");
        exit(0);
    }

    printf("horseshit");

    struct stat stat_buffer;
    if (stat(fname, &stat_buffer) == -1)
    {
        printf("error number %d\n", errno);
        perror("cmacs could not retrieve file data");
        return;
    }

    file_size = *&stat_buffer.st_size;
    char* buffer;

    if ((buffer = (char*)malloc(sizeof(char) * file_size)) == NULL)
    {
        printf("failed to allocate memory\n");
        fclose(fd);
        return;
    }

    fread(buffer, file_size, 1, fd);
    fclose(fd);

    if (buffer_list_size == 0)
    {
        if ((buffer_list = (buffer_info**)malloc(sizeof(buffer_info*))) == NULL)
        {
            printf("failed to allocate memory\n");
            free(buffer);
            return;
        }        
    }
    else
    {
        if ((realloc(buffer_list, sizeof(buffer_info*) * (buffer_list_size + 1))) == NULL)
        {
            printf("failed to grow size of buffer list\n");
            free(buffer);
            return;
        }                
    }

   buffer_list[buffer_list_size]->buffer = buffer;
   buffer_list[buffer_list_size]->fname = fname;
   current_buffer = buffer_list_size;
   buffer_list_size++;

}


void dealloc_all_buffers()
{
    for (int i = 0; i < buffer_list_size; i++)
    {
        free(buffer_list[i]);
    }

    free(buffer_list);
}



// FUNCTION RESPONSIBILITIES

// Main
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
//    index buffer list with current buffer number
// if file does not exist then create it 

// Change Buffer
// switch to a new buffer
// given a name search the buffer list for the new buffer
// if the name does not exist then create a new buffer structure with that name 
//    and allocate it an empty buffer

// Shutdown 
// on exit
// free all allocated buffers
// free buffer list

// Switch current buffer
// search files for correct name 
// update buffer number
// create new buffer on unrecognized name

