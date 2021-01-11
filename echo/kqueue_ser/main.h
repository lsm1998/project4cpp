//
// Created by 刘时明 on 2021/1/11.
//

#ifndef PROJECT4CPP_MAIN_H
#define PROJECT4CPP_MAIN_H

#include <sys/socket.h>
#include <sys/event.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <fcntl.h>
#include <unistd.h>
#include <cstdio>
#include <cerrno>
#include <cstring>
#include <cstdlib>

#define exit_if(r, ...) if(r) {printf(__VA_ARGS__); printf("error no: %d error msg %s\n", errno, strerror(errno)); exit(1);}

#define PORT 8090

#endif //PROJECT4CPP_MAIN_H
