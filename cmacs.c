#include <stdio.h>
#include <stdlib.h>
#include <ncurses.h>
#include <errno.h>
#include <sys/stat.h>
#include "cmacs.h"

char** buffer_list;
FILE** open_file_list;
uint buffer_list_size = 0;                 // open number of buffers should always 
                                           // equal number of open files
uint current_buffer = 0;

// ^^^^^^^^^^^^^^
//consider moving globals to main

int main (int argc, char** argv)
{
    char* name = argv[1];

    open_file(name);
    // screen loop goes here

    initscr();
    addstr(buffer_list[current_buffer]);

    refresh();

    getch();
    clear();

    getch();
    endwin();

    close_all_files();

    return 1;
}

// should this return int for errors?
// passed prelim testing (need in depth testing)
void open_file(char* name)
{
    FILE* fd;
    off_t file_size;
    fd = fopen(name, "r+");

    if (fd == NULL)
    {
        printf("error number %d\n", errno);
        perror("cmacs failed to open file");
        return;
    }

    struct stat stat_buffer;
    if (stat(name, &stat_buffer) == -1)
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

    if (buffer_list_size == 0)
    {
        if (((open_file_list = (FILE**)malloc(sizeof(FILE*))) == NULL)
             || (buffer_list = (char**)malloc(sizeof(char*))) == NULL)
        {
            printf("failed to allocate memory\n");
            fclose(fd);
            free(buffer);
            return;
        }        
    }
    else
    {
        if ((open_file_list = (FILE**)malloc(sizeof(FILE*) * (buffer_list_size + 1))) == NULL || ((buffer_list = (char**)malloc(sizeof(char*) * (buffer_list_size + 1))) == NULL))
        {
            printf("failed to allocate memory\n");
            fclose(fd);
            free(buffer);
            return;
        }                
    }

   buffer_list[buffer_list_size] = buffer;
   open_file_list[buffer_list_size] = fd;
   current_buffer = buffer_list_size;
   buffer_list_size++;
}


void close_all_files()
{
    for (int i = 0; i < buffer_list_size; i++)
    {
        fclose(open_file_list[i]);
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

// create structure for maintaining buffers
// contains char*, file name 
// we are going to switch to opening and closing files immediately after use
// example open, read, close or open, write, close not open, lots of stuff, close
// should change close_files to dealloc_all_buffers
