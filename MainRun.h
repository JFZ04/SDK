#ifndef THREADPOOLDEMO_MAINRUN_H
#define THREADPOOLDEMO_MAINRUN_H




#include "CCWebServlet.h"
#include "CCSQLite3.h"

class MainRun
{
public:
    bool Init();
    int Run();
    CCWebServlet m_servlet;
private:
    const CCString Paths = "../static/File";
    const CCString BuffPaths = "../static/Buffer";
    std::mutex mutex;

public:
    void WebSocketFun();
    void Roots(CCRequest& req,CCResponse& res);
    void Ping(CCRequest& req,CCResponse& res);
    void login(CCRequest& request,CCResponse& response);
    void Home25(CCRequest& request,CCResponse& response);
    void FileAdd(CCRequest& request,CCResponse& response);
    void File(CCRequest& request,CCResponse& response);
    void FileDelete(CCRequest& request,CCResponse& response);
    void FolderAdd(CCRequest& request,CCResponse& response);
    void ResFile(CCRequest& request,CCResponse& response);
    void FileDeleteD(CCRequest& request,CCResponse& response);
    void NoServletFun(CCRequest& request,CCResponse& response);
    void FileDownload(CCRequest& request,CCResponse& response);

};


#endif
