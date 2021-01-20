//
// Created by 刘时明 on 2021/1/20.
//

#ifndef PROJECT4CPP_ROW_H
#define PROJECT4CPP_ROW_H

#include <sort.h>

template<typename T>
class Row : public Comparable<T>
{
public:
    int compare_to(T o);
};


#endif //PROJECT4CPP_ROW_H
