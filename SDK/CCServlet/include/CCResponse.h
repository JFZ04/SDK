#ifndef CCWeb_Response_H
#define CCWeb_Response_H
#pragma once

#include "CCRequest.h"
#include "../../include/CCSocket.h"
#include "CCString.h"
#include "mutex"
#include <openssl/sha.h>
#include <openssl/bio.h>
#include <openssl/evp.h>
#include <openssl/buffer.h>


class HTTPResponse: public CCRequest, public CORS {
public:
    HTTPResponse() = default;
    HTTPResponse(CCSocket& socket,CORS& cors);
    void HtmlTextWrite(const CCString& str);
    bool HtmlWrite(const CCString& str);
    void Write(const CCString& str,const char* type = "text/plain");
    void Write(char * data,size_t length);
    void HTTPWrite(const CCString& str);
    bool WriteMultipart(const CCString& Path);
    bool FileDownload(const CCString& Path);
    void Send(const CCString& string);
    void ResponseOK();
    void ResopenseWebSocket(const CCString& Key);
    void SetHeader(const CCString& Key,CCString Value);
    bool IsConnect();
    std::map<CCString,CCString> GetHeaders();

private:
    bool SendResources(const char* Path, const char* Mode = "rb");
    CCString base64_encode(const unsigned char *data, size_t length);
    CCString sha1(const CCString& input);
    CCSocket Client;
    std::mutex Mutex;
    CORS CORSConfig;
    std::map<CCString,CCString> Headers;
    std::streamoff getFileSize(const std::string& filePath);


private:


};


class CCResponse:public HTTPResponse
{
public:

public:
    CCResponse() = default;
    CCResponse(CCSocket& socket,CORS& cors);
    HTTPResponse GetWriter();

private:
    CCSocket Client;
    CORS Cors;
};


#endif
