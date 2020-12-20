//
// Created by 刘时明 on 2020/12/20.
//

#ifndef PROJECT4CPP_MAIN_H
#define PROJECT4CPP_MAIN_H

#include <cstdio>
#include <netinet/in.h>
#include <unistd.h>
#include <thread>
#include <cstring>

#define MAX_BUFF_SIZE 4096

#define PORT 8090

void tcp_listen(const int *sock_fd);

#endif //PROJECT4CPP_MAIN_H
