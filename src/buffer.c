#include <stdlib.h>
#include <string.h>
#include <ncurses.h>
#include "cmacs.h"

// Pos is the position where the character should be inserted in the buffer.
int add_char_to_buffer(char character)
{
    buffer* buf = buffers[curr_buffer]; 
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

    if (pos != (new_text_len - 1))    // We are not at the end of the buffer. Shift memory
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

// delete is a bool for direction false for delete, true for backspace
int remove_char_from_buffer(int backspace)
{
    buffer* buf = buffers[curr_buffer];
    int buf_len = strlen(buf->text);
    int pos = buf->pos;

    if (!backspace)  // remove current char
    {
        if (buf->text[buf->pos] == '\n')
            buf->depth--;
        memcpy(buf->text + pos, buf->text + pos + 1, (buf->size - pos) * sizeof(char));
    }
    else         // remove previous char
    {
        if (buf->text[buf->pos - 1] == '\n')
            buf->depth--;
        if (pos == buf_len - 1)
        {
            buf->text[pos - 1] = '\0';
        }
        else
        {
            memcpy(buf->text + pos - 1, buf->text + pos, (buf->size - pos) * sizeof(char));
        }
    }
    buf->text[buf_len - 1] = '\0';
    return 1;
}

int process_keystroke(int key)
{
    int y, x, maxy, maxx;
    int update_display = 0;                // set to 1 when screen needs to be redrawn

    buffer* buf = buffers[curr_buffer];
    char curr = buf->text[buf->pos];                       
    int curr_is_null = (curr == '\0');
    int curr_is_newl = (curr == '\n');

    getyx(stdscr, y, x);
    getmaxyx(stdscr, maxy, maxx);

    if (key == BCKSPCE)
        key = CTRL('h');

    if (key < 0x1f)  // key is a control key
    {
        switch(key)
        {
            case CTRL('f'):
                if (curr_is_null)
                    break;
                if (curr_is_newl || (x == maxx - 1))
                {
                    x = 0;
                    y++;
                }
                else
                {
                    x++;
                }
                buf->pos++;
                move(y, x);
                break;
            case CTRL('b'):
                if (x != 0)
                {
                    x--;
                    buf->pos--;
                    move(y, x);
                }
                else
                {
                    if (y == 0)
                        break;
                    else 
                    {
                        update_line_count();
                        y--;
                        x = buf->lines->lens[y];
                        move(y, x);
                        buf->pos--;
                    }
                }
                break;
            case CTRL('n'):
                if (y == buf->depth - 1)
                    break;
                y++;
                update_line_count();
                if (buf->lines->lens[y] < x) 
                {
                    buf->pos += (buf->lines->lens[y - 1] - x) + buf->lines->lens[y] + 1;
                    x = buf->lines->lens[y];
                }
                else
                {
                    buf->pos += (buf->lines->lens[y - 1] - x) + x + 1;              
                }
                move(y, x);
                break;
            case CTRL('p'):
                if (y == 0)
                    break;
                y--;
                update_line_count();
                if (buf->lines->lens[y] < x)
                {
                    buf->pos -= x + 1;
                    x = buf->lines->lens[y];
                }
                else
                {
                    buf->pos -= (x + (buf->lines->lens[y] - x + 1));  
                }
                
                move(y, x);
                break;
            case CTRL('e'):
                update_line_count();
                if (x == buf->lines->lens[y])
                    break;
                buf->pos += (buf->lines->lens[y] - x);
                x = buf->lines->lens[y];
                move(y, x);
                break;
            case CTRL('a'):                
                if (x == 0)
                    break;
                buf->pos -= x;
                x = 0;
                move(y, x);
                break;
            case CTRL('d'):
                if (buf->text[buf->pos] == '\0')
                    break;
                remove_char_from_buffer(DELETE);
                update_display = 1;
                break;
            case CTRL('h'):
                if (buf->pos == 0)
                    break;
                if (buf->text[buf->pos - 1] != '\n')
                {
                    remove_char_from_buffer(BCKSPCE);
                    buf->pos--;
                    x--;
                }
                else
                {
                    remove_char_from_buffer(BCKSPCE);
                    buf->pos--;
                    y--;
                    x = buf->lines->lens[y];
                }
                update_display = 1;
                break;
            case CTRL('j'):
                add_char_to_buffer('\n');
                y++;
                x = 0;
                buf->pos++;
                buf->depth++;
                update_display = 1;                
                break;
            case TAB:  // this should probably not be here but it was convenient
                add_char_to_buffer(' ');
                add_char_to_buffer(' ');
                add_char_to_buffer(' ');
                add_char_to_buffer(' ');
                buf->pos += 4;
                x += 4;
                update_display = 1;
                break;            
            default:
                return 0;            
        }
    }
    else             // key is regular input
    {
        add_char_to_buffer((char)key);
        update_display = 1;
        buf->pos++;
        x++; // needs end of line handling
    }

    if (update_display)
    {
        clear();
        addstr(buf->topline);
        move(y, x);
    }
}
