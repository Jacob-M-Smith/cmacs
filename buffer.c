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
        memcpy(buf->text + pos + 1, buf->text + pos, (buf->text - pos)*sizeof(char));
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
    int suc = add_char_to_buffer(buf, (char)key);
    return suc;

    // Adding the character is the default option. We need to add cases for hotkeys and other special keys.
    // Backspace could utilize memmove?
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
