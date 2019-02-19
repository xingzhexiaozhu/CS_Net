#include<stdio.h>
#include<signal.h>
#include<unistd.h>

int main() {
	void f(int);		//声明信号处理函数
	int i;

//	signal(SIGINT, SIG_IGN);	//忽略信号
	signal(SIGINT, SIG_DFL);	//默认处理
//	signal(SIGINT, f);		//调用信号处理函数

	for (int i=0; i<5; i++) {
		printf("hello\n");
		sleep(1);
	}
	
	return 0;
}

void f(int signum) {
	printf("O happend\n");
}
