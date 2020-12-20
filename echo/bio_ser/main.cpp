//
// Created by 刘时明 on 2020/12/20.
//

#include "main.h"
#include <thread>

int main()
{
    struct sockaddr_in server_addr{};

    server_addr.sin_addr.s_addr = htonl(INADDR_ANY);
    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(PORT);

    int listen_fd = socket(AF_INET, SOCK_STREAM, 0);

    if (listen_fd < 0)
    {
        printf("创建socket失败 \n");
        return listen_fd;
    }

    int ret = bind(listen_fd, (struct sockaddr *) &server_addr, sizeof(server_addr));
    if (ret < 0)
    {
        printf("bind失败 \n");
        return ret;
    }

    // listen监听
    ret = listen(listen_fd, CONN_BUFF_SIZE);
    if (ret < 0)
    {
        printf("listen失败 \n");
        return ret;
    }

    printf("服务器启动完毕 \n");

    while (true)
    {
        struct sockaddr_in conn_addr{};
        int conn_fd = accept(listen_fd, nullptr, nullptr);

        printf("一个客户端连入！ \n");
        if (conn_fd < 0)
        {
            printf("接受客户端请求失败 \n");
            return conn_fd;
        }
        conn_addr.sin_family = AF_INET;
        conn_addr.sin_port = htons(PORT);
        conn_addr.sin_addr.s_addr = inet_addr("127.0.0.1");

        std::thread t(tcp_handler, &conn_addr, &conn_fd);
        t.detach();
    }
    return 0;
}