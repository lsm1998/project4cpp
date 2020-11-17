/**
* 作者：刘时明
* 时间：2020/11/13 0013
*/
#ifndef PROJECT4CPP_MAIN_H
#define PROJECT4CPP_MAIN_H

#include <cstdlib>
#include <cstdio>
#include <cstring>
#include <iostream>
#include <istream>

#include "list.h"
#include "mailList.h"

using namespace std;

struct date
{
    char no[9];
    char tele[12];
};

typedef struct node
{
    //为了能够读写结构体，将这里定义的结构体改一下
    struct date info;
    struct node *next;
} NODE;

#endif //PROJECT4CPP_MAIN_H
