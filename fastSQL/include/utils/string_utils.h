//
// Created by 刘时明 on 2021/1/19.
//

#ifndef PROJECT4CPP_STRING_UTILS_H
#define PROJECT4CPP_STRING_UTILS_H

#include <string>
#include <vector>

namespace utils
{
    bool isZhCh(char c);

    std::vector<std::string> *stringList(const std::string &str);

    std::string toUp(const std::string &str);

    bool isChar(const std::string &str);

    bool isDigit(const std::string &str);

    bool isCharOrDigit(const std::string &str);

    bool isSpaceChar(const std::string &str);
}

#endif //PROJECT4CPP_STRING_UTILS_H
