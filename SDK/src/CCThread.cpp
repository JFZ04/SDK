#include "../include/CCThread.h"

#ifdef __CPP__


CCThread::CCThread()
{

}

CCThread::CCThread(std::thread a)
{

}


CCThread::CCThread(CCThread&& other)
	: Thread(std::move(other.Thread)), Flag(other.Flag)
{
	other.Flag = false;
}

CCThread::~CCThread()
{
	if (Thread.joinable())
	{
		Thread.join();
	}
}

void CCThread::Stop()
{
	Flag = false;
}

bool CCThread::Sign()
{
	return Flag;
}





#elif _WIN32
CCThread::CCThread()
{

}
CCThread::CCThread(CCThread&& other)
{


}
bool CCThread::SetThread(_In_ LPTHREAD_START_ROUTINE lpStartAddress, _In_opt_ __drv_aliasesMem LPVOID lpParameter, _Out_opt_ LPDWORD lpThreadId)
{
	hThread = CreateThread(nullptr, 0, lpStartAddress, lpParameter, CREATE_SUSPENDED, lpThreadId);
	if (hThread!=NULL)
		return true;
	else
		return false;

}
bool CCThread::Start()
{
	Flag = true;
	dwaid = ResumeThread(hThread); //启动线程
	if (dwaid == 1)
	{
		return true;
	}
	else
	{
		Flag = false;
		return false;
	}

}

bool CCThread::Stop()
{
	Flag = false;
	auto a = CloseHandle(hThread);
	if(a == 1)
	{
		TerminateThread(hThread, dwaid);
		return true;
	}
	else
	{
		return false;
	}
}

bool CCThread::Sign()
{
	return Flag;

}


#elif __linux__

CCThread::CCThread()
{

}

CCThread::CCThread(CCThread&& other)
{

}

bool CCThread::SetThread(ThreadFuns funs,FUNA arg)
{
    Funas = funs;
    agsa = arg;
    if(Funas!=NULL)
    {
        return true;
    }
    return false;
}

bool CCThread::Start()
{
    this->Flag = true;
    int err = pthread_create(&Thread, NULL, Funas,agsa);
    if(err!=0)
    {
        return true;
    }
    return false;
}


bool CCThread::Stop()
{
    this->Flag = false;
    return true;
}

bool CCThread::Sign()
{
    if(Thread == 0)
    {
        this->Flag = false;
    }
    return this->Flag;
}

#endif





