//
// Created by 刘时明 on 2021/1/19.
//
#include <string_utils.h>

namespace utils
{
    // 是否中文
    bool is_zh_ch(char c)
    {
        return !~(c >> 8);
    }

    // 字符串转单个字符列表，适用于utf8
    std::vector<std::string> *string_list(const std::string &str)
    {
        auto *list = new std::vector<std::string>();
        for (int i = 0; i < str.size();)
        {
            if (is_zh_ch(str[i]))
            {
                list->push_back(str.substr(i, 3));
                i += 3;
            } else
            {
                list->push_back(str.substr(i, 1));
                i++;
            }
        }
        return list;
    }

    // 字母转大写
    std::string to_up(const std::string &str)
    {
        char *c = const_cast<char *>(str.c_str());
        for (int i = 0; i < str.size(); ++i)
        {
            c[i] = toupper(c[i]);
        }
        return std::string(c);
    }

    // 是否字母
    bool is_char(const std::string &str)
    {
        if (str.size() != 1)
        {
            return false;
        }
        char c = str.at(0);
        return (c >= 'a' && c <= 'z') || (c >= 'A' && c <= 'Z');
    }

    // 是否数字
    bool is_digit(const std::string &str)
    {
        if (str.size() != 1)
        {
            return false;
        }
        char c = str.at(0);
        return c >= '0' && c <= '9';
    }

    // 是否字母或者数字
    bool is_char_or_digit(const std::string &str)
    {
        return is_char(str) || is_digit(str);
    }

    // 是否空格
    bool is_space_char(const std::string &str)
    {
        if (str.size() != 1)
        {
            return false;
        }
        char c = str.at(0);
        return c == ' ';
    }
}