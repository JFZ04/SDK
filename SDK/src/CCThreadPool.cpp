#include "CCThreadPool.h"

CCThreadPool::CCThreadPool(int numThreads){
    InitStart(numThreads);
}

void CCThreadPool::InitStart(int numThreads){
    InitStart(numThreads,30,1000 * 1000);
}

void CCThreadPool::InitStart(int corePoolSize, int maximumPoolSize, int keepAliveTime) {
    m_corePoolSize = corePoolSize;
    m_keepAliveTime = keepAliveTime;
    m_maximumPoolSize = maximumPoolSize;
    for (int i = 0; i < m_corePoolSize; ++i) {
        TaskPool pool;
        pool.ID = i;
        pool.Sign = true;
        m_threads[i] = pool;
        CCThread(&CCThreadPool::worker,this,i).detach();
    }
}

void CCThreadPool::worker(int ID){
    auto &T = m_threads[ID];
    T.Online = true;
    while (T.Sign){
        try {
            m_mutex.lock();
            if(m_taskQueues.empty()){
                m_mutex.unlock();
                continue;
            }
            auto task = m_taskQueues.front();
            if(task){
                m_taskQueues.pop_front();
                m_mutex.unlock();
                m_threads[ID].Running = true;
                TaskRunningSize++;
                if(Print){
                    CC::Println("CCThreadPool::worker Run ID:" + CC::to_String(ID));
                }
                task();
                if(Print){
                    CC::Println("CCThreadPool::worker Finish ID:" + CC::to_String(ID));
                }
                TaskRunningSize--;
                m_threads[ID].Running = false;
            }
        }
        catch (CCException& e){
            String msg = "CCThreadPool::worker Exception:" + CC::to_String(e.what());
            CC::Println(msg);
        }
    }
    T.Online = false;
    if(Print){
        String msg = "CCThreadPool::worker Stop:" + CC::to_String(ID);
        CC::Println(msg);
    }
}

int CCThreadPool::GetThreadCount() const{
    return m_threads.size() + TemporaryThreads;
}

int CCThreadPool::GetUnusedCount() const{
    int A = (int) m_threads.size() + TemporaryThreads;
    return A - TaskRunningSize;
}

void CCThreadPool::SetThreadTimeout(int us){
    m_keepAliveTime = us;
}

void CCThreadPool::Stop(){
    for (auto& thread : m_threads) {
        thread.second.Sign = false;
        while (thread.second.Online);
        CCThread::Sleep(1000);
    }
    m_threads.clear();
}

CCThreadPool::CCThreadPool(CCThreadPool &other) {
    m_corePoolSize = other.m_corePoolSize;
    m_keepAliveTime = other.m_keepAliveTime;
    m_taskQueue = other.m_taskQueue;
    m_taskQueues = other.m_taskQueues;
    m_threads = other.m_threads;
}

int CCThreadPool::GetCorePoolSize() const {
    return m_corePoolSize;
}

int CCThreadPool::GetPoolMaxSize() const {
    return m_maximumPoolSize;
}

void CCThreadPool::SetPrint(bool F) {
    Print = F;
}

int CCThreadPool::GetTaskRunningSize() const {
    return TaskRunningSize;
}









