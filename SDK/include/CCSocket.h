#define  __CCSocket__
#ifdef   __CCSocket__
#pragma once

#include "iostream"
#include "string"

struct CCHostInfo
{
    std::string IPAddress;
    unsigned short Port;
};

#ifdef _WIN32

#include <winsock2.h>
#include <ws2tcpip.h>
#include <mswsock.h>

#pragma warning(disable : 4996)
#pragma comment(lib,"ws2_32.lib")     //<2F><><EFBFBD> 0 3<EFBFBD><D3BF> 0 4<EFBFBD>

typedef long long ByteHander;
#define HanderSize sizeof(ByteHander)


enum IPVX
{
 	IPV4 = AF_INET,
	IPV6 = AF_INET6
};
enum TORU
{
	TCP = IPPROTO_TCP,
	UDP = IPPROTO_UDP
};
enum TYPE
{
	STREAM = SOCK_STREAM,
	DGRAM = SOCK_DGRAM
};



class CCSocket
{
public:
    SOCKET Socketbit = -1;

public:
	CCSocket() = default;
	CCSocket Socket(IPVX IPV4orIPV6, TORU TCPorUDP, TYPE Type = TYPE::STREAM);
    void SetSocketNonBlocking();
    bool IsDataAvailable();
    std::string GetlocadIP(IPVX ipvx = IPVX::IPV4,int Number = 1);
    CCHostInfo GetHost();
	bool Connect(const char* IP, unsigned short Port);
	bool Bind(const char* IP,unsigned short Port);
	bool Listen(unsigned short UserNum = 10);
    CCSocket Accept();
	bool Send(const char* str);
    bool Send(const void *__buf, size_t __n, int __flags);
	bool SendByte(const char* str,int len);
	bool UDPSend(const char* str,const char* IP,int Port);
	bool UDPSendByte(const char* str, ByteHander len, const char* IP, int Port);
	ByteHander RecvData(char* buffer,ByteHander lens);
	ByteHander UDPRecvData(char* buffer,ByteHander lens,CCHostInfo* info);
	void Close();
	bool GetDateHead(char* data, ByteHander* size);
	bool GetStrHead(char* data,ByteHander size);
    bool operator == (CCSocket socket);
    bool operator != (CCSocket socket);
    bool isConnectionAlive();

private:
	WSADATA wsd;
    sockaddr_in client = {};
	int IPVx = 0;
	int sock = -1;
    bool Stop = false;

};




#elif __linux__
#include <sys/socket.h>
#include <netinet/in.h>
#include <linux/if.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <fcntl.h>
#include <ifaddrs.h>
#include <netdb.h>
#include <sys/poll.h>

typedef int SOCKET;
typedef long long ByteHander;
#define HanderSize sizeof(ByteHander)


enum IPVX
{
    IPV4 = AF_INET,
    IPV6 = AF_INET6
};
enum TORU
{
    TCP = IPPROTO_TCP,
    UDP = IPPROTO_UDP
};
enum TYPE
{
    STREAM = SOCK_STREAM,
    DGRAM = SOCK_DGRAM
};

class CCSocket
{
public:
    SOCKET Socketbit = -1;
public:
    CCSocket() = default;
	CCSocket Socket(IPVX IPV4orIPV6, TORU TCPorUDP, TYPE Type = TYPE::STREAM);
    void SetSocketNonBlocking();
    bool IsDataAvailable();
    std::string GetlocadIP(IPVX ipvx = IPVX::IPV4,int Number = 1);
    CCHostInfo GetHost();
    bool Connect(const char* IP, unsigned short Port);
    bool Bind(const char* IP,int Port);
    bool Listen(unsigned short UserNum = 10);
    CCSocket Accept();
    bool Send(const char* str);
    bool Send(const void *__buf, size_t __n, int __flags);
    bool SendByte(const char* str,int len);
    bool UDPSend(const char* str,const char* IP,int Port);
    bool UDPSendByte(const char* str, ByteHander len, const char* IP, int Port);
    ByteHander RecvData(char* buffer,ByteHander lens);
    ByteHander UDPRecvData(char* buffer,ByteHander lens,CCHostInfo* info);
    bool GetDateHead(char* data, ByteHander* size);
    bool GetStrHead(char* data,ByteHander size);
    void Close();
    //----------------------------------------------------------------------------------------
    bool operator == (CCSocket socket);
    bool operator != (CCSocket socket);
    bool isConnectionAlive();


private:
    sockaddr_in client = {};
	int IPVx = 0;
    int opt = 1;
    bool Stop = false;






};








#else




#endif


#endif
