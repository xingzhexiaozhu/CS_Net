#include<stdio.h>
#include<stdlib.h>
#include<ctype.h>
#include<string.h>
#include<iostream>
#include<unistd.h>
#include<sys/types.h>
#include<sys/socket.h>
#include<arpa/inet.h>

#include<sys/epoll.h>

#define SERVER_PORT 7777
#define EPOLL_MAX_NUM 1024
#define BUFFER_LEN 4096

char buffer[BUFFER_LEN];

void str_toupper(char *str)
{
	int i;
	for(int i=0; i<strlen(str); i++)
	{
		str[i] = toupper(str[i]);
	}
}

int main(int argc, char **argv)
{
	int listen_fd, client_fd;
	struct sockaddr_in server_addr, client_addr;
	socklen_t len;

	int epoll_fd;
	struct epoll_event event, *my_events;

	// socket
	if ((listen_fd = socket(AF_INET, SOCK_STREAM, 0)) == -1)
    {
        std::cout<<"socket failed."<<std::endl;
        exit(EXIT_FAILURE);
    }

	//bind
	server_addr.sin_family = AF_INET;
	server_addr.sin_port = htons(SERVER_PORT);
	server_addr.sin_addr.s_addr = htonl(INADDR_ANY);
	if (bind(listen_fd, (sockaddr *)&server_addr, sizeof(server_addr)) == -1)
    {
        std::cout<<"bind failed."<<std::endl;
        exit(EXIT_FAILURE);
    }

	// listen
	if (listen(listen_fd, 100) == -1)
    {
        std::cout<<"listen failed."<<std::endl;
        exit(EXIT_FAILURE);
    }

	// epoll create
	epoll_fd = epoll_create(EPOLL_MAX_NUM);
	if (epoll_fd < 0) 
	{
		std::cout<<"epoll create failed."<<std::endl;
		exit(EXIT_FAILURE);
	}

	// listen fd -> epoll
	event.events = EPOLLIN;
	event.data.fd = listen_fd;
	if (epoll_ctl(epoll_fd, EPOLL_CTL_ADD, listen_fd, &event) < 0) {
		std::cout<<"epoll ctl failed."<<std::endl;
		exit(EXIT_FAILURE);
	}

	my_events = malloc(sizeof(struct epoll_event) * EPOLL_MAX_NUM);
	while(1) {
		// epoll wait
		int active_fds_cnt = epoll_wait(epoll_fd, my_events, EPOLL_MAX_NUM, -1);
		int i = 0;
		for (i=0; i<active_fds_cnt; i++)
		{
			if (my_events[i].data.fd == listen_fd) {
				// accept
				client_fd = accept(listen_fd, (struct sockaddr*)&client_addr, &len);
				if (client_fd < 0)
				{
					std::cout<<"accept failed."<<std::endl;
					continue;
				}

				char ip[20];
				std::cout<<"new connection "<<inet_ntop(AF_INET, &client_addr.sin_addr, ip, sizeof(ip), ntohs(client_addr.sin_port));
				event.events = EPOLLIN | EPOLLET;
				event.data.fd = client_fd;
				epoll_ctl(epoll_fd, EPOLL_CTL_ADD, client_fd, &event);
			} else if {
				std::cout<<"EPOLLIN"<<std::endl;
				client_fd = my_events[i].data.fd;
				buffer[0] = '\0';
				int n = read(client_fd, buffer, 5);
				if (n < 0) {
					std::cout<<"read failed."<<std::endl;
					continue;
				} else if (n==0) {
					epoll_ctl(epoll_fd, EPOLL_CTL_DEL, client_fd, &event);
					close(client_fd);
				} else {
					std::cout<<"read "<<buffer<<std::endl;
					buffer[n] = '\0';
					str_toupper(buffer);
					write(client_fd, buffer, strlen(buffer));
					std::cout<<"write "<<bufer<<std::endl;
					memset(buffer, 0, BUFFER_MAX_LEN);
				}
			} else if (my_events[i].events & EPOLLOUT) {
				std::cout<<"EPOLLOUT"<<std::endl;	
			}
		}
	}
	close(epoll_fd);
	close(listen_fd);
	return 0;
}
