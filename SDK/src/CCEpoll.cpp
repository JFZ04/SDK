#include "CCEpoll.h"

CCEpoll::CCEpoll(CCSocket& socket,int size) {
    Create(socket,size);
}

void CCEpoll::Create(CCSocket &socket, int size) {
#ifdef _WIN32
    this->hIoCompletionPort = CreateIoCompletionPort(INVALID_HANDLE_VALUE, NULL, 0, 0);
    CreateIoCompletionPort((HANDLE)socket.Socketbit, hIoCompletionPort, (ULONG_PTR)socket.Socketbit, 0);
#elif __linux__

#endif
}

CCSocket CCEpoll::Accept(CCSocket &socket) {
#ifdef _WIN32
    listenSocket = socket;
    CCSocket clientSocket;
    if (completionKey == (ULONG_PTR)listenSocket.Socketbit) {
        clientSocket = socket.Accept();
        CreateIoCompletionPort((HANDLE)clientSocket.Socketbit, hIoCompletionPort, (ULONG_PTR)clientSocket.Socketbit, 0);
    }
    return clientSocket;
#elif __linux__

#endif

}

bool CCEpoll::Status() {
#ifdef _WIN32
    return GetQueuedCompletionStatus(hIoCompletionPort, &bytesTransferred, &completionKey, &pOverlapped, INFINITE);
#elif __linux__

#endif
}

bool CCEpoll::CompletionKey() {
#ifdef _WIN32
    return completionKey == (ULONG_PTR)listenSocket.Socketbit;
#elif __linux__

#endif
}
