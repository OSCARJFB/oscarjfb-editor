# oscarjfb-editor

This is supposed to be a terminal-based texteditor, I'm taking args from the main function to determine if the user reads from an existing file or not. I load this data into a buffer, this buffer is split by character into a linked list. 

Each node in the linked list is supposed to have a x, y coordinate for character position in the terminal, and also a storage type (32bit int) for the character itself, and finally a *next and *prev pointer to itself. 

By traversing the list i'm able to ncurses to set terminal to raw mode enabling editing, the editor marker is supposed to use the coordinates modifying the linked list from any location. Thus I can use mvaddch to print to the screen any changes at any location. This linked list should later on user demand be saved to a file. finally I free all allocated memory on the heap