#include<curses.h>
#include<unistd.h>
#include<string.h>

// 编译该文件用以下命令：
// gcc curses_demo2.c -o test -I/usr/include -L/usr/lib -lncurses

#define	LEFTEDGE	10
#define	RIGHTEDGE	30
#define	ROW		10

int main() {
	char* message = "Hello";
	char* blank = "	";
	int dir = +1;
	int pos = LEFTEDGE;

	initscr();
	clear();
	while(1){
		move(ROW, pos);
		addstr(message);
		move(LINES-1, COLS-1);
		refresh();
		sleep(1);
		move(ROW, pos);
		addstr(blank);
		pos += dir;
		if (pos >= RIGHTEDGE)
			dir = -1;
		if (pos <= LEFTEDGE)
			dir = +1;
	}
}
