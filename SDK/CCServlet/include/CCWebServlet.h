#ifndef CCWeb_Servlet_H
#define CCWeb_Servlet_H
#pragma once


#include "CCResponse.h"
#include "CCTimeData.h"
#define CC_ServletFunClass(Fun) [this](auto && PH1, auto && PH2) { Fun(std::forward<decltype(PH1)>(PH1), std::forward<decltype(PH2)>(PH2)); }
#define CC_ServletFun(Fun) Fun


class CCWebServlet:public CCResponse
{
private:
    using RequestFunc = std::function<void(CCRequest& ,CCResponse&)>;
    std::map<std::string,RequestFunc> RequestFun;
    CCString ServerIP;
    CCSocket m_Socket;
    int numThreads = 5,ListenMax = 50,RootLen = 0;
    bool FlagRun = true;
    CCThread m_Thread;
    std::vector<std::string> PathSix;
    CORS CORSConfig;
    std::mutex m_Mutex;
    std::function<void()> WebSocketSignal;
    RequestFunc WebSignalFun;
    CCThreadPool m_ThreadPool;
public:

public:
    CCWebServlet() = default;
    void SetThreadNumber(int headcount);
    void SetWebServlet(CCString UrlPath,RequestFunc RFunc);
    void SetAddRootPath(const CCString& rootPath);
    void SetWebServletFun(RequestFunc RFunc);
    bool Sign() const;
    bool Start(CCString IP,int port);
    void Close();
    void SetCorsConfig(CORS& cors);
    template<typename Fun>
    void SetWebSocketSignalSlots(Fun && fun);
    CCThreadPool GetServletThreadPool();

private:
    void ProcessRequest();
    void ResponseData(CCSocket & socket);
    CCString GetFileHeaders(CCString& str,CCSocket &socket,std::map<CCString, CCString> &map);





};


#endif
