#ifndef CCWeb_Request_H
#define CCWeb_Request_H
#pragma once
#include "CCString.h"
#include "map"
#include <sys/stat.h>
#include "CORS.h"
#include "../../include/CCJSONObject.h"
#include "../../include/CCFile.h"
#include "../../include/CCSocket.h"
#include "../../include/CCThread.h"
#include "../../include/CCThreadPool.h"
#include "CCObject.h"
#include "../../include/CCFileOutStream.h"
#define Buffer_Max 1024

class CCRequest;
enum RequestProcess
{
    RequestProcess_First,
    RequestProcess_Header,
    RequestProcess_Body,
};

struct BufferReader
{
    CCString Body;
    CCString Method;
    CCString Path;
    CCString Version;
    std::vector<char> Buffer;
};

struct BufferFile
{
    BufferFile() = default;
    std::vector<char> Buffer;
    std::map<CCString,CCString> Headers;
    CCString GetFileName();
    bool Save(const CCString& Path);
};

enum MethodType
{
    MethodType_GET,
    MethodType_POST,
    MethodType_PUT,
    MethodType_DELETE,
    MethodType_HEAD,
    MethodType_OPTIONS,
};

class OutPutStream
{
private:
    CCString RMethod = "GET";
    CCString RPath,RBody;
    std::map<CCString,CCString> SendHeaders = {{"Method",RMethod}};
    CCSocket Socket;

public:
    OutPutStream(CCSocket& sc);
    void SetMethod(MethodType method);
    void AddHeader(const CCString& Key,const CCString& Value);
    void SetHost(const CCString& host);
    void SetBufferBody(CCString body);
    void Send();
    CCSocket GetSocket();
};

class InPutStream:public CCFileOutStream
{
public:

private:
    std::vector<char> Buffer;
    BufferReader bufferReader;
    CCSocket Socket;
    std::map<CCString,CCString> FileHeaders;
    std::map<CCString,CCString> Headers;
public:
    InPutStream() = default;
    InPutStream(CCRequest* Res);
    void WriteBufferToFile(const CCString& Path);
    CCObject Get(CCString key);
private:
    void DeleteTailOf(CCString Path,CCString str);
    std::string readTailOfFile(const std::string& filepath, size_t bufferSize);
    bool filterAndPrint(const std::string& tail, const std::string& filter);
};


class CCRequest
{
private:
    BufferReader Buffer;

public:
    std::map<CCString,CCString> Headers;
    std::map<CCString,CCString> FileHeaders;
public:
    CCRequest() = default;
    CCRequest(CCSocket& sc);
    CCString GetFileSuffix(const CCString& filePath);
    CCString GetFileType(const CCString& str);
    unsigned int GetFileSize(const char* path);
    void SetBuffer(BufferReader& buffer);
    BufferReader GetReader();
    static JSON GetJson(const BufferReader& instr);
    BufferFile GetFile(BufferReader in);
    CCString GetParameter(CCString string);
    BufferReader ReaderFileData();
    OutPutStream GetWriter();
    void GetFormData(CCString& input,CCString& key,CCString& value);
    CCString GetFileDateHeader(BufferReader in,std::map<CCString,CCString>& map);
    CCString GetFileDateHeader(CCString& in,std::map<CCString,CCString>& map);
    std::vector<char> ReaderFormData(BufferReader in, std::map<CCString, CCString> &map);
    InPutStream GetInputStream();

private:
    std::map<std::string, std::string> parseKeyValuePairs(const std::string& input);
    CCSocket Socket;

};


#endif
