//
// Created by 刘时明 on 2021/1/19.
//
#include <string_utils.h>

bool is_zh_ch(char c)
{
    return !~(c >> 8);
}

std::string sub(std::string str, int start, int end)
{
    if (typeid(str) == typeid(std::string) && str.length() > 0)
    {
        int len = str.length();
        std::string tmp = "";
        std::vector<std::string> dump;
        int i = 0;
        while (i < len)
        {
            if (is_zh_ch(str.at(i)) == 1)
            {
                dump.push_back(str.substr(i, 2));
                i = i + 2;
            } else
            {
                dump.push_back(str.substr(i, 1));
                i = i + 1;
            }
        }
        end = end > 0 ? end : dump.size();
        if (start < 0 || start > end)
        {
            printf("方法调用错误，第一个参数应该大于等于0且小于第二个参数！\n");
        }
        for (int i = start; i <= end; i++)
        {
            tmp += dump[i - 1];
        }
        return tmp;
    } else
    {
        printf("请输入字符串！\n");
        return "";
    }
}