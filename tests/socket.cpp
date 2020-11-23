//
// Created by 刘时明 on 2020/11/21.
//
#include "socket.h"

web::ServerSocket::ServerSocket(int port)
{
    this->port = port;
}

web::Socket *web::ServerSocket::accept_client()
{
    struct sockaddr_in client{};
    socklen_t len = sizeof(client);
    //接受连接
    auto *result = new web::Socket;
    result->conn_fd = accept(sock_fd, (struct sockaddr *) &client, &len);
    return result;
}

int web::ServerSocket::bind_addr()
{
    return this->bind_addr(this->port);
}

int web::ServerSocket::bind_addr(int port)
{
    this->server_addr.sin_addr.s_addr = htonl(INADDR_ANY);
    this->server_addr.sin_family = AF_INET;
    this->server_addr.sin_port = htons(port);
    this->sock_fd = socket(AF_INET, SOCK_STREAM, 0);
    if (this->sock_fd < 0)
    {
        return 0;
    }
    //绑定
    int ret = bind(sock_fd, (struct sockaddr *) &server_addr, sizeof(server_addr));
    if (ret < 0)
    {
        return 0;
    }
    //监听
    ret = listen(sock_fd, 10);
    if (ret < 0)
    {
        return 0;
    }
}

int web::Socket::connect_tcp(int port)
{
    const char* ip="127.0.0.1";
    return this->connect_tcp(ip,port);
}

int web::Socket::connect_tcp(const char *ip, int port)
{
    this->conn_fd = socket(AF_INET,SOCK_STREAM,0);

    if(this->conn_fd < 0)
    {
        perror("sock\n");
        return 0;
    }
    conn_addr.sin_family = AF_INET;
    conn_addr.sin_port = htons(port);
    conn_addr.sin_addr.s_addr = inet_addr(ip);

    return 0;
}
