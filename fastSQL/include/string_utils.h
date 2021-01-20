//
// Created by 刘时明 on 2021/1/19.
//

#ifndef PROJECT4CPP_STRING_UTILS_H
#define PROJECT4CPP_STRING_UTILS_H

#include <string>
#include <vector>

bool is_zh_ch(char c);

std::vector<std::string>* string_list(const std::string& str);

bool is_char(const std::string &str);

bool is_digit(const std::string &str);

bool is_char_or_digit(const std::string &str);

bool is_space_char(const std::string &str);

#endif //PROJECT4CPP_STRING_UTILS_H
