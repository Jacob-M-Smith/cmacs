#ifndef BINDS_H
#define BINDS_H

#define CTRL(x) ((x) & 0x1f)
#define NBINDS 1000

/*Function Prototypes*/
int cursor_forward();
/*End Function Prototypes*/

#define BCKSPCE  127
#define TAB      9
#define DELETE   0        // constant adds readability to remove char function 

#define TAB_SIZE 4        // change to adjust for system tab size 
                          // legal values are multiples of 2, common values are 2, 4, 8.

struct keytable
{
    int keycode;
    int (*cmdptr)();
};

extern struct keytable kbinds[];

#endif
