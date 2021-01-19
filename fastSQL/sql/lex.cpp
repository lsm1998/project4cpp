//
// Created by 刘时明 on 2021/1/18.
//
#include <lex.h>
#include <string_utils.h>

std::list<std::string> *sql_tokenizer(const std::string &sql)
{
    auto *list = new std::list<std::string>();
    int current = 0;
    char c;
    std::stringbuf sb{};
    sb.str();
    sb.sputn("hello", 5);
    while (current < sql.length())
    {
        if (is_zh_ch(sql[current]))
        {
            std::cout << sql.substr(current, 3) << std::endl;
            current += 3;
        } else
        {
            std::cout << sql[current] << std::endl;
            current++;
        }
    }
    return list;
}