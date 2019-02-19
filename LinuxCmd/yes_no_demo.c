#include<stdio.h>
#include<termios.h>
#include<fcntl.h>
#include<string.h>
#include<signal.h>
#include<unistd.h>

#define	QUESTION	"Do you want another transaction"
#define	TRIES		3		//max tries
#define	SLEEPTIME	2		//time per try
#define	BEEP		putchar('\a')	//alter user


void tty_mode(int);
void set_crmode();
void set_cr_noecho_mode();
void ctrl_c_handler(int);
void set_nodelay_mode();
int get_response(char *, int);

int main() {
	int response;

	tty_mode(0); 	//save tty mode

	//set_crmode();	//set chr-by-chr mode
	set_cr_noecho_mode();	//set-icanon, -echo
	set_nodelay_mode();	//no input >= EOF

	signal(SIGINT, ctrl_c_handler); //handle INT
	signal(SIGQUIT, SIG_IGN);	//ignore quit signals

	response = get_response(QUESTION, TRIES);	//get some answer
	tty_mode(1);

	return response;
}

void tty_mode(int how) {
	static struct termios original_mode;

	static int original_flags;
	static int stored = 0;

	if (how == 0) {
		tcgetattr(0, &original_mode);
		original_flags = fcntl(0, F_GETFL);
		stored = 1;
	}else if (stored) {
		tcsetattr(0, TCSANOW, &original_mode);
		fcntl(0, F_SETFL, original_flags);
	}
}

void set_crmode() {
	struct termios ttystate;
	tcgetattr(0, &ttystate);
	ttystate.c_lflag &= ~ICANON;	//no buffering
	ttystate.c_cc[VMIN] = 1;	//get one char at a time
	tcsetattr(0, TCSANOW, &ttystate);	//install settings
}

void set_cr_noecho_mode() {
	struct termios ttystate;
	tcgetattr(0, &ttystate);
	ttystate.c_lflag &= ~ICANON;
	ttystate.c_lflag &= ~ECHO;	//no echo either
	ttystate.c_cc[VMIN] = 1;
	tcsetattr(0, TCSANOW, &ttystate);
}

void set_nodelay_mode() {
	int termflags;
	termflags = fcntl(0, F_GETFL);
	termflags |= O_NDELAY;
	fcntl(0, F_SETFL, termflags);
}

void ctrl_c_handler(int signum) {
	tty_mode(1);
	exit(1);	
}

int get_response(char *question, int maxtries) {
	int input;
	printf("%s(y/n)?", question);
	fflush(stdout);
	while(1) {
		sleep(SLEEPTIME);
		input = tolower(get_ok_char());
		switch(input) {
			case 'y':
			case 'Y': return 0;
			case 'n':
			case 'N': return 1;
			//case EOF: return 1;
			//default: 
			//	printf("\n cannot understand %c, Please type y or n", input);
		}
		if (maxtries-- == 0)
			return 2;
		BEEP;
	}
}

int get_ok_char() {
	int c;
	while((c = getchar()) != EOF && strchr("yYnN", c) == NULL)
		;
	return c;
}
