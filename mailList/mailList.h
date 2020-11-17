/**
* 作者：刘时明
* 时间：2020/11/15 0015
*/
#ifndef PROJECT4CPP_MAIL_LIST_H
#define PROJECT4CPP_MAIL_LIST_H

#include <string>

using namespace std;

#define FILE_PATH = "data.bin"

class Mail
{
public:
    char no[10];
    char title[20];
};

typedef Mail DATA;

class MailList
{
private:

public:
    MailList() = default;

    void init();
};


#endif //PROJECT4CPP_MAIL_LIST_H
