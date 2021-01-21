//
// Created by 刘时明 on 2021/1/20.
//
#include <utils/date.h>

Date *Date::now()
{
    return new Date(time(nullptr));
}

Date::Date(long time)
{
    this->t = time;
}

long Date::uniTime() const
{
    return this->t;
}

std::string Date::format(const std::string &format) const
{
    char now[64];
    time_t tt = this->t;
    struct tm *t_time = localtime(&tt);
    strftime(now, 64, format.c_str(), t_time);
    return std::string(now);
}
