//
// Created by 刘时明 on 2021/1/18.
//
#include <lex.h>
#include <string_utils.h>

std::list<std::string> *sql_tokenizer(const std::string &sql)
{
    auto *sql_list = string_list(sql);
    auto *list = new std::list<std::string>();
    int current = 0;
    std::string current_str;

    std::stringbuf append_str{};

    while (current < sql_list->size())
    {
        current_str = sql_list->at(current);
        // 是否字符
        if (is_char(current_str))
        {
            append_str.str("");
            while (is_char_or_digit(current_str))
            {
                // 只要以字母开头, 则获取到后面的所有字母或数字, 或者是点号, 如city.Name, 或者是.*, 如city.*
                append_str.sputn(current_str.c_str(), current_str.size());
                current_str = sql_list->at(++current);
            }
            if (current_str == ".")
            {
                append_str.sputn(current_str.c_str(), current_str.size());
                current_str = sql_list->at(++current);
                if (current_str == "*")
                {
                    append_str.sputn(current_str.c_str(), current_str.size());
                    current_str = sql_list->at(++current);
                } else
                {
                    while (is_char_or_digit(current_str))
                    {
                        append_str.sputn(current_str.c_str(), current_str.size());
                        current_str = sql_list->at(++current);
                    }
                }
            }
            list->push_back(append_str.str());
            continue;
        }
        // 是否数字
        if (is_digit(current_str))
        {
            append_str.str("");
            while (is_digit(current_str))
            {
                append_str.sputn(current_str.c_str(), current_str.size());
                current_str = sql_list->at(++current);
            }
            list->push_back(append_str.str());
            continue;
        }
        // 是否空格
        if (is_space_char(current_str))
        {
            current++;
            continue;
        }
        if (current_str == ";")
        {
            break;
        }
        // 特殊符号
        std::cout << current_str << std::endl;
    }
    return list;
}