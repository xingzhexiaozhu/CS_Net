#include "client.h"

int main(int argc, char** argv)
{
    clock_t start = clock();

    // step1：创建socket连接
    int sockfd = creat_socket(argc, argv);
    if (sockfd == -1) {
        exit(EXIT_FAILURE);
    }

    ThreadPool<THREAD_NUM> pool;  // 声明多线程
    //  step2：一个线程接收数据、多线程解析数据
    clock_t rp_start = clock(); 
    std::vector<char> part_data;                // 临时数组，存放一次接收数据最后不完整的部分
    std::vector<std::vector<Record*>> records;  //  
    std::vector<std::set<std::string>> rename_keys; // 有rename操作的key
    long part = 0, totalRcv = 0;
    while (1) 
    {
        std::vector<char> block_data(MAXBUF);

        if ((part = recv(sockfd, &block_data.at(0), MAXBUF, 0)) == -1)
        {
            std::cout<<"recv failed."<<std::endl;
            exit(EXIT_FAILURE);
        }
        if (part <= 0) 
        {
            break;
        }

        if (!part_data.empty()) 
        {
            block_data.insert(block_data.begin(), part_data.begin(), part_data.end());
        }
        part_data.clear();
        get_perfect_data(block_data, part_data);



        totalRcv += part;
        std::cout<<"receive: "<<totalRcv<<std::endl;
    }
    clock_t rp_end = clock();
    std::cout << "recv_parse_cost: " << ((double) rp_end - rp_start) / CLOCKS_PER_SEC * 1000 << "ms\n";
    close(sockfd);  

    return 0;
}

int creat_socket(int argc, char **argv)
{
    if (argc != 3)
    {
        std::cout<<"Use:<IP><PORT>"<<std::endl;
        return -1;
    }

    //  创建socket链接
    int sockfd;
    sockaddr_in addr;

    memset(&addr, 0, sizeof(sockaddr));
    addr.sin_family = AF_INET;
    addr.sin_port = htons(atoi(argv[2]));
    if (inet_aton(argv[1], (in_addr*)&addr.sin_addr.s_addr) == 0)
    {
        std::cout<<"ip invalid."<<std::endl;
        return -1;
    }

    if ((sockfd = socket(AF_INET, SOCK_STREAM, 0)) < 0)
    {
        std::cout<<"socket failed."<<std::endl;
        return -1;
    }

    if (connect(sockfd, (sockaddr*)&addr, sizeof(addr)) == -1)
    {
        std::cout<<"connect failed."<<std::endl;
        return -1;
    }

    std::cout<<"connect success."<<std::endl;
    return sockfd;
}

void get_perfect_data(std::vector<char> &block_data, std::vector<char> &part_data)
{
    if (block_data.empty() || block_data.back() == '\n')
    {
        return;
    }
    while (block_data.back() != '\n')
    {
        part_data.emplace_back(block_data.back());
        block_data.pop_back();
    }
}