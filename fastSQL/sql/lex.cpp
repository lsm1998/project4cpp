//
// Created by 刘时明 on 2021/1/18.
//
#include <lex/lex.h>
#include <utils/string_utils.h>
#include <log.h>

std::list<std::string> *sqlTokenizer(const std::string &sql)
{
    using namespace utils;
    auto *sql_list = stringList(sql);
    auto *list = new std::list<std::string>();
    int current = 0;
    // 当前迭代的字符串
    std::string current_str;
    // 字符串拼接
    std::stringbuf append_str{};
    while (current < sql_list->size())
    {
        current_str = sql_list->at(current);
        // 是否字符
        if (isChar(current_str))
        {
            append_str.str("");
            while (isCharOrDigit(current_str))
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
                    while (isCharOrDigit(current_str))
                    {
                        append_str.sputn(current_str.c_str(), current_str.size());
                        current_str = sql_list->at(++current);
                    }
                }
            }
            // 可能是函数调用
            if (current_str == "(")
            {
                append_str.sputn("(", 1);
                // 找下一个')'
                bool flag = false;
                while (current < sql_list->size())
                {
                    current_str = sql_list->at(++current);
                    if (current_str == ")")
                    {
                        append_str.sputn(")", 1);
                        current++;
                        flag = true;
                        break;
                    }
                    append_str.sputn(current_str.c_str(), current_str.size());
                }
                if (!flag)
                {
                    ERROR_PRINT_EXIT("Syntax error, '(' need ')'. \n");
                }
            }
            list->push_back(toUp(append_str.str()));
            continue;
        }
        // 是否数字
        if (isDigit(current_str))
        {
            append_str.str("");
            while (isDigit(current_str))
            {
                append_str.sputn(current_str.c_str(), current_str.size());
                current_str = sql_list->at(++current);
            }
            list->push_back(toUp(append_str.str()));
            continue;
        }
        // 是否空格
        if (isSpaceChar(current_str))
        {
            current++;
            continue;
        }
        // 结束符
        if (current_str == ";")
        {
            break;
        }

        // 特殊符号
        if (current_str == "(" || current_str == ")" || current_str == "[" || current_str == "]"
            || current_str == "," || current_str == "*" || current_str == "=")
        {
            list->push_back(current_str);
            current++;
        } else if (current_str == "'")
        {
            append_str.str("");
            append_str.sputn("'", 1);
            std::string last_str = current_str;
            while (true)
            {
                current_str = sql_list->at(++current);
                if (current_str == "'" && last_str != "\\")
                {
                    append_str.sputn(current_str.c_str(), current_str.size());
                    list->push_back(append_str.str());
                    break;
                }
                append_str.sputn(current_str.c_str(), current_str.size());
                last_str = current_str;
            }
            current++;
        } else if (current_str == ">")
        {
            // 后面是否有 =
            if (sql_list->at(current + 1) == "=")
            {
                list->push_back(">=");
                current += 2;
            } else
            {
                list->push_back(">");
                current++;
            }
        } else if (current_str == "<")
        {
            // 后面是否有 =
            if (sql_list->at(current + 1) == "=")
            {
                list->push_back("<=");
                current += 2;
            } else
            {
                list->push_back("<");
                current++;
            }
        } else if (current_str == "!")
        {
            if (sql_list->at(current + 1) != "=")
            {
                ERROR_PRINT_EXIT("Syntax error, '!' must be before '='. \n");
            }
            list->push_back("!=");
            current += 2;
        }
    }
    return list;
}