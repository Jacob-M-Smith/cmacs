#include <stdlib.h>
#include <malloc.h>
#include <string.h>
#include "cmacs.h"


// Pos is the position where the character should be inserted in the buffer.
int add_char_to_buffer(buffer* buf, char character)
{
    int new_buffer_size = strlen(buf->text) + 1;
//    int pos = buf->pos;
    int pos = new_buffer_size; // For testing only.

    size_t memory_size = malloc_usable_size(buf->text);
    size_t buffer_memory_consumed = new_buffer_size * sizeof(char);           // We include the + 1 because we will be adding a char.

    if (memory_size < buffer_memory_consumed)
    {
        if ((realloc(buf->text, memory_size + 1024)) == NULL)
        {
            printf("realloc failed\n");
            return 0;
        }
    }

    if (pos != new_buffer_size)                                               // This means we are not inserting at the end of the char* and need to shift the characters.
    {
        memmove(&buf->text[pos], &buf->text[pos + 1], strlen(&buf->text[pos]));
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
