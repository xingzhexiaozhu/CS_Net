#include<stdio.h>
#include<signal.h>
# define SHHHH

int main() {
	void wakeup(int signum);
	
	printf("about to sleep for 4 seconds\n");
	signal(SIGALRM, wakeup);  // catch it
	alarm(4);  // set clock
	pause();   // freeze here
	printf("Moning so soon?\n"); // back to work

	return 0;
}

void wakeup(int signum) {
	# ifdef SHHHH
	printf("Alarm received from kernel\n");
	# endif
}
