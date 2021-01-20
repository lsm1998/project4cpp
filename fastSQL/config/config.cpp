//
// Created by 刘时明 on 2021/1/12.
//
#include <config.h>
#include <log.h>

FastConfig *FastConfig::instance = nullptr;
std::mutex FastConfig::m_mutex = std::mutex{};

FastConfig *FastConfig::getInstance()
{
    if (instance == nullptr)
    {
        m_mutex.lock();
        if (instance == nullptr)
        {
            instance = new FastConfig();
            instance->load_version = 0;
        }
        m_mutex.unlock();
    }
    return instance;
}

void FastConfig::load()
{
    if (instance->load_version.fetch_add(1) > 0)
    {
        INFO_PRINT("无需重复加载 \n");
        return;
    }
    INFO_PRINT("加载完毕 \n");
}

