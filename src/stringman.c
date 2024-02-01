#include <stdlib.h>
#include <string.h>
#include "cmacs.h"

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

// gets the address of the first character in the line specified by linenum
char* lineaddr(uint linenum)
{
    buffer* buf = buffers[curr_buffer];
    if (linenum > buf->depth)       // bounds check
        return NULL;

    update_line_count();

    if (linenum == 0)
        return buf->text;

    uint curr_len = 0;
    for (uint i = 0; i < linenum; i++)
        curr_len += buf->lines->lens[i] + 1;

    return buf->text + (curr_len * sizeof(char));
}


// counts the number of newline chars in the buffer to set depth var
uint count_newline()
{
    char* curr = buffers[curr_buffer]->text;
    uint counter = 0;

    while(*curr != '\0')
    {
        if (*curr == '\n')
            counter++;

        curr++;
    }

    return counter;
}

void update_line_count()
{
    buffer* buf = buffers[curr_buffer];
    line* lines = buf->lines;
    char* curr = buf->text;

    // realloc on size dif
    if (buf->depth != lines->size)
    {
        // SAME REALLOC TEMP CHECK
        if ((realloc(lines->lens, sizeof(int) * buf->depth)) == NULL)
            mem_panic();
        lines->size = buf->depth;
    }

    int pos = 0;
    for (int i = 0; i < buf->depth; i++)
    {
        if (i == buf->depth - 1)
        {
            lines->lens[i] = strlen(curr);
        }
        else 
        {
            pos = lines->lens[i] = strline(curr);
        }
        curr = &curr[pos + 1];
    }
}
