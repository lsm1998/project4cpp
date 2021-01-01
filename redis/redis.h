//
// Created by 刘时明 on 2020/12/23.
//

#ifndef PROJECT4CPP_REDIS_H
#define PROJECT4CPP_REDIS_H

typedef struct redisObject
{
    // 类型
    unsigned type: 4;

    // 编码
    unsigned encoding: 4;

    // 对象最后一次被访问的时间
    unsigned lru: REDIS_LRU_BITS;

    // 引用计数
    int refcount;

    // 指向实际值的指针
    void *ptr;

} rObj;

#endif //PROJECT4CPP_REDIS_H
