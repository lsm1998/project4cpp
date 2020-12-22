#ifndef _PTHREAD_POOL_
#define _PTHREAD_POOL_

#include <queue>
#include <exception>
#include <iostream>

#include <mutex>

typedef std::mutex mutex_locker;

typedef std::condition_variable cond_locker;

template<class T>
class thread_pool
{
private:
    int thread_number;  //线程池的线程数
    //int max_task_number;  //任务队列中的最大任务数
    pthread_t *all_threads;   //线程数组
    std::queue<T *> task_queue; //任务队列
    mutex_locker queue_mutex_locker;  //互斥锁
    cond_locker queue_cond_locker; //cond
    bool is_stop; //是否结束线程
public:
    thread_pool(int thread_num = 20);

    ~thread_pool();

    bool append_task(T *task);

    void start();

    void stop();

private:
    //线程运行的函数。执行run()函数
    static void *worker(void *arg);

    void run();

    T *getTask();
};

template<class T>
thread_pool<T>::thread_pool(int thread_num):
        thread_number(thread_num), is_stop(false), all_threads(NULL)
{
    if (thread_num <= 0)
    {
        printf("threadpool can't init because thread_number = 0");
    }
    all_threads = new pthread_t[thread_number];
    if (all_threads == nullptr)
    {
        printf("can't init threadpool because thread array can't new");
    }
}

template<class T>
thread_pool<T>::~thread_pool()
{
    delete[]all_threads;
    stop();
}

template<class T>
void thread_pool<T>::stop()
{
    is_stop = true;
    queue_cond_locker.notify_all();
    // queue_cond_locker.broadcast();
}

template<class T>
void thread_pool<T>::start()
{
    for (int i = 0; i < thread_number; ++i)
    {
        if (pthread_create(all_threads + i, NULL, worker, this) != 0)
        {
            //创建线程失败，清除成功申请的资源并抛出异常
            delete[]all_threads;
            throw std::exception();
        }
        if (pthread_detach(all_threads[i]))
        {
            //将线程设置为脱离线程，失败则清除成功申请的资源并抛出异常
            delete[]all_threads;
            throw std::exception();
        }
    }
}

//添加任务进入任务队列
template<class T>
bool thread_pool<T>::append_task(T *task)
{   //获取互斥锁
    queue_mutex_locker.lock();

    bool is_signal = task_queue.empty();
    //添加进入队列
    task_queue.push(task);
    queue_mutex_locker.unlock();
    //唤醒等待任务的线程
    if (is_signal)
    {
        // queue_cond_locker.signal();
        queue_cond_locker.notify_one();
    }
    return true;
}

template<class T>
void *thread_pool<T>::worker(void *arg)
{
    thread_pool *pool = (thread_pool *) arg;
    pool->run();
    return pool;
}

template<class T>
T *thread_pool<T>::getTask()
{
    T *task = NULL;
    queue_mutex_locker.lock();
    if (!task_queue.empty())
    {
        task = task_queue.front();
        task_queue.pop();
    }
    queue_mutex_locker.unlock();
    return task;
}

template<class T>
void thread_pool<T>::run()
{
    while (!is_stop)
    {
        T *task = getTask();
        if (task == NULL)
        {
            std::unique_lock<mutex_locker> un(queue_mutex_locker);
            queue_cond_locker.wait(un);
        }
        else
        {
            task->doit();
            delete task;
        }
    }
}

#endif


