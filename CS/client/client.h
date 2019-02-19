#ifndef CLIENT_H_
#define CLIENT_H_

#include <sys/socket.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>


#include <iostream>
#include <vector>
#include <set>
#include <map>
#include <ctime>

#include "thread_pool.h"
using nbsdx::concurrent::ThreadPool;

#include "record.h"
#include "tool.h"
#include "time_consuming/time_consuming_md5.h"

const char* DEFAULT_IP = "127.0.0.1"; //默认IP
const int DEFAULT_PORT = 10001;       // 默认端口号
const int THREAD_NUM = 40;            // 线程数量
const int MAXBUF = 1048576;           // socket缓冲区（2^20）
const int BASELEN = 3;                // 一行记录最少三个字符串

int creat_socket(int argc, char **argv);    // 创建socket
void get_perfect_data(std::vector<char> &block_data, std::vector<char> &part_data); // 从socket buffer中获取完整行
int parse_filter(std::vector<char> &block_data, );                       // 从完整的块中解析出Record、过滤掉非OD_的key
// int divide_key_to_map();                  // key做hash，按hash分组
// int parse_cmd_md5(std::map<std::string, Record>);  // 执行命令，回放得到结果，同时调用md5函数，结果存放map（排序）
// int sort_and_write();                   // 排序、写文件


#endif  // CLIENT_H_