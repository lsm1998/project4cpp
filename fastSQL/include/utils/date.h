//
// Created by 刘时明 on 2021/1/20.
//

#ifndef PROJECT4CPP_DATE_H
#define PROJECT4CPP_DATE_H

#include <ctime>
#include <string>

class Date
{
private:
    long t;

public:
    static Date *now();

    Date() = default;

    explicit Date(long time);

    [[nodiscard]] long uniTime() const;

    [[nodiscard]] std::string format(const std::string &format) const;
};

#endif //PROJECT4CPP_DATE_H
