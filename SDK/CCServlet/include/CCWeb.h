#ifndef DEMO_CCWEB_H
#define DEMO_CCWEB_H

#include "../../include/CCSocket.h"
#include "../../include/CCThread.h"
#include "functional"
#include <sys/stat.h>
#include "map"
#include "mutex"
#include "nlohmann/json.hpp"
#include "fstream"
#include "sstream"

#define UserMAX 20
#define BufferSize 4096

enum RequestProcess
{
    RequestProcess_First,
    RequestProcess_Header,
    RequestProcess_Body,
};

struct HTTPRequestFile
{
    std::string Name;
    std::string Type;
    std::vector<char> Buffer;
    std::map<std::string,std::string> Headers;
    bool Save(std::string Path) const;
};


struct HTTPRequest
{
    unsigned int ID = 0;
    std::string Method;
    std::string Path;
    std::string Version;
    std::string Data;
    std::string Buffer;
    std::vector<char> Bufferstr;
    std::map<std::string,std::string> Headers;
    std::map<std::string,std::string> BodyData;
    HTTPRequestFile FileData;
};



class CCWeb
{
private:
    using RequestFunc = std::function<void(const HTTPRequest&,CCWeb*)>;
    std::map<std::string,RequestFunc> RequestFun;
    std::vector<std::string> PathSix;
    bool PrintBool = false;
    std::mutex mutexs;
    std::string BUFFER;

public:
    CCWeb() = default;
    //----------------------------------------------------------------
    HTTPRequest HTTPRequests;
    //----------------------------------------------------------------
    bool HTTPStart(const char * IP, int port);
    bool SendResources(unsigned int ID, const char* Path, const char* Mode = "rb");
    bool SendResourcesData(unsigned int ID, const char* Data);
    bool SendData(unsigned int ID, const char* Data,const char* type = "text/plain");
    std::string GetPath(const char* Path);
    std::string GetMethod(const char* str);
    std::string GetFileSuffix(const std::string& filePath);
    std::string GetFileType(const std::string& str);
    std::string GetData(std::string name,HTTPRequest request);
    std::string FilterString(const char * str,char Begin,char End);
    std::string GetRequestData(const char * str);
    std::string GetIP(IPVX port = IPVX::IPV4);
    std::string StrSplicing(const char *__restrict _format,...);
    void HTTPStop();
    void SetAddRootPath(const std::string& rootPath);
    void SetPrint(bool print);
    void SetRequestFunction(std::string RootPath,RequestFunc RFunc);
    unsigned int GetFileSize(const char* path);
    //----------------------------------------------------------------
    template<typename Data>
    std::string ReviseHTMLData(const char * Path, const char* Flags,Data&& data);
    template<typename Data>
    std::string ReviseData(const char * Date, const char* Flags,Data&& data);
    template<class name>
    name GetHTTPRequestData(const char * Date,std::string Request);
    std::string GetHTTPRequestData(const char * Date,std::string Request);
    std::string Getboundary(std::string& FileData);
    //----------------------------------------------------------------

private:
    std::string Filtration(const char * buf);
    std::string FiltrationJson(const char *buf);
    //----------------------------------------------------------------
    CCSocket Server,Client[UserMAX] = {  };
    CCThread Threadappect,Thread[UserMAX] = { };
    unsigned int ThreadCount = 0,lent = 0;
    CC_Thread Appect();
    CC_Thread ReadThread(unsigned int ID);


};
template<typename Data>
inline std::string CCWeb::ReviseHTMLData(const char *Path, const char *Flags, Data&& data)
{
    std::string str,Flag = std::string("{{") + Flags + "}}";
    std::ifstream file(Path,std::ios::in);
    if (file.is_open())
    {
        char buf[BufferSize] = {0};
        while (file.getline(buf,BufferSize))
        {
            str.append(buf);
            memset(buf,'\000',BufferSize);
        }
        file.close();
    }
    else
    {
        return "Not found file";
    }
    size_t i = str.find(Flag);
    if(i != std::string::npos)
    {
        str.replace(i,Flag.length(),data);
    }
    return str;
}

template<typename Data>
inline std::string CCWeb::ReviseData(const char *Date, const char *Flags, Data&& data)
{
    std::string str = Date,Flag = std::string("{{") + Flags + "}}";
    size_t i = str.find(Flag);
    if(i != std::string::npos)
    {
        str.replace(i,Flag.length(),data);
    }
    return str;
}

#endif
