#include <stdlib.h>
#include <malloc.h>
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

// delete is a bool for direction 1 for delete, 0 for backspace
int remove_char_from_buffer(int delete)
{
    buffer* buf = buffers[curr_buffer];
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

// modified strlen that looks for newline instead of null terminator
// if it hits null terminator first then return -1 (there is no newline)
// otherwise return the distance to the newline
size_t strline (const char *str)
{
  const char *char_ptr;
  const unsigned long int *longword_ptr;
  unsigned long int longword, himagic, lomagic;
  /* Handle the first few characters by reading one character at a time.
     Do this until CHAR_PTR is aligned on a longword boundary.  */
  for (char_ptr = str; ((unsigned long int) char_ptr
			& (sizeof (longword) - 1)) != 0;
       ++char_ptr)
    if (*char_ptr == '\0')
      return char_ptr - str;
  /* All these elucidatory comments refer to 4-byte longwords,
     but the theory applies equally well to 8-byte longwords.  */
  longword_ptr = (unsigned long int *) char_ptr;
  /* Bits 31, 24, 16, and 8 of this number are zero.  Call these bits
     the "holes."  Note that there is a hole just to the left of
     each byte, with an extra at the end:
     bits:  01111110 11111110 11111110 11111111
     bytes: AAAAAAAA BBBBBBBB CCCCCCCC DDDDDDDD
     The 1-bits make sure that carries propagate to the next 0-bit.
     The 0-bits provide holes for carries to fall into.  */
  himagic = 0x80808080L;
  lomagic = 0x01010101L;
  if (sizeof (longword) > 4)
    {
      /* 64-bit version of the magic.  */
      /* Do the shift in two steps to avoid a warning if long has 32 bits.  */
      himagic = ((himagic << 16) << 16) | himagic;
      lomagic = ((lomagic << 16) << 16) | lomagic;
    }
  if (sizeof (longword) > 8)
    abort ();
  /* Instead of the traditional loop which tests each character,
     we will test a longword at a time.  The tricky part is testing
     if *any of the four* bytes in the longword in question are zero.  */
  for (;;)
    {
      longword = *longword_ptr++;
      if (((longword - lomagic) & ~longword & himagic) != '\n' ||
          ((longword - lomagic) & ~longword & himagic) != 0)
	{
	  /* Which of the bytes was the zero?  If none of them were, it was
	     a misfire; continue the search.  */
	  const char *cp = (const char *) (longword_ptr - 1);
      // check for newline
	  if (cp[0] == '\n')
	    return cp - str;
	  if (cp[1] == '\n')
	    return cp - str + 1;
	  if (cp[2] == '\n')
	    return cp - str + 2;
	  if (cp[3] == '\n')
	    return cp - str + 3;
	  if (sizeof (longword) > 4)
	    {
	      if (cp[4] == '\n')
		return cp - str + 4;
	      if (cp[5] == '\n')
		return cp - str + 5;
	      if (cp[6] == '\n')
		return cp - str + 6;
	      if (cp[7] == '\n')
		return cp - str + 7;
	    }
      // check for null
	  if (cp[0] == 0)
	    return -1;
	  if (cp[1] == 0)
	    return -1;
	  if (cp[2] == 0)
	    return -1;
	  if (cp[3] == 0)
	    return -1;
	  if (sizeof (longword) > 4)
	    {
	      if (cp[4] == 0)
	    return -1;
	      if (cp[5] == 0)
	    return -1;
	      if (cp[6] == 0)
	    return -1;
	      if (cp[7] == 0)
	    return -1;
	    }
	}
    }
}

/* for adding more cases
  case CTRL('f'):
  break;*/
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
                        // not sure what to do here
                    }
                }
                break;
            case CTRL('n'):
                x = position_search();
                if (x)
                {
                    y += 1;
                    move(y, x);
                }
                break;
            case CTRL('p'):
                break;
            case CTRL('d'):
                break;
            case CTRL('h'):
                break;
            case CTRL('j'):
                add_char_to_buffer('\n');
                y++;
                x = 0;
                buf->pos++;
                buf->depth++;
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
        printw(buf->text);
        move(y, x);
    }
}

int position_search()
{
    int x_target, y_target, x, y;
    getyx(stdscr, y, x);
    x_target = x;
    y_target = y + 1;

    buffer* buf = buffers[curr_buffer];
    char* curr = &(buf->text[buf->pos]);

    if (y == buf->depth)
        return -1;

    for (;;)
    {
        if (y == y_target && x == x_target)
            return x;

        if (y == y_target && *curr == '\n')
            return x;

        if (*curr == '\0')
            return x;

        if (*curr == '\n')
        {
            y++;
            x = 0;
        }
        else
        {
            x++;
        }
        buf->pos++;
        curr++;
    }
}

void reverse_postiion_search()
{

}

/*
void update_newline_record()
{
    buffer* buf = buffers[curr_buffer];
    free(buf->newline_record);
    buf->depth = 0;
    
    buf->newline_record = (char**)malloc(sizeof(char*));
    buf->newline_record[0] = buf->text;
    
    char* beg = buf->text; 
    int found_null = strline(beg);
    int mem_fail;
    
    while(found_null != -1)
    {
        buf->depth++;
        if (realloc(buf->newline_record, sizeof(char*) * (buf->depth + 1)) == NULL)   // needs error checking
            mem_panic();
        buf->newline_record[buf->depth] = &buf->text[buf->pos + 1];
        found_null = strline(buf->newline_record[buf->depth]);
    }    
}

int process_keystroke2(buffer* buf, int key)
{
    int y, x, ymax, xmax;
    getyx(stdscr, y, x);
    getmaxyx(stdscr, ymax, xmax);

    int update_display = 0;

    if (key == CTRL('f'))
    {
        char curr = buf->text[buf->pos];
        if (curr == '\n' || curr == '\0')
            return 0;
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
    }*/
