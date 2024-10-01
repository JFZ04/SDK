#ifndef CCEpoll_H
#define CCEpoll_H
#pragma once
#include "CCSocket.h"
#ifdef _WIN32

#elif __linux__

#endif
typedef int CCEpoll_t;


class CCEpoll
{
private:
    CCSocket listenSocket;
#ifdef _WIN32
    HANDLE hIoCompletionPort = nullptr;
    DWORD bytesTransferred{};
    ULONG_PTR completionKey{};
    OVERLAPPED* pOverlapped{};
#elif __linux__

#endif

public:
    CCEpoll() = default;
    CCEpoll(CCSocket& socket,int size = 10);
    void Create(CCSocket& socket,int size = 10);
    CCSocket Accept(CCSocket& socket);
    bool Status();
    bool CompletionKey();

private:


};


#endif
