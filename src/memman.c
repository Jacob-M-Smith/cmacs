#include <stdlib.h>
#include <ncurses.h>
#include "cmacs.h"

// blanket function to catch memory allocation failures
void mem_panic()
{
    dealloc_all_buffers();
    endwin();
    printf("panic: insufficient memory");
    exit(EXIT_FAILURE);
}

void dealloc_all_buffers()
{
    for (int i = 0; i < buffers_size; i++)
    {
        if (buffers[i]->text)
            free(buffers[i]->text);
        if (buffers[i]->lines->lens)
            free(buffers[i]->lines->lens);
        if (buffers[i]->lines)
            free(buffers[i]->lines);
        if (buffers[i])
            free(buffers[i]);
    }

    free(buffers);
}
