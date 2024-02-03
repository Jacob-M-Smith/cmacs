# cmacs

This is a text editor in the Emacs family but written in C. I enjoy GNU Emacs but occassionally want to make modifications and unfortunately Emacs is written in lisp which I don't know. Instead of learning a new language I decided the rational solution was to write my own editor in C.

## Tech

Cmacs is written entirely in C for Linux systems. It utilizes the Standard Library to handle file reads and writes. GUI is handled with the ncurses library.

## Versions
*version 1*
This version contains the base features required to be a text editor.

- File Input and Output (done)
- User facing display (done)
- Dynamic editable buffer (done)
  - insertion and removal of characters (done)
- Cursor movement (done)
- Return/Enter functionality (done)

*version 2*
- More advanced movement key modifiers (done)
  - ctrl + e (end of line) (done)
  - ctrl + a (start of line) (done)
- Scrolling (done)
- center command
  - ctrl + l
- Multiple Buffers and Files
- Copy, Cut and Paste
  - Text Highlighting
  - Mark setting
- Line Numbers

*version 3*
- search
  - and replace
- undo button
- syntax highlighting for C
- multiple windows

## Notes and Bugs
- A minute amount of memory is never freed by ncurses' initscr function.
- Valgrind has some errors for conditional jumps based on uninitialized values which are false flags.
- There are bugs associated with the length of the line containing the null terminator.
