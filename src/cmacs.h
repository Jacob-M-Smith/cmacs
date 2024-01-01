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

void     open_file(char*);
void     dealloc_all_buffers();
void     update_file();
int      add_char_to_buffer(buffer*, char);
int      process_keystroke(buffer*, int);
int      linear_coordinate_translator(int, int, int);
void*    calculate_end_ptr(void*, int, int);
size_t   strline(const char*);


#endif
