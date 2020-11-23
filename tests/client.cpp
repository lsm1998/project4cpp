//
// Created by 刘时明 on 2020/11/22.
//
#include "client.h"

int main()
{
    auto *socket=new web::Socket;

    socket->connect_tcp(8000);

    if(socket->conn_fd==0)
    {
        printf("connect_tcp error\n");
        return 0;
    }

    printf("socket->conn_fd=%d \n",socket->conn_fd);

    char buf[20]="hello";

    for (int i = 0; i < 1000; ++i)
    {
        write(socket->conn_fd,buf,20);
        printf("send once! \n");
        sleep(1);
    }
    return 0;
}
