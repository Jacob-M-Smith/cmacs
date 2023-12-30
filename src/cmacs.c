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

    if (buffer_list_size == 0)
    {
        if ((buffer_list = (buffer**)malloc(sizeof(buffer*))) == NULL)
        {
            printf("failed to allocate memory\n");
            free(text);
            return;
        }        
    }
    else
    {
        if ((realloc(buffer_list, sizeof(buffer*) * (buffer_list_size + 1))) == NULL)
        {
            printf("failed to grow size of buffer list\n");
            free(text);
            return;
        }                
    }
    
    buffer_list[buffer_list_size] = (buffer*)malloc(sizeof(buffer));
    buffer_list[buffer_list_size]->size = strlen(text);
    buffer_list[buffer_list_size]->pos = 0;
    buffer_list[buffer_list_size]->text = text;
    buffer_list[buffer_list_size]->fname = fname;
    current_buffer = buffer_list_size;
    buffer_list_size++;
}


void dealloc_all_buffers()
{
    for (int i = 0; i < buffer_list_size; i++)
    {
        free(buffer_list[i]->text);
        free(buffer_list[i]);
    }

    free(buffer_list);
}

void update_file(WINDOW* win)
{
    FILE* fd;
    char* fname = buffer_list[current_buffer]->fname;
    char* text = buffer_list[current_buffer]->text;
    fd = fopen(fname, "w");
    fwrite(text, strlen(text), 1, fd);
    fclose(fd);
}
