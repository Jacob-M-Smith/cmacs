#include <stdlib.h>
#include <malloc.h>
#include <string.h>
#include <ncurses.h>
#include "cmacs.h"


// Pos is the position where the character should be inserted in the buffer.
int add_char_to_buffer(buffer* buf, char character)
{
    int new_text_len = strlen(buf->text) + 1;
    int buffer_size = buf->size;
    int pos = buf->pos;

    if ((new_text_len - 1) >= buffer_size)
    {
        if ((buf->text = (char*)realloc(buf->text, buffer_size + 1024)) == NULL)
        {
            printf("realloc failed\n");
            return 0;
        }
        buf->size += 1024;
    }

    if (pos != (new_text_len - 1))                                               // This means we are not inserting at the end of the char* and need to shift the characters.
    {        
        memcpy(buf->text + pos + 1, buf->text + pos, (buffer_size - pos) * sizeof(char));
        buf->text[pos] = character;
        return 1;
    }
    else
    {
        buf->text[pos] = character;
        buf->text[pos + 1] = '\0';
        return 1;
    }
}

int process_keystroke(buffer* buf, int key)
{

    if (key == CTRL('f'))
    {
        int y, x, ymax, xmax;
        getyx(stdscr, y, x);
        getmaxyx(stdscr, ymax, xmax);
        if (x < xmax)
        {
            x++;
            move(y, x);
            buf->pos = linear_coordinate_translator(x, y, xmax);
            buf->x = x;
        }
    }
    else if (key == CTRL('b'))
    {
        int y, x, ymax, xmax;
        getyx(stdscr, y, x);
        getmaxyx(stdscr, ymax, xmax);
        if (x > 0)
        {
            x--;
            move(y, x);
            buf->pos = linear_coordinate_translator(x, y, xmax);
            buf->x = x;
        }
    }
    else if (key == CTRL('n'))
    {
        int y, x, ymax, xmax;
        getyx(stdscr, y, x);
        getmaxyx(stdscr, ymax, xmax);
        if (y < ymax)
        {
            y++;
            move(y, x);
            buf->pos = linear_coordinate_translator(x, y, xmax);
            buf->y = y;
        }
    }
    else if (key == CTRL('p'))
    {
        int y, x, ymax, xmax;
        getyx(stdscr, y, x);
        getmaxyx(stdscr, ymax, xmax);
        if (y > 0)
        {
            y--;
            move(y, x);
            buf->pos = linear_coordinate_translator(x, y, xmax);
            buf->y = y;
        }
    }
    else
    {
        add_char_to_buffer(buf, (char)key);
        int y, x, ymax, xmax;
        getyx(stdscr, y, x);
        getmaxyx(stdscr, ymax, xmax);
        clear();
        x++;
        buf->x = x;
        buf->pos = linear_coordinate_translator(x, y, xmax);
        printw(buf->text);
        move(y, x);
    }

    return 1;
}

// Calculates the index for the buffer from the x and y coordinates of the window's cursor.
int linear_coordinate_translator(int x, int y, int x_max)
{  
    return (x_max * y) + x;
}

// TODO

// Handle control keys in process_keystroke.
// Movement strokes with wmove

// Function to change buffers
// Adjust current buffer number.
// Create a new buffer if necessary.

// Linear translation function.
