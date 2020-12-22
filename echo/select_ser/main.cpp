//
// Created by 刘时明 on 2020/12/20.
//

#include <iostream>
#include <cstdio>
#include <arpa/inet.h>
#include <unistd.h>

#define SERVER_PORT 8090

#define SERVER_QUEUE 10
#define FD_SET_SIZE 10
#define MAX_BUF 1024

int main()
{
    //设置服务器Addr，在18000，任意IP监听
    int serverFd;
    serverFd = socket(AF_INET, SOCK_STREAM, 0);

    /* 设置 serverFd 为非阻塞方式 */
    int opt = SO_REUSEADDR;
    setsockopt(serverFd, SOL_SOCKET, opt, &opt, sizeof(opt));

    struct sockaddr_in serverAddr{};
    socklen_t serverAddrLen = sizeof(sockaddr_in);
    serverAddr.sin_addr.s_addr = htonl(INADDR_ANY);
    serverAddr.sin_port = htons(SERVER_PORT);

    // 绑定
    if (bind(serverFd, (struct sockaddr *) &serverAddr, serverAddrLen))
    {
        std::cout << "Binding on " << SERVER_PORT << " fail." << std::endl;
        return -1;
    }

    // 创建等待队列
    listen(serverFd, SERVER_QUEUE);

    // 设定fd_set
    fd_set read_fds, test_fds;
    FD_ZERO(&read_fds);
    FD_SET(serverFd, &read_fds);

    // 非阻塞模式等待客户连接
    struct sockaddr_in clientAddr{};
    int clientFd;
    socklen_t clientAddrLen;
    int len;
    char buf[MAX_BUF];
    while (true)
    {
        FD_SET(serverFd, &read_fds);
        test_fds = read_fds;

        // 选择read_fds中可用的fd
        if (select(FD_SET_SIZE, &test_fds, (fd_set *) nullptr, (fd_set *) nullptr,
                   (struct timeval *) nullptr) > 0)
        {
            if (FD_ISSET(serverFd, &test_fds))
            {
                // 如果服务器fd可用，则为accept
                clientAddrLen = sizeof(sockaddr_in);
                clientFd = accept(serverFd, (struct sockaddr *) &clientAddr,
                                  &clientAddrLen);
                if (clientFd == -1)
                {
                    std::cout << "accept() error" << std::endl;
                    return -1;
                } else
                {
                    FD_SET(clientFd, &read_fds);
                    std::cout << inet_ntoa(clientAddr.sin_addr) << " connect"
                              << " at fd" << clientFd << std::endl;
                }
            }
            // 依次查询
            for (int fd = 0; fd < FD_SET_SIZE; fd++)
            {
                if (FD_ISSET(fd, &test_fds))
                {
                    // 如果是Client活动，进行Echo
                    len = read(fd, buf, MAX_BUF);
                    if (len > 0)
                    {
                        write(fd, buf, len);
                    } else
                    {
                        // 结束
                        std::cout << "Client end at" << clientFd << std::endl;
                        FD_CLR(fd, &read_fds);
                    }
                }
            }
        }
    }
    close(serverFd);
}