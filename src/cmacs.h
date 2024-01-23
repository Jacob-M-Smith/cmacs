#ifndef CMACS_H
#define CMACS_H

#define CTRL(x) ((x) & 0x1f)

#define BCKSPCE  127
#define TAB      9

#define DELETE   0        // constant adds readability to remove char function 

typedef struct line       // holds information about lines
{                         // makes navigation keys easier
    uint size;
    int* lens;
} line;

typedef struct buffer
{
    char*  fname;          // file name
    uint   pos;            // position of cursor in linear buffer
    uint   size;           // amount of memory allocated to the buffer
    uint   depth;          // number of newline characters in a file   
    line*  lines;          // structure managing line lengths records
    uint   toplinenum;     // line number used for scrolling
    char*  topline;        // an address within the bounds of text where display should start
    char*  text;           // string holding the contents of the file
} buffer;

// buffer management variables
extern buffer** buffers;
extern uint buffers_size;
extern uint curr_buffer;

// files.c (functions to handle file input and output)
void     dealloc_all_buffers();
void     open_file(char*);
void     update_file();
void     mem_panic();
uint     count_newline();
void     update_line_count();

// buffer.c (functions to handle buffer management)
int      add_char_to_buffer(char);
int      process_keystroke(int);
int      remove_char_from_buffer(int);

//stringman.c (functions to handle string manipulation)
size_t   strline(const char*);
char*    lineaddr(uint);

//memman.c 

// cmacs.c is driver
#endif
