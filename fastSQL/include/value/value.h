//
// Created by 刘时明 on 2021/1/20.
//

#ifndef PROJECT4CPP_VALUE_H
#define PROJECT4CPP_VALUE_H

#include <sort.h>

typedef enum ValueType
{
    // 未知类型
    UNKNOWN = -1,
    // 空值
    NULL = 0,
    // 布尔类型
    BOOLEAN = 1,
    // byte
    BYTE = 2,
    // int
    INT = 3,
    // double
    Double = 4,
    // 字符串
    STRING = 5,
    // 二进制
    BLOB = 6,
    // 日期
    DATE = 7
} ValueType;

template<typename T>
class Value
{
public:
    virtual ValueType get_value_type() = 0;

    virtual Comparable<T> get_value() = 0;
};

#endif //PROJECT4CPP_VALUE_H
