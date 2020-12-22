//
// Created by 刘时明 on 2020/12/22.
//

// 定义缓存块大小为10MB
#define AOF_RW_BUF_BLOCK_SIZE 1024*1024*10

typedef struct AofRwbLock
{
    // 缓存块已使用字节数和可用字节数
    unsigned long used, free;
    // 缓存块
    char buf[AOF_RW_BUF_BLOCK_SIZE];
} AofRwbLock;