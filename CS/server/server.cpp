#include <sys/socket.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <iostream>
#include <fcntl.h>
#include <fstream>
#include <vector>
#include <ios>
#include <string.h>
#include <errno.h>
#include <stdlib.h>
#include<ctime>

#define PORT 10001

#define MAXBUF 1048576   // socket buffer，2^20

/*
 * 服务端
 *  参数：文件路径
 */ 

int main(int argc, char **argv)
{
    if (argc != 2)
    {
        std::cout<<"Use:<file_path>"<<std::endl;
        exit(EXIT_FAILURE);
    }

    // step1：加载文件
    const char *path = argv[1];
    struct stat fileInfo;
    if (stat(path, &fileInfo) == -1)
    {
        std::cout<<"get file info failed"<<std::endl;
        exit(EXIT_FAILURE);
    }

    // 加载数据耗时
    clock_t start = clock();

    std::ifstream fin(path, std::ios::binary);
    std::vector<char> buf(static_cast<unsigned int>(fin.seekg(0, std::ios::end).tellg()));
    fin.seekg(0, std::ios::beg).read(&buf[0], static_cast<std::streamsize>(buf.size()));
    fin.close();

    clock_t end = clock();
    std::cout << "load_data_cost: " << ((double) end - start) / CLOCKS_PER_SEC * 1000 << "ms\n";

    // 读文件的结果
    // for (std::vector<char>::iterator it = buf.begin(); it != buf.end(); it++)
    // {
    //     outfile << *it;
    //     std::cout << *it;
    // }

    // step2：创建连接
    int listenfd, connfd;
    sockaddr_in addr, c_addr;
    socklen_t len = sizeof(sockaddr);

    memset(&addr, 0, len);
    addr.sin_family = AF_INET;
    addr.sin_port = htons(PORT);
    addr.sin_addr.s_addr = htonl(INADDR_ANY);

	// socket
    if ((listenfd = socket(AF_INET, SOCK_STREAM, 0)) == -1)
    {
        std::cout<<"socket failed."<<std::endl;
        exit(EXIT_FAILURE);
    }

	// bind
    if (bind(listenfd, (sockaddr *)&addr, sizeof(addr)) == -1)
    {
        std::cout<<"bind failed."<<std::endl;
        exit(EXIT_FAILURE);
    }

	// listen
    if (listen(listenfd, 100) == -1)
    {
        std::cout<<"listen failed."<<std::endl;
        exit(EXIT_FAILURE);
    }

    std::cout<<"Watting for connection..."<<std::endl;

	// accept
    if ((connfd = accept(listenfd, (sockaddr*)&c_addr, &len)) == -1)
    {
        std::cout<<"accept failed."<<std::endl;
        exit(EXIT_FAILURE);
    } else {
        std::cout<<"Connection from IP: "<<inet_ntoa(c_addr.sin_addr)<<", port: "<<ntohs(c_addr.sin_port)<<", socket: "<<connfd<<std::endl;
    }

    // step3：发送数据
    long start_idx = 0, end_idx = MAXBUF;

    // 发送数据耗时
    start = clock();

    for (; end_idx < static_cast<std::streamsize>(buf.size()); start_idx = end_idx, end_idx += MAXBUF)
    {
        // std::vector<char> v(buf.begin() + start_idx, buf.begin() + end_idx);
        // for (std::vector<char>::iterator it = v.begin(); it != v.end(); it++)
        // {
        //     std::cout << *it;
        // }
        // send(connfd, &v, MAXBUF, 0);

        // // send(connfd, &buf.at(start_idx), MAXBUF, 0);
        // int send(int s, const void *buf, int len, int flags);
        if ((send(connfd, &buf.at(start_idx), MAXBUF, 0)) < 0)
        {
            std::cout<<errno<<std::endl;
            std::cout<<"send file failed."<<std::endl;
            std::cout << strerror(errno) << std::endl;
            exit(EXIT_FAILURE);
        }
        std::cout<<"total: "<<static_cast<std::streamsize>(buf.size())<<", process: "<<end_idx<<std::endl;
    }
    if (start_idx < static_cast<std::streamsize>(buf.size()))
    {
        // std::vector<char> v(buf.begin() + start_idx, buf.end());
        // for (std::vector<char>::iterator it = v.begin(); it != v.end(); it++)
        // {
        //     std::cout << *it;
        // }
        // send(connfd, &v, v.size(), 0);
        
        // // send(connfd, &buf.at(start_idx), static_cast<std::streamsize>(buf.size()) - start_idx, 0);
        if ((send(connfd, &buf.at(start_idx), static_cast<std::streamsize>(buf.size()) - start_idx , 0)) < 0)
        {
            std::cout<<"send file failed."<<std::endl;
            exit(EXIT_FAILURE);
        }
        std::cout<<"total: "<<static_cast<std::streamsize>(buf.size())<<", process: "<<end_idx<<std::endl;
    }

    end = clock();
    std::cout << "send_data_cost: " << ((double) end - start) / CLOCKS_PER_SEC * 1000 << "ms\n";

    close(listenfd);
    close(connfd);

    return 0;
}
