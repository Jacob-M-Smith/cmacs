#ifndef CMACS_H
#define CMACS_H

#define CTRL(x) ((x) & 0x1f)

#define BCKSPCE  127
#define TAB      9

typedef struct buffer
{
    char*  fname;  
    int    pos;            // position of cursor in linear buffer
    int    x, y;           // x, y value of screen cursor
    uint   size;           // amount of memory allocated to the buffer
    char*  text;
    char** newline_record; // contains the starting char of every newline
} buffer;

// buffer management variables
extern buffer** buffers;
extern uint buffers_size;
extern uint curr_buffer;

// files.c (handles file input and output)
void     open_file(char*);
void     update_file();

// buffer.c (handle buffer management)
void     dealloc_all_buffers();
int      add_char_to_buffer(char);
int      remove_char_from_buffer(int);
int      process_keystroke(int);
size_t   strline(const char*);

// cmacs.c is driver
#endif
