#ifndef CMACS_H
#define CMACS_H

void open_file(char*);
void dealloc_all_buffers();

typedef struct buffer_info
{
    char* buffer;
    char* fname;
} buffer_info;

#endif
