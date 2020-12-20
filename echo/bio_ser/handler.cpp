//
// Created by 刘时明 on 2020/12/20.
//
#include "main.h"

void tcp_handler(sockaddr_in *conn_addr, int *conn_fd)
{
    while (true)
    {
        unsigned char buff[MAX_BUFF_SIZE];
        int iRet = recv(*conn_fd, buff, sizeof(buff), 0);
        if (iRet < 0)
        {
            printf("recv客户端失败 \n");
            return;
        }
        buff[iRet] = 0;

        printf("服务端收到消息：%s\n", buff);

        ssize_t result=write(*conn_fd, buff, iRet);
        printf("%zd \n",result);
    }
}