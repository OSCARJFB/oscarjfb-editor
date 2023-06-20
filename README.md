# oscarjfb-editor

This is a terminal-based text editor, It takes args from the main function to determine whether the user is reading from an existing file or not. It loads this data into a buffer, this buffer is divided character by character in a linked list.

Each node in this linked list should have an x and y coordinate indicating character position in the terminal, it should also contain a storage type of (32bit int) for the character itself and finally a *next and *prev pointer. This linked list thus forms the structure of the text, which together with ncurses enables text editing. 

TODO

Bugs:
- Tab is not working correctly.
- Repaste of copied text doesn't work.

Planned features: 
- Enable running the software wihout a passed file arg. 
- Save should be able to change or give a name filename to an edited file. 
- Find and Replace functionality would be cool! In other words the possiblity of replacing all matching words in one sweep. 
- Code structure could be improved.
- Performance!

Fixed
- Calculation of x coordinates when newline is entered.
- copy and pasting list creation and pasting.