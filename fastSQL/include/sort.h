//
// Created by 刘时明 on 2021/1/20.
//

#ifndef PROJECT4CPP_SORT_H
#define PROJECT4CPP_SORT_H

template<typename T>
class Comparable
{
public:
    virtual int compareTo(T o) = 0;
};

#endif //PROJECT4CPP_SORT_H
