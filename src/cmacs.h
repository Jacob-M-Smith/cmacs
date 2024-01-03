#ifndef CMACS_H
#define CMACS_H

#define CTRL(x) ((x) & 0x1f)

#define BCKSPCE  127
#define TAB      9

typedef struct buffer
{
    char*  fname;  
    uint   pos;            // position of cursor in linear buffer
    uint   size;           // amount of memory allocated to the buffer
    uint   depth;          // number of newline characters in a file   
    char*  text;
    char** newline_record; // contains the starting char of every newline
} buffer;

// buffer management variables
extern buffer** buffers;
extern uint buffers_size;
extern uint curr_buffer;

// files.c (handles file input and output)
void     dealloc_all_buffers();
void     open_file(char*);
void     update_file();
void     mem_panic();

// buffer.c (handle buffer management)
int      add_char_to_buffer(char);
int      process_keystroke(int);
int      remove_char_from_buffer(int);
size_t   strline(const char*);
void     update_newline_record();

// cmacs.c is driver
#endif
