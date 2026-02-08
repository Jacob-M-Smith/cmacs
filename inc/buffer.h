#ifndef BUFFER_H
#define BUFFER_H

/*Function Prototypes*/
int      add_char_to_buffer(char);
int      process_keystroke(int);
uint     detab();
int      remove_char_from_buffer(int);
int      ctrl_commands(int);
int      ctrl_x_sub_commands(int);
int      meta_commands(int);
void     redraw(int, int);

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
    uint   line_num;       // the line number where the cursor currently resides in the buffer
    uint   curr_depth;     // meta data used to track earliest line in buffer to be displayed
    char*  disp_start;     // an address within the bounds of text where display should start
    char*  text;           // string holding the contents of the file
} buffer;

// buffer management variables
extern buffer** buffers;
extern uint buffers_size;
extern uint curr_buffer;

#endif
