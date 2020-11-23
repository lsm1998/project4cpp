//
// Created by 刘时明 on 2020/11/22.
//
#include <thread>
#include "sever.h"

void handle(web::Socket *socket)
{
    printf("socket=%p \n", socket);
    while (true)
    {
        int iRet;
        unsigned char buf[256];
        iRet = recv(socket->conn_fd, buf, sizeof(buf), 0);
        buf[iRet] = 0;
        if (iRet < 0)
        {
            printf("recv data failed\n");
            break;
        }
        printf("%s\n",buf);
        write(socket->conn_fd, buf, iRet);
    }
    delete socket;
}

int main()
{
    auto *serverSocket = new web::ServerSocket;
    serverSocket->bind_addr(8000);
    while (true)
    {
        auto *client = serverSocket->accept_client();
        printf("socket=%p", socket);
        if (client->conn_fd == 0)
        {
            break;
        }
        std::thread t(handle, client);
        t.join();
    }
    return 0;
}