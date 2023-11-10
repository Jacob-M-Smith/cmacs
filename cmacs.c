#include <stdio.h>
#include <stdlib.h>
#include <ncurses.h>
#include <errno.h>
#include <sys/stat.h>
#include <string.h>
#include "cmacs.h"

buffer_info** buffer_list;
uint buffer_list_size = 0;
uint current_buffer = 0;

int main (int argc, char** argv)
{
    char* fname = argv[1];

    open_file(fname);
    // screen loop goes here

    initscr();
    addstr(buffer_list[current_buffer]->buffer);

    refresh();

    while(1)
    {
        int key = getch();
        if (key == 10)                                       // Replace this key with the short cut to save.
        {
            printf("write to file here");
            break;                                          // Once we have shortcuts working this can be removed. We need to setup proper exiting.
        }
    }
    
    endwin();

    dealloc_all_buffers();

    return 1;
}

// should this return int for errors?
// passed prelim testing (need in depth testing)
void open_file(char* fname)
{

    FILE* fd;
    off_t file_size;

    fd = fopen(fname, "a+");       // append + should open or create

    if (fd == NULL)
    {
        printf("error number %d\n", errno);
        printf("failed to open file %s", fname);
        perror("cmacs");
        exit(0);
    }

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

    buffer_list[buffer_list_size] = (buffer_info*)malloc(sizeof(buffer_info));
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

void update_file()
{
    FILE* fd;
    char* fname = buffer_list[current_buffer]->fname;
    char* buffer = buffer_list[current_buffer]->buffer;
    fd = fopen(fname, "w");
    fwrite(buffer, strlen(buffer), 1, fd);
    fclose(fd);
}


// TODO

// Save screen contents to files associated buffer.
// Handle more complex keyboard inputs (shortcuts, arrows, sigactions).
// Include a shortcut to add new files and change buffers (still need to test).
// Render a text cursor (currently invisible).
// How do we handle when the contents of the screen is larger than the size of the buffer?
// write function to change buffer

// Change Buffer
// switch to a new buffer
// given a name search the buffer list for the new buffer
// update buffer number
// create new buffer on unrecognized name

// NOTE
// seg faults are most likely to occur in open_file
