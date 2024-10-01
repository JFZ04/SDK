#ifndef  __CCThread__
#define  __CCThread__
#include <thread>
#include <functional>

namespace Threading
{
    inline void Sleep(unsigned int us)
    {
        if(us!=0)
            std::this_thread::sleep_for(std::chrono::microseconds(us));
        else
            while (true);
    };
};

#define __CPP__
#ifdef __CPP__



#pragma warning(disable : 4996)
#define C_ARG(A) std::ref(A)
#define CC_Thread void
#define FUNA void*


class CCThread:public std::thread
{
public:
    CCThread();
    CCThread(std::thread a);
	CCThread(CCThread&& other);
    using std::thread::thread;
    ~CCThread();
    template <typename Func, typename... Args>
    bool SetThread(Func&& fun, Args&&... args)
    {
        if (Thread.joinable()) {
            return false;
        }

        task = std::bind(std::forward<Func>(fun), std::forward<Args>(args)...);
        return true;
    }

    void Start()
    {
        if (task) {
            Flag = true;
            Thread = std::thread(task);
            Thread.detach();
        }
    }
	void Stop();
	bool Sign();
    static void Sleep(unsigned int us){
        if(us!=0)
            std::this_thread::sleep_for(std::chrono::microseconds(us));
        else{
            while (true);
        }
    };

private:
	std::thread Thread;
    std::function<void()> task;
	bool Flag = false;

};





#elif _WIN32
#include <windows.h>
#define CC_Thread DWORD WINAPI 
#define FUNA LPVOID

class CCThread
{
public:
    CCThread();
    CCThread(CCThread&& other);
    bool SetThread(
        _In_ LPTHREAD_START_ROUTINE lpStartAddress,
        _In_opt_ __drv_aliasesMem LPVOID lpParameter = NULL,
        _Out_opt_ LPDWORD lpThreadId = nullptr);
    bool Start();
    bool Stop();
    bool Sign();


private:
    HANDLE hThread;
    DWORD dwaid;
    bool Flag = false;




   
};




#elif __linux__

#include <functional>
#include <pthread.h>
#define CC_Thread void*
#define FUNA void*
typedef void* (*ThreadFuns)(void*);
class CCThread
{
public:
    CCThread();
    CCThread(CCThread&& other);
    bool SetThread(ThreadFuns funs,FUNA arg = NULL);
    bool Start();
    bool Stop();
    bool Sign();



private:
    pthread_t Thread;
    ThreadFuns Funas;
    FUNA agsa;
    bool Flag = false;





};



#endif




#endif 

