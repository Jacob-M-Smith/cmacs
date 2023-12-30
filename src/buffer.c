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

// delete is a bool for direction 1 for delete, 0 for backspace
int remove_char_from_buffer(buffer* buf, int delete)
{
    int buf_len = strlen(buf->text);
    int pos = buf->pos;

    if (delete)  // remove current char
    {
        if (pos == buf_len)
            return 1;
        else
        {
            memcpy(buf->text + pos, buf->text + pos + 1, (buf->size - pos) * sizeof(char));
        }           

    }
    else         // remove previous char
    {
        if (pos == buf_len - 1)
        {
            buf->text[pos - 1] = '\0';
        }
        else
        {
            memcpy(buf->text + pos - 1, buf->text + pos, (buf->size - pos) * sizeof(char));
        }
    }
    return 1;
}

int process_keystroke(buffer* buf, int key)
{
    int y, x, ymax, xmax;
    getyx(stdscr, y, x);
    getmaxyx(stdscr, ymax, xmax);
    int update_display = 0;

    if (key == CTRL('f'))
    {
        if (x < xmax)
        {
            x++;
            move(y, x);
            buf->pos++;
            buf->x = x;
        }
    }
    else if (key == CTRL('b'))
    {
        if (x > 0)
        {
            x--;
            move(y, x);
            buf->pos--;
            buf->x = x;
        }
    }
    else if (key == CTRL('n'))
    {
        if (y < ymax)
        {
            y++;
            move(y, x);
//            buf->pos;
            buf->y = y;
        }
    }
    else if (key == CTRL('p'))
    {
        if (y > 0)
        {
            y--;
            move(y, x);
// NEEDS MORE COMPLICATED MATH FOR BUFFER POSITION
//            buf->pos = linear_coordinate_translator(x, y, xmax);
            buf->y = y;
        }
    }
    else if (key == CTRL('d'))
    {
        remove_char_from_buffer(buf, 1);
        update_display = 1;
    }
    else if (key == CTRL('h') || key == BCKSPCE)
    {
        remove_char_from_buffer(buf, 0);
        update_display = 1;
        buf->pos--;
        x--;
    }
    else if (key == CTRL('j'))
    {
        add_char_to_buffer(buf, '\n');
        y++;
        x = 0;
        buf->pos += 2;
        update_display = 1;
    }
    else if (key < 0x1f)
    {
        return 0;  // stop unimplemented hotkeys
    }
    else
    {
        add_char_to_buffer(buf, (char)key);
        update_display = 1;
        buf->pos++;
        x++;
    }

    if (update_display)
    {
        clear();
        printw(buf->text);
        move(y, x);
    }

    return 1;
}
