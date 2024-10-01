#ifndef __CC_ThreadPool_H__
#define __CC_ThreadPool_H__
#pragma once

#include "iostream"
#include <functional>
#include "deque"
#include <mutex>
#include <vector>
#include <map>
#include "CCThread.h"
#include "CC.h"

class CCThreadPool
{
public:
    struct TaskPool {
        bool Running = false;
        int ID = -1;
        bool Sign = false;
        bool Online = false;
    };

public:
    CCThreadPool() = default;
    explicit CCThreadPool(int numThreads);
    CCThreadPool(CCThreadPool& other);
    void InitStart(int corePoolSize);
    void InitStart(int corePoolSize, int maximumPoolSize,int keepAliveTime);
    template<typename Func,typename... Args>
    void AddTask(Func && func, Args&&... args);
    int GetThreadCount() const;
    int GetUnusedCount() const;
    int GetCorePoolSize() const;
    int GetPoolMaxSize() const;
    int GetTaskRunningSize() const;
    void SetPrint(bool F);
    void SetThreadTimeout(int us);
    void Stop();


private:
    void worker(int ID);
    template<typename Func,typename... Args>
    void AdHocTasks(Func && func, Args&&... args);
    std::vector<std::function<void()>> m_taskQueue;
    std::deque<std::function<void()>> m_taskQueues;
    std::map<int, TaskPool> m_threads;
    std::mutex m_mutex;
    int m_corePoolSize = 5, m_maximumPoolSize = 30, m_keepAliveTime = 1000 * 1000;
    int TaskRunningSize = 0,TemporaryThreads = 0;
    bool Print = false;
};
template<typename Func,typename... Args>
inline void CCThreadPool::AddTask(Func && func, Args&&... args) {
    if(TaskRunningSize >= m_maximumPoolSize){
        CC::Println("CCThreadPool::AddTask: Threaded tasks exceed the maximum, wait!");
        return;
    }
    std::function<void()> FUNS = std::bind(std::forward<Func>(func), std::forward<Args>(args)...);
    if(FUNS){
        if(GetUnusedCount() <= m_corePoolSize){
            m_mutex.lock();
            m_taskQueues.push_front(FUNS);
            m_mutex.unlock();
        }
        else {
            CCThread([this,FUNS](){
                AdHocTasks(FUNS);
            }).detach();
        }
    }
    else {
        CC::Println("CCThreadPool::AddTask: The task is empty!");
    }
}

template<typename Func, typename... Args>
inline void CCThreadPool::AdHocTasks(Func &&func, Args &&... args) {
    TemporaryThreads++;
    try {
        func();
    }
    catch (CCException& e) {
        String str = "CCThreadPool::AdHocTasks: The temporary task failed -> " + CC::to_String(e.what());
        CC::Println(str);
    }
    TemporaryThreads--;
    CCThread::Sleep(m_keepAliveTime);
}

#endif
