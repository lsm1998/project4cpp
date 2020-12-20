//
// Created by 刘时明 on 2020/12/20.
//
#include "main.h"

void tcp_listen(const int *sock_fd)
{
    char buff[MAX_BUFF_SIZE + 1];
    int n;
    while ((n = read(*sock_fd, buff, MAX_BUFF_SIZE)) > 0)
    {
        buff[n] = 0;
        printf("来自服务器的消息：%s \n", buff);
        if (n < 0)
        {
            printf("读取消息失败 \n");
            break;
        }
    }
}