#include <stdio.h>
#include <stdlib.h>
#include <ncurses.h>
#include <errno.h>
#include <sys/stat.h>
#include <string.h>
#include "cmacs.h"

buffer** buffers;
uint buffers_size;
uint curr_buffer;

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
    char* text;

    if ((text = (char*)malloc(sizeof(char) * file_size)) == NULL)
    {
        printf("failed to allocate memory\n");
        fclose(fd);
        return;
    }

    fread(text, file_size, 1, fd);
    fclose(fd);

    if (buffers_size == 0)
    {
        if ((buffers = (buffer**)malloc(sizeof(buffer*))) == NULL)
        {
            printf("failed to allocate memory\n");
            free(text);
            return;
        }
        curr_buffer = 0;
    }
    else
    {
        if ((realloc(buffers, sizeof(buffer*) * (buffers_size + 1))) == NULL)
        {
            printf("failed to grow size of buffer list\n");
            free(text);
            return;
        }
    }
    
    buffers[buffers_size] = (buffer*)malloc(sizeof(buffer));
    buffers[buffers_size]->fname = fname;
    buffers[buffers_size]->pos = 0;
    buffers[buffers_size]->size = strlen(text);
    buffers[buffers_size]->text = text;
    curr_buffer = buffers_size;
    buffers_size++;
    buffers[curr_buffer]->depth = count_newline();
}


void dealloc_all_buffers()
{
    for (int i = 0; i < buffers_size; i++)
    {
        free(buffers[i]->text);
        free(buffers[i]);
    }

    free(buffers);
}

void update_file(WINDOW* win)
{
    FILE* fd;
    char* fname = buffers[curr_buffer]->fname;
    char* text = buffers[curr_buffer]->text;
    fd = fopen(fname, "w");
    fwrite(text, strlen(text), 1, fd);
    fclose(fd);
}

// blanket function to catch memory allocation failures
void mem_panic()
{
    dealloc_all_buffers();
    endwin();
    printf("panic: insufficient memory");
    exit(EXIT_FAILURE);
}

// counts the number of newline chars in the buffer to set depth var
uint count_newline()
{
    char* curr = buffers[curr_buffer]->text;
    uint counter = 0;

    while(*curr != '\0')
    {
        if (*curr == '\n')
            counter++;

        curr++;
    }

    return counter;
}
