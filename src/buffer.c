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

// delete is a bool for direction false for delete, true for backspace
int remove_char_from_buffer(int delete)
{
    buffer* buf = buffers[curr_buffer];
    int buf_len = strlen(buf->text);
    int pos = buf->pos;

    if (buf->text[buf->pos] == '\n')
        buf->depth--;

    if (delete == 0)  // remove current char
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
    if (*char_ptr == '\n')
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
                        update_line_count();
                        y--;
                        x = buf->lines->lens[y];
                        move(y, x);
                        buf->pos--;
                    }
                }
                break;
            case CTRL('n'):
                if (y == buf->depth)
                    break;
                y++;
                update_line_count();
                if (buf->lines->lens[y] < x)   // next line is longer short than current x pos
                {
                    buf->pos += (buf->lines->lens[y - 1] - x) + buf->lines->lens[y];
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
            case CTRL('d'):
                if (buf->text[buf->pos] == '\0')
                    break;
                remove_char_from_buffer(DELETE);
                update_display = 1;
                x++;
                buf->pos++;
            case CTRL('h'):
                if (buf->pos == '\0')
                    break;
                remove_char_from_buffer(BCKSPCE);
                update_display = 1;
                buf->pos--;
                x--;
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
        printw(buf->text);
        move(y, x);
    }
}
