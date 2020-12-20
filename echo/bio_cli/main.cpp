//
// Created by 刘时明 on 2020/12/20.
//

#include "main.h"

int main()
{
    int sock_fd;
    struct sockaddr_in serv_addr{};

    sock_fd = socket(AF_INET, SOCK_STREAM, 0);
    if (sock_fd < 0)
    {
        printf("创建socket失败 \n");
        return sock_fd;
    }
    memset(&serv_addr, 0, sizeof(serv_addr));
    serv_addr.sin_family = AF_INET;
    serv_addr.sin_port = htons(PORT);

    if (connect(sock_fd, (struct sockaddr *) (&serv_addr), sizeof(serv_addr)) < 0)
    {
        printf("连接服务器失败 \n");
        return -1;
    }

    std::thread t(tcp_listen, &sock_fd);
    t.detach();

    while (true)
    {
        printf("输入消息： \n");
        char cmd[MAX_BUFF_SIZE];
        scanf("%s", cmd);
        if (strcmp(cmd, "exit") == 0)
        {
            break;
        }
        write(sock_fd, cmd, strlen(cmd) + 1);
    }
    return 0;
}