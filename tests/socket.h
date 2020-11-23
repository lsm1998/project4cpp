//
// Created by 刘时明 on 2020/11/21.
//

#ifndef PROJECT4CPP_SOCKET_H
#define PROJECT4CPP_SOCKET_H

#include <netinet/in.h>
#include <unistd.h>
#include <cstdio>
#include <arpa/inet.h>

namespace web
{
    class Socket
    {
    private:
        struct sockaddr_in conn_addr;

    public:
        int conn_fd;

        Socket() = default;

        int connect_tcp(const char *ip,int port);

        int connect_tcp(int port);
    };

    class ServerSocket
    {
    private:
        int port;
        struct sockaddr_in server_addr{};
        int sock_fd;

    public:
        ServerSocket() = default;

        ServerSocket(int port);

        int bind_addr();

        int bind_addr(int port);

        Socket *accept_client();
    };
}

#endif //PROJECT4CPP_SOCKET_H
