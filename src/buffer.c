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

    if (new_text_len >= buffer_size)
    {
        if ((buf->text = (char*)realloc(buf->text, buffer_size + 1024)) == NULL)
        {
            printf("realloc failed\n");
            return 0;
        }
        buf->disp_start = lineaddr(buf->line_num);
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
    uint y, x, maxy, maxx;
    uint update_display = 0;                // set to 1 when screen needs to be redrawn

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
                    buf->line_num++;
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
                    break;
                }
                if (y == 0)
                    break;
                update_line_count();
                y--;
                buf->line_num--;
                buf->pos--;
                x = buf->lines->lens[y];
                move(y, x);
                break;
            case CTRL('n'):
                if (buf->line_num + 1 == buf->depth - 1)
                    break;
                if (y == maxy - 1)
                {
                    if (buf->line_num == buf->depth - 1) // EOF
                        break;
                    buf->curr_depth += (maxy / 2);                    
                    buf->disp_start = lineaddr(buf->curr_depth);
                    y = (maxy / 2) + 1;
                    update_display = 1;
                }
                else
                    y++;
                update_line_count();
                buf->line_num++;
                if (buf->lines->lens[buf->line_num] < x) 
                {
                    buf->pos += (buf->lines->lens[buf->line_num - 1] - x) + buf->lines->lens[buf->line_num] + 1;
                    x = buf->lines->lens[buf->line_num];
                    move(y, x);
                    break;
                }
                buf->pos += (buf->lines->lens[buf->line_num - 1] - x) + x + 1;
                move(y, x);
                break;               
            case CTRL('p'):
                if (buf->line_num == 0)
                    break;
                update_line_count();
                buf->line_num--;
                if (y != 0 && buf->lines->lens[buf->line_num] < x)
                {
                    y--;
                    buf->pos -= x + 1;
                    x = buf->lines->lens[buf->line_num];
                    update_display = 1;
                    break;                
                }
                else if (y != 0)
                {
                    y--;
                    buf->pos -= (x + (buf->lines->lens[buf->line_num] - x + 1));  
                    update_display = 1;
                    break;
                }
                else if (y == 0 && buf->lines->lens[buf->line_num] < x)
                {
                    buf->curr_depth = buf->curr_depth > (maxy / 2) ? buf->curr_depth - maxy / 2 : 0;
                    y = buf->curr_depth ? (maxy / 2) - 1 : buf->line_num;
                    buf->disp_start = buf->curr_depth ? lineaddr(buf->curr_depth) : buf->text;
                    buf->pos -= x + 1;
                    x = buf->lines->lens[buf->line_num];
                    update_display = 1;
                    break;
                }
                else
                {
                    buf->curr_depth = buf->curr_depth > (maxy / 2) ? buf->curr_depth - maxy / 2 : 0;
                    y = buf->curr_depth ? (maxy / 2) - 1 : buf->line_num;
                    buf->disp_start = buf->curr_depth ? lineaddr(buf->curr_depth) : buf->text;
                    buf->pos -= (x + (buf->lines->lens[buf->line_num] - x + 1));  
                    update_display = 1;
                    break;
                }
                break;
            case CTRL('e'):
                update_line_count();
                if (x == buf->lines->lens[buf->line_num])
                    break;
                buf->pos += (buf->lines->lens[buf->line_num] - x);
                x = buf->lines->lens[buf->line_num];
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
                    buf->line_num--;
                    buf->pos--;
                    y--;
                    x = buf->lines->lens[y];
                }
                update_display = 1;
                break;
            case CTRL('j'):
                add_char_to_buffer('\n');
                if (y == maxy - 1)
                {
                    buf->curr_depth += maxy / 2;
                    y -= maxy / 2;
                    buf->disp_start = lineaddr(buf->curr_depth);
                    update_display = 1;
                }
                y++;
                x = 0;
                buf->pos++;
                buf->depth++;
                buf->line_num++;
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
        addstr(buf->disp_start);
        move(y, x);
    }
}
