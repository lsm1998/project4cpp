//
// Created by 刘时明 on 2020/12/20.
//

#ifndef PROJECT4CPP_MAIN_H
#define PROJECT4CPP_MAIN_H

#include <cstdio>
#include <arpa/inet.h>
#include <unistd.h>

#define PORT 8090

#define MAX_BUFF_SIZE 4096

#define CONN_BUFF_SIZE 10

void tcp_handler(sockaddr_in *conn_addr,int *conn_fd);

#endif //PROJECT4CPP_MAIN_H
