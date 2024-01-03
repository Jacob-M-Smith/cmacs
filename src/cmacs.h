#ifndef CMACS_H
#define CMACS_H

#define CTRL(x) ((x) & 0x1f)

#define BCKSPCE  127
#define TAB      9

typedef struct buffer
{
    char* fname;  
    int   pos;            // position of cursor in linear buffer
    int   x, y;           // x, y value of screen cursor
    uint  size;           // amount of memory allocated to the buffer
    char* text;
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
int      add_char_to_buffer(buffer*, char);
int      process_keystroke(buffer*, int);
size_t   strline(const char*);

// cmacs.c is driver
#endif
