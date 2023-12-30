#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <sys/stat.h>
#include <string.h>
#include "cmacs.h"

void open_file(char* fname)
{
    FILE* fd;
    off_t file_size;

    if (buffer_list_size == 0)
    {
        if ((buffer_list = (buffer**)malloc(sizeof(buffer*))) == NULL)
        {
            printf("failed to allocate memory\n");
            return;
        }        
    }
    else
    {
        if ((realloc(buffer_list, sizeof(buffer*) * (buffer_list_size + 1))) == NULL)
        {
            printf("failed to grow size of buffer list\n");
            return;
        }                
    }

    if ((buffer_list[buffer_list_size] = (buffer*)malloc(sizeof(buffer))) == NULL)
    {
        printf("failed to allocate buffer\n");
        return;
    }

    // setup buffer defaults
    buffer_list[buffer_list_size]->fname = fname;
    buffer_list[buffer_list_size]->lin_num = 0;
    buffer_list[buffer_list_size]->lin_pos = 0;
    buffer_list[buffer_list_size]->x = 0;
    buffer_list[buffer_list_size]->y = 0;
    buffer_list[buffer_list_size]->lines_head = NULL;
    buffer_list[buffer_list_size]->current_line = NULL;

    // buffer list management
    current_buffer = buffer_list_size;
    buffer_list_size++;

    // rip lines from file
    fd = fopen(fname, "a+");       // append + should open or create

    if (fd == NULL)
    {
        printf("error number %d\n", errno);
        printf("failed to open file %s", fname);
        perror("cmacs");
        exit(0);
    }

    line* head;
    line* current;
    char* line_content = NULL;
    size_t size;
    uint count = 0;
    while (getline(&line_content, &size, fd) != -1)
    {
        if (count)   // default
        {
            current->next = (line*)malloc(sizeof(line));
            current->next->prev = current;
            current = current->next;
            current->next = NULL;
        }         
        else
        {
            head = (line*)malloc(sizeof(line));
            current = head;
            count++;
            current->next = NULL;
            current->prev = NULL;
        }
        current->len = size;
        current->text = (char*)malloc(sizeof(char) * size);
        strcpy(current->text, (const char*) line_content);
    }       

    fclose(fd);

    buffer_list[current_buffer]->lines_head = head;
    buffer_list[current_buffer]->current_line = head;
}

void dealloc_all_buffers()
{
    line* current;
    line* temp;
    for (int i = 0; i < buffer_list_size; i++)
    {
        current = buffer_list[current_buffer]->lines_head;
        while(current != NULL)
        {
            temp = current;
            current = current->next;
            free(temp->text);
            free(temp);
        }        
    }
    free(buffer_list);
}

void display_buffer()
{
    line* current = buffer_list[current_buffer]->lines_head;
    uint count = 0;
    while (current != NULL)
    {
        move(count, 0);
        printw(current->text);
        current = current->next;
        count++;
    }
}

void update_file()
{
    FILE* fd;
    char* fname = buffer_list[current_buffer]->fname;
    line* current = buffer_list[current_buffer]->lines_head;

    fd = fopen(fname, "w");
    
    while (current != NULL)
    {
        fputs(current->text, fd);
        current = current->next;
    }
    fclose(fd);
}
