/*
 * CS354: Operating Systems. 
 * Purdue University
 * Example that shows how to read one line with simple editing
 * using raw terminal.
 */

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <curses.h>
#define MAX_BUFFER_LINE 2048
#define MAX_HISTORY 15
// Buffer where line is stored
int line_length;
char line_buffer[MAX_BUFFER_LINE];

// Simple history array
// This history does not change. 
// Yours have to be updated.
int history_index = 0;
char * history [MAX_HISTORY];
int h_index = 0;
int history_length = 0;
int cursorPos = 0;

void add(char * line) {
	if(strlen(line) < 2) return;
	line[line_length] = '\0';
	history[h_index++ % MAX_HISTORY] = strdup(line);
	history_length = h_index;
	history_index = h_index;
}	
void insertAt(char * arg,char in,int pos) {
	if(pos < 0 || pos >= strlen(arg)) return;
	char * newArg = (char *)(malloc(sizeof(char) * (strlen(arg) + 1)));
	int flag = 0;
	int i;
	for(i = 0; i < strlen(newArg);i++) {
		if(i == pos) {
			flag = 1;
			newArg[i] = in;
		}
		newArg[i+flag] = arg[i];
	}
	free(arg);
	arg = newArg;
}

void read_line_print_usage()
{
	char * usage = "\n"
		" ctrl-?       Print usage\n"
		" Backspace    Deletes last character\n"
		" up arrow     See last command in the history\n";

	write(1, usage, strlen(usage));
}
void resetCursor(int f) {
	int i = line_length;
	while(i > cursorPos) {
		int x = 1;
		char * out = "\033[1D";
		char ch;
		write(1,out,strlen(out));

		i--;
	}

}

void pushToEnd() {
	int i = cursorPos;
	while(i < line_length) {
		int x = 1;
		char * out = "\033[1C";
		char ch;
		write(1,out,strlen(out));
		i++;
	}
}

void cleanLine(int upTill) {
	char ch;
	int i = 0;
	for (i =0; i < upTill; i++) {
		ch = 8;
		write(1,&ch,1);
	}

	// Print spaces on top
	for (i =0; i < upTill; i++) {
		ch = ' ';
		write(1,&ch,1);
	}

	// Print backspaces
	for (i =0; i < upTill; i++) {
		ch = 8;
		write(1,&ch,1);
	}	
}
void cleanLine2(int upTill,int cPos) {
	char ch;
	int i = 0;
	for (i =0; i < cPos; i++) {
		ch = 8;
		write(1,&ch,1);
	}

	// Print spaces on top
	for (i =0; i < upTill; i++) {
		ch = ' ';
		write(1,&ch,1);
	}

	// Print backspaces
	for (i =0; i < upTill; i++) {
		ch = 8;
		write(1,&ch,1);
	}	
}

void cleanLine3(int upTill,int cPos) {
	char ch;
	int i = 0;
	for (i =0; i < cPos; i++) {
		ch = 8;
		write(1,&ch,1);
	}

	// Print spaces on top
	for (i =0; i < upTill; i++) {
		ch = ' ';
		write(1,&ch,1);
	}

	// Print backspaces
	for (i =0; i < upTill-1; i++) {
		ch = 8;
		write(1,&ch,1);
	}	
}


/* 
 * Input a line with some basic editing.
 */
char * read_line() {

	// Set terminal in raw mode
	tty_raw_mode();

	line_length = 0;
	cursorPos = 0;
	// Read one line until enter is typed
	while (1) {

		// Read one character in raw mode.
		char ch;
		read(0, &ch, 1);

		if (ch < 127 && ch>=32) {
			// It is a printable character. 

			// Do echo
			if(cursorPos != line_length) {
				int i;
				if(line_length == MAX_BUFFER_LINE - 2) break;
				for(i = line_length;i>=cursorPos;i--) {
					line_buffer[i+1] = line_buffer[i];
				}
				line_buffer[cursorPos] = ch;
				//if(line_length>=3)
				cleanLine2(line_length,cursorPos);
				line_length++;
				cursorPos++;
				write(1, line_buffer, line_length);
				resetCursor(0);
				//		printf("pos:%d\nactual:%d\n",cursorPos,line_length);
			} else {

				write(1,&ch,1);

				// If max number of character reached return.
				if (line_length==MAX_BUFFER_LINE-2) break; 

				// add char to buffer.
				line_buffer[line_length]=ch;
				line_length++;
				cursorPos++;
			}
		}
		else if (ch==10) {
			// <Enter> was typed. Return line

			// Print newline
			write(1,&ch,1);

			break;
		}
		else if (ch == 1) {
			while(cursorPos > 0) {
				int x = 1;
				char * out = "\033[1D";
				char ch;
				int i;
				for (i =0; i < strlen(out); i++) {
					ch = out[i];
					write(1,&ch,1);
				}
				cursorPos--;
			}

		}
		else if(ch == 5) {
			while(cursorPos < line_length) {
				int x = 1;
				char * out = "\033[1C";
				char ch;
				int i;
				for (i =0; i < strlen(out); i++) {
					ch = out[i];
					write(1,&ch,1);
				}
				cursorPos++;
			}
		}
		else if (ch == 31) {
			// ctrl-?
			read_line_print_usage();
			line_buffer[0]=0;
			break;
		}
		else if (ch == 8 || ch == 127) {
			// <backspace> was typed. Remove previous character read.
			if(line_length > 0) {
				if(cursorPos!=line_length && cursorPos > 0) {
					if(line_length == MAX_BUFFER_LINE - 2) break;
					int i;
					for(i = cursorPos - 1;i<=line_length;i++) {
						line_buffer[i] = line_buffer[i+1];
					}
					cleanLine2(line_length,cursorPos);
					line_length--;
					cursorPos--;
					write(1, line_buffer, line_length);
					resetCursor(0);


				} else if (cursorPos > 0){

					// Go back one character
					ch = 8;
					write(1,&ch,1);

					// Write a space to erase the last character read
					ch = ' ';
					write(1,&ch,1);

					// Go back one character
					ch = 8;
					write(1,&ch,1);

					// Remove one character from buffer
					line_length--;
					cursorPos--;
				}
			}
		}
		else if (ch==27) {
			// Escape sequence. Read two chars more
			//
			// HINT: Use the program "keyboard-example" to
			// see the ascii code for the different chars typed.
			//
			char ch1; 
			char ch2;
			read(0, &ch1, 1);
			read(0, &ch2, 1);
			if (ch1==91 && ch2==65) {
				if(history_index > 0) {
					// Up arrow. Print next line in history.

					// Erase old line
					// Print backspaces
					pushToEnd();
					int i = 0;
					cleanLine(line_length);
					if(history_index) history_index=(history_index-1)%history_length;
					else history_index = h_index - 1;	
					// Copy line from history
					strcpy(line_buffer, history[history_index]);
					line_length = strlen(line_buffer);
					cursorPos = line_length;
					// echo line
					write(1, line_buffer, line_length);
				}
			}
			if (ch1==91 && ch2==66) {
				if(history_index < h_index - 1) {
					// Down arrow. Print prev line in history.

					// Erase old line
					// Print backspaces
					pushToEnd();
					int i = 0;
					cleanLine(line_length);

					// Copy line from history
					history_index=(history_index+1);
					strcpy(line_buffer, history[history_index]);
					line_length = strlen(line_buffer);
					cursorPos = line_length;
					// echo line
					write(1, line_buffer, line_length);
				} else {
					cursorPos = 0;
					cleanLine(line_length);	
					line_length = 0;
					history_index = history_length;
				}
			}

			if (ch1==91 && ch2==67) {
				//right
				if(cursorPos < line_length) {
					int x = 1;
					char * out = "\033[1C";
					char ch;
					int i;
					for (i =0; i < strlen(out); i++) {
						ch = out[i];
						write(1,&ch,1);
					}
					cursorPos++;
				}
			}

			if (ch1==91 && ch2==68) {
				//left
				if(cursorPos > 0) {
					int x = 1;
					char * out = "\033[1D";
					char ch;
					int i;
					for (i =0; i < strlen(out); i++) {
						ch = out[i];
						write(1,&ch,1);
					}
					cursorPos--;
				}


			}
			if(ch1 == 91 && ch2 == 51) {
				char ch3;
				read(0, &ch3, 1);
				if(ch3 == 126) {
					if(line_length > 0) {
						if(cursorPos!=line_length && cursorPos >= 0) {
							int x = 1;
							char * out = "\033[1C";
							write(1,out,strlen(out));
							cursorPos++;
									
							if(line_length == MAX_BUFFER_LINE - 2) break;
							int i;
							for(i = cursorPos - 1;i<=line_length;i++) {
								line_buffer[i] = line_buffer[i+1];
							}
							cleanLine2(line_length,cursorPos);
							line_length--;
							cursorPos--;
							write(1, line_buffer, line_length);
							resetCursor(0);





						} 
					}
				}
			}
		}

	}

	if(line_length) add(line_buffer);
	// Add eol and null char at the end of string
	line_buffer[line_length]=10;
	line_length++;
	line_buffer[line_length]=0;
	return line_buffer;
}

