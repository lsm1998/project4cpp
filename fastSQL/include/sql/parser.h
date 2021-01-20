//
// Created by 刘时明 on 2021/1/20.
//

#ifndef PROJECT4CPP_PARSER_H
#define PROJECT4CPP_PARSER_H

#include <string>
#include <list>
#include "prepared.h"

class SqlParser
{
private:
    std::string currentToken = "";
    std::list<std::string> *tokens;

public:
    std::string next();

    bool checkEqual(const std::string& str);

    Prepared* parse(const std::string& sql);

    Prepared* parseSelect();

    Prepared* parseInsert();

    Prepared* parseUpdate();
};

#endif //PROJECT4CPP_PARSER_H
