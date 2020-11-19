/**
* 作者：刘时明
* 时间：2020/11/15 0015
*/
#ifndef PROJECT4CPP_MAIL_LIST_H
#define PROJECT4CPP_MAIL_LIST_H

#include <string>
#include <iostream>

#define FILE_PATH = "data.bin"
#define STR1(R)  #R
#define STR2(R)  STR1(R)
#define GUID_LEN 64

void add_list();

void show_list();

void delete_list();

void search();

std::string generate_uuid();

class Mail
{
public:
    char id[GUID_LEN];
    char name[9];
    char phone[11];
};

typedef Mail DATA;

#endif //PROJECT4CPP_MAIL_LIST_H
