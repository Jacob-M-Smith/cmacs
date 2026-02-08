#include <stdlib.h>
#include <string.h>
#include <ncurses.h>
#include "../inc/cmacs.h"

/*Contains all the functions to preform hot-key actions*/

// Keybinds structure
struct keytable kbinds[NBINDS] = 
{
    {CTRL('f'), &cursor_forward}
};

// CTRL+f
int cursor_forward()
{
    uint y, x, maxy, maxx;
    uint update_display = 0;                // set to 1 when screen needs to be redrawn
    buffer* buf = buffers[curr_buffer];     // working buffer
    char curr = buf->text[buf->pos];        // current character

    getyx(stdscr, y, x);
    getmaxyx(stdscr, maxy, maxx);

    if (curr == '\0')
        return 1;
    if ((curr == '\n') || (x == maxx - 1))
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
}

/* COMMENTED OUT UNTIL FILE SCOPE IS CORRECT
// CTRL+b
int cursor_back()
{
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
    x = buf->lines->lens[buf->line_num];
    move(y, x);
}

//CTRL+n
int line_next()
{
    if (buf->line_num + 1 == buf->depth)
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
}

//CTRL+p
int line_previous()
{
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
}

//CTRL+e
void jump_line_end()
{
    update_line_count();
    if (x == buf->lines->lens[buf->line_num])
        break;
    buf->pos += (buf->lines->lens[buf->line_num] - x);
    x = buf->lines->lens[buf->line_num];
    move(y, x);
}

//CTRL+a
void jump_line_being()
{
    if (x == 0)
        break;
    buf->pos -= x;
    x = 0;
    move(y, x);
}

//CTRL+d
void delete_char()
{
    if (buf->text[buf->pos] == '\0')
        break;
    remove_char_from_buffer(DELETE);
    update_display = 1;
}

//CTRL+h
void backspace()
{
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
}

//CTRL+j
void return()
{
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
}

//CTRL+l
void center_screen()
{
    if (buf->line_num < (maxy / 2) + 1 || y == maxy / 2)
        break;
    buf->curr_depth += y - (maxy / 2);
    y = maxy / 2;
    buf->disp_start = lineaddr(buf->curr_depth);
    update_display = 1;
}

//CTRL+v
void next_page()
{
    if (buf->depth < buf->curr_depth +  maxy - 2) // scrolling would jump over EOF
        break; 
    x = 0, y = 0;
    buf->curr_depth += (maxy - 2);
    buf->line_num = buf->curr_depth;
    buf->disp_start = lineaddr(buf->curr_depth); 
    buf->pos = (int)(buf->disp_start - buf->text);  // new position
    update_display = 1;
}

//META+v
void previous_page()
{
    // lots of this case will need to be adjusted based on inclusion of a menu bar
    if (buf->curr_depth == 0)           // we cannot scroll up (we are already at top, maybe we have not written a full "page worth of text")
        break;
    if (buf->curr_depth <= maxy)         // we cannot scroll up the full page
    {
        buf->curr_depth = 0;
        buf->disp_start = buf->text;
        buf->line_num = maxy - 1;
        buf->pos = (int)(lineaddr(buf->line_num) - buf->text);
    }
    else
    {
        buf->line_num = buf->curr_depth;
        buf->pos = (int)(lineaddr(buf->line_num) - buf->text);
        buf->curr_depth -= maxy - 1;
        buf->disp_start = lineaddr(buf->curr_depth);
    }
    x = 0; 
    y = maxy - 1;
    update_display = 1;
}

// CTRL+x, s
void save_current_buffer()
{
    update_file();
}

// CTRL+x, CTRL+s
void save_all_buffers()
{
    // change to update all files
    update_file();
}

// CTRL+<
void top_of_file()
{
    if (buf->pos = 0)
        break;
    buf->disp_start = buf->text;
    buf->line_num = 0;
    buf->pos = 0;
    x = 0, y = 0;
    update_display = 1;
}

//CTRL+>
void bottom_of_file()
{
     buf->pos = strlen(buf->text);
     buf->line_num = buf->depth - 1;
     x = buf->lines->lens[buf->line_num];

     if (buf->depth > maxy) // in this case change where the display starts 
     {
         buf->curr_depth = buf->depth - (maxy - 5);
         buf->disp_start = lineaddr(buf->curr_depth);
         y = maxy - 6;
     }
     else       
         y = buf->depth - 1;
     update_display = 1;
}
*/
