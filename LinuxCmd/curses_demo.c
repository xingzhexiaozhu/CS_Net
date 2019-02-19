#include<stdio.h>
#include<curses.h>
#include<unistd.h>

// 编译该文件用以下命令：
// curses_demo.c -o test -I/usr/include -L/usr/lib -lncurses

int main() {
	int i;

	initscr();	//turn on curses
	
	clear();	//clear screen
	
	for (i = 0; i < LINES; i++) {
		move(i, i+i);
		if (i%2 == 1)
			standout();
		addstr("Game Over!");
		if(i%2 == 1)
			standend();
		refresh();
		sleep(1);
		move(i, i+i);
		addstr("			");
	}

	//refresh();	//update the screen
	//getch();	//wait for user input
	endwin();	//turn off curses

	return 0;
}
