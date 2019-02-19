#include<stdio.h>
#include<unistd.h>

int globvar = 6;
char buf[] = "a write to stdout\n";

int main()
{
	int var;
	pid_t pid;

	var = 88;
	printf("before fork, pid = %ld, glob = %d, var = %d\n", (long)getpid(), globvar, var);

	if ((pid = fork()) < 0) {
		printf("%s\n", "fork error");
	}else if (pid == 0) {
		globvar++;
		var++;
	}else {
		sleep(2);
	}

	printf("pid = %ld, glob = %d, var = %d\n", (long)getpid(), globvar, var);
	return 0;
}
