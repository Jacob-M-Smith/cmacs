#ifndef CMACS_H
#define CMACS_H

#define CTRL(x) ((x) & 0x1f)

#define BCKSPCE  127
#define TAB      9


typedef struct buffer
{
    char* fname;  
    uint  lin_num, lin_pos;   // current line number and linear position in line
    uint  x, y;               // x, y value of screen cursor
    line* lines_head;         // line 0
    line* current_line;       // current line we are operating on 
} buffer;

typedef struct line
{
    line* next;               // next line in sequence, null if end
    line* prev;               // previous line in sequence, null if head
    uint len;                 // length of line
    char* text;               // contents of line
} line;

void     open_file(char*);
void     dealloc_all_buffers();
void     update_file();
int      add_char_to_buffer(buffer*, char);
int      process_keystroke(buffer*, int);
int      linear_coordinate_translator(int, int, int);
void*    calculate_end_ptr(void*, int, int);


#endif
