//
// Created by 刘时明 on 2021/1/9.
//

#include "main.h"

#include <sys/socket.h>
#include <netinet/in.h>
#include <stdio.h>
#include <errno.h>
#include <unistd.h>
#include <string.h>
#include <stdlib.h>
#include <sys/wait.h>
#include <poll.h>

#define MAX_LINE 5
#define OPEN_MAX 1024

int main()
{
    int listen_fd, conn_fd, sock_fd, i, maxi;
    int n_ready;
    socklen_t cli_len;
    ssize_t n;
    char buf[MAX_LINE];
    struct pollfd client[OPEN_MAX];
    struct sockaddr_in serv_addr{}, cli_addr{};
    //创建监听套接字
    if ((listen_fd = socket(AF_INET, SOCK_STREAM, 0)) < 0)
    {
        printf("socket() error!");
        exit(0);
    }
    //先要对协议地址进行清零
    bzero(&serv_addr, sizeof(serv_addr));
    //设置为 IPv4 or IPv6
    serv_addr.sin_family = AF_INET;
    //绑定本地端口号
    serv_addr.sin_port = htons(8090);
    //任何一个 IP 地址，让内核自行选择
    serv_addr.sin_addr.s_addr = htonl(INADDR_ANY);
    //绑定套接口到本地协议地址
    if (bind(listen_fd, (struct sockaddr *) &serv_addr, sizeof(serv_addr)) < 0)
    {
        printf("bind() error!");
        exit(0);
    }
    //服务器开始监听
    if (listen(listen_fd, 5) < 0)
    {
        printf("listen() error!");
        exit(0);
    }
    client[0].fd = listen_fd;
    client[0].events = POLLRDNORM;//关心监听套机字的读事件
    for (i = 1; i < OPEN_MAX; ++i)
    {
        client[i].fd = -1;
    }
    maxi = 0;
    while (true)
    {
        n_ready = poll(client, maxi + 1, -1);
        if (client[0].revents & POLLRDNORM)
        {
            cli_len = sizeof(cli_addr);
            // accept 的后面两个参数都是值-结果参数，他们的保留的远程连接电脑的信息，如果不管新远程连接电脑的信息，可以将这两个参数设置为 NULL
            conn_fd = accept(listen_fd, (struct sockaddr *) &cli_addr, &cli_len);
            if (conn_fd < 0)
            {
                break;
            }
            for (i = 1; i < OPEN_MAX; ++i)
            {
                if (client[i].fd < 0)
                {
                    client[i].fd = conn_fd;
                }
                break;
            }
            client[i].events = POLLRDNORM;
            if (i > maxi)
            {
                maxi = i;
            }
            if (--n_ready <= 0)
                continue;
        }
        for (i = 1; i < OPEN_MAX; ++i)
        {
            if ((sock_fd = client[i].fd) < 0)
            {
                continue;
            }
            if (client[i].revents & POLLRDNORM | POLLERR)
            {
                if ((n = read(sock_fd, buf, MAX_LINE)) < 0)
                {
                    if (errno == ECONNRESET)
                    {
                        close(sock_fd);
                        client[i].fd = -1;
                    } else
                    {
                        printf("read error!\n");
                    }
                } else if (n == 0)
                {
                    close(sock_fd);
                    client[i].fd = -1;
                } else
                {
                    write(sock_fd, buf, n);
                }
                if (--n_ready <= 0)
                {
                    break;
                }
            }
        }
    }
}