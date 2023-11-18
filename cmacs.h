#ifndef CMACS_H
#define CMACS_H

typedef struct buffer
{
    char* fname;  
    int   pos;
    char* end_ptr;   // points to end of text buffer
    char* text;
} buffer;

void     open_file(char*);
void     dealloc_all_buffers();
void     update_file();
int      add_char_to_buffer(buffer*, char);
int      process_keystroke(buffer*, int);
int      linear_coordinate_translator(int, int, int);
void*    end_ptr_calculator(void*, int, int);


#endif
