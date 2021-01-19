//
// Created by 刘时明 on 2021/1/9.
//
#ifndef FASTSQL_CONFIG_H
#define FASTSQL_CONFIG_H

#include <mutex>
#include <atomic>

class FastConfig
{
private:
    FastConfig() = default;

    FastConfig(FastConfig &) = delete;

    FastConfig &operator=(const FastConfig &) = delete;

    static FastConfig *instance;

    static std::mutex m_mutex;

    std::atomic<int> load_version;

public:
    static FastConfig *getInstance();

    void load();
};

#endif //FASTSQL_CONFIG_H
