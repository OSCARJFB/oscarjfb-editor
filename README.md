# oscarjfb-editor

This is a terminal-based text editor, It takes args from the main function to determine whether the user is reading from an existing file or not. It loads this data into a buffer, this buffer is divided character by character in a linked list.

Each node in this linked list should have an x and y coordinate indicating character position in the terminal, it should also contain a storage type of (32bit int) for the character itself and finally a *next and *prev pointer. This linked list thus forms the structure of the text, which together with ncurses enables text editing. 

TODO

Bugs:
- The coordinates in addNode() are incorrectly calcuated on x-axis when newlines are read.
- The last node in the copy list doesn't link its *prev pointer correctly to the previous node.

Planned features: 
- Enable running the software wihout a passed file arg. 
- Save should be able to change or give a name filename to an edited file. 
- Replace functionality would be cool! In other words the possiblity of replacing all matching words in one sweep. 
- Code structure could be improved.
- Performance!!!