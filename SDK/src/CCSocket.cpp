#include "../include/CCSocket.h"

#include <cstring>

#ifdef _WIN32


CCSocket CCSocket::Socket(IPVX IPV4orIPV6, TORU TCPorUDP, TYPE Type)
{
    CCSocket sc;
    WSACleanup();
	if (WSAStartup(MAKEWORD(2, 2), &this->wsd)!=0)
	{
		WSACleanup();
		return sc;
	}
	this->Socketbit = socket(IPV4orIPV6, Type, TCPorUDP);
	if (this->Socketbit == INVALID_SOCKET)
	{
		WSACleanup();
		return sc;
	}
	this->sock = TCPorUDP;
	this->IPVx = IPV4orIPV6;
    sc.sock = this->sock;
    sc.IPVx = this->IPVx;
    sc.Socketbit = this->Socketbit;
	return sc;
}

std::string CCSocket::GetlocadIP(IPVX ipvx,int Number)
{
    WSADATA wsaData;
    int result = WSAStartup(MAKEWORD(2, 2), &wsaData);
    if (result != 0) {
        std::cerr << "WSAStartup failed: " << result << std::endl;
        return "Error";
    }

    // 获取主机名
    char hostName[NI_MAXHOST];
    if (gethostname(hostName, NI_MAXHOST) != 0) {
        std::cerr << "gethostname failed" << std::endl;
        WSACleanup();
        return "Error";
    }

    struct addrinfo hints, *info;
    ZeroMemory(&hints, sizeof(hints));
    hints.ai_family = AF_UNSPEC; // AF_INET for IPv4 only
    hints.ai_socktype = SOCK_STREAM;
    hints.ai_protocol = IPPROTO_TCP;

    // 获取地址信息
    result = getaddrinfo(hostName, NULL, &hints, &info);
    if (result != 0) {
        std::cerr << "getaddrinfo failed: " << result << std::endl;
        WSACleanup();
        return "Error";
    }
    char ipString[INET6_ADDRSTRLEN] = {0};
    // 遍历所有地址信息并打印IPv4地址
    for (struct addrinfo* p = info; p != nullptr; p = p->ai_next) {
        void* addr;

        // 对于IPv4，使用ipv4_mapping来兼容IPv6地址
        if (p->ai_family == ipvx) {
            struct sockaddr_in* ipv4 = (struct sockaddr_in*)p->ai_addr;
            addr = &(ipv4->sin_addr);
        } else {
            continue; // 跳过非IPv4地址
        }

        // 将地址转换为字符串
        inet_ntop(p->ai_family, addr, ipString, sizeof(ipString));

       // std::cout << "IPv4 address: " << ipString << std::endl;
    }

    // 释放地址信息
    freeaddrinfo(info);

    WSACleanup();
    return ipString;
}

bool CCSocket::Connect(const char* IP, unsigned short Port)
{
    Stop = true;
	sockaddr_in serveraddr;
	memset(&serveraddr, 0, sizeof(serveraddr));
	serveraddr.sin_family = AF_INET;
	serveraddr.sin_port = htons(Port);
	serveraddr.sin_addr.s_addr = inet_addr(IP);
	int len = sizeof(serveraddr);
	int i = connect(this->Socketbit,(sockaddr*)&serveraddr,len);
	if (i == SOCKET_ERROR)
	{
		return false;
	}
	return true;
}

bool CCSocket::Bind(const char* IP, unsigned short Port)
{
	sockaddr_in serveraddr;
	memset(&serveraddr, 0, sizeof(serveraddr));
	serveraddr.sin_family = AF_INET;
	serveraddr.sin_port = htons(Port);
	serveraddr.sin_addr.s_addr = inet_addr(IP);

	int i = bind(this->Socketbit, (sockaddr*)&serveraddr, sizeof(serveraddr));
	if (i == SOCKET_ERROR)
	{
		closesocket(this->Socketbit);
		WSACleanup();
		return false;
	}

	return true;
}

bool CCSocket::Listen(unsigned short UserNum)
{
    Stop = true;
	int iLisRet = listen(this->Socketbit, UserNum);
	if (iLisRet == SOCKET_ERROR)
	{
		closesocket(this->Socketbit);
		WSACleanup();
		return false;
	}
	return true;
}

CCSocket CCSocket::Accept()
{
    CCSocket sc;
	SOCKADDR_IN serveraddrfrom{};
	int Len = sizeof(serveraddrfrom);
    SOCKET s = accept(this->Socketbit, (SOCKADDR*)&serveraddrfrom, &Len);
    sc.Socketbit = s;
    sc.IPVx = this->IPVx;
    sc.sock = this->sock;
    sc.client = serveraddrfrom;
	return sc;
}

bool CCSocket::Send(const char* str)
{
	int ires = send(this->Socketbit, str, strlen(str), 0);  //<2F><><EFBFBD><EFBFBD><EFBFBD>ַ<EFBFBD><D6B7><EFBFBD>ȥ
	if (ires == -1)
	{
		return false;
	}
	return true;
}

bool CCSocket::SendByte(const char* str, int len)
{
	int ires = send(this->Socketbit, str, len, 0);  //<2F><><EFBFBD><EFBFBD><EFBFBD>ַ<EFBFBD><D6B7><EFBFBD>ȥ
	if (ires == -1)
	{
		return false;
	}
	return true;
}

bool CCSocket::UDPSend(const char* str, const char* IP, int Port)
{
	sockaddr_in serverAddr;
	serverAddr.sin_family = AF_INET;
	serverAddr.sin_port = htons(Port);  // <20><><EFBFBD><EFBFBD><EFBFBD><EFBFBD><EFBFBD>Ķ˿ں<CBBF>  
	serverAddr.sin_addr.s_addr = inet_addr(IP);  // <20><><EFBFBD><EFBFBD><EFBFBD><EFBFBD><EFBFBD>ĵ<EFBFBD>ַ
	if (sendto(this->Socketbit, str, strlen(str), 0, (SOCKADDR*)&serverAddr, sizeof(serverAddr)) == SOCKET_ERROR)
	{
		return false;
	}
	else 
	{
		return true;
	}
}

bool CCSocket::UDPSendByte(const char* str,ByteHander len ,const char* IP, int Port)
{
	sockaddr_in serverAddr;
	serverAddr.sin_family = AF_INET;
	serverAddr.sin_port = htons(Port);
	serverAddr.sin_addr.s_addr = inet_addr(IP);
	if (sendto(this->Socketbit, str, len, 0, (SOCKADDR*)&serverAddr, sizeof(serverAddr)) == SOCKET_ERROR)
	{
		return false;
	}
	else
	{
		return true;
	}
}

ByteHander CCSocket::RecvData(char* buffer,ByteHander lens)
{
	int len = recv(this->Socketbit,buffer,lens,0);
	return len;
}

ByteHander CCSocket::UDPRecvData(char* buffer, ByteHander lens,CCHostInfo* info)
{
    sockaddr_in A = {};
    int clientSize = sizeof(A);
	int bytesReceived = recvfrom(this->Socketbit, buffer, lens+1, 0,(SOCKADDR*)&A, &clientSize);
    char IP[INET_ADDRSTRLEN] = {0};
    inet_ntop(IPVx, &A.sin_addr, IP, INET_ADDRSTRLEN);
    info->IPAddress = IP;
    info->Port = ntohs(A.sin_port);
	return bytesReceived;
}

void CCSocket::Close()
{
    Stop = false;
	closesocket(this->Socketbit);
	//WSACleanup();
}

bool CCSocket::GetDateHead(char* data, ByteHander* size)
{
	int i = sizeof(data);
	memcpy(size,data,i);
	if (size!=0)
	{
		return true;
	}
	return false;
}


bool CCSocket::GetStrHead(char* data, ByteHander size)
{
	for (int i = 0; i < HanderSize; i++)
	{
		data[i] = '\0';
	}
	data[0] = '-';
	memcpy(data, &size, sizeof(ByteHander));
	if (data[0] == '-')
	{
		return false;
	}
	return true;
}

bool CCSocket::Send(const void *__buf, size_t __n, int __flags)
{
    int err = send(Socketbit, (char*)__buf, __n, __flags);
    if(err == -1)
    {
        return false;
    }
    return true;
}

bool CCSocket::operator==(CCSocket socket)
{
    if(socket.Socketbit == this->Socketbit)
    {
        return true;
    }
    return false;
}

bool CCSocket::operator!=(CCSocket socket)
{
    if(socket.Socketbit != this->Socketbit)
    {
        return true;
    }
    return false;
}

CCHostInfo CCSocket::GetHost()
{
    CCHostInfo hostInfo;
    char IP[INET_ADDRSTRLEN] = {0};
    inet_ntop(IPVx, &client.sin_addr, IP, INET_ADDRSTRLEN);
    hostInfo.IPAddress = IP;
    hostInfo.Port = ntohs(client.sin_port);
    return hostInfo;
}

void CCSocket::SetSocketNonBlocking()
{
    u_long mode = 1;
    if (ioctlsocket(this->Socketbit, FIONBIO, &mode) == SOCKET_ERROR)
    {
        std::cerr << "ioctlsocket failed with error: " << WSAGetLastError() << std::endl;
    }
}

bool CCSocket::IsDataAvailable()
{
    fd_set readfds;
    FD_ZERO(&readfds);
    FD_SET(this->Socketbit, &readfds);
    timeval timeout = {0, 0}; // Non-blocking check
    int ret = select(0, &readfds, nullptr, nullptr, &timeout);
    if (ret == SOCKET_ERROR)
    {
        //std::cerr << "select failed with error: " << WSAGetLastError() << std::endl;
        return false;
    }
    return FD_ISSET(this->Socketbit, &readfds);
}

bool CCSocket::isConnectionAlive() {
    fd_set fds;
    FD_ZERO(&fds);
    FD_SET(Socketbit, &fds);
    timeval timeout = { 0, 0 }; // 非阻塞模式立即返回
    int ret = select(Socketbit + 1, &fds, NULL, &fds, &timeout);
    if (ret < 0) {
        // 错误处理
        return false;
    } else if (ret > 0) {
        if (FD_ISSET(Socketbit, &fds)) {
            if (FD_ISSET(Socketbit, &fds)) {
                // 检查异常条件
                return false;
            }
        }
    }
    return true;
}


#elif __linux__


CCSocket CCSocket::Socket(IPVX IPV4orIPV6, TORU TCPorUDP, TYPE Type)
{
    CCSocket sc;
    Socketbit = socket(IPV4orIPV6, Type, TCPorUDP);
    if(Socketbit == 0)
    {
        Socketbit = -1;
        return sc;
    }
    int err = setsockopt(Socketbit, SOL_SOCKET, SO_REUSEADDR | SO_REUSEPORT, &opt, sizeof(opt));
    if(err)
    {
        Socketbit = -2;
        return sc;
    }
    this->IPVx = IPV4orIPV6;
    sc.IPVx = this->IPVx;
    sc.Socketbit = Socketbit;
    return sc;
}

std::string CCSocket::GetlocadIP(IPVX ipvx,int Number)
{
    struct ifaddrs *ifAddrStruct = nullptr;
    struct ifaddrs *ifa = nullptr;
    void *tmpAddrPtr = nullptr;

    getifaddrs(&ifAddrStruct);
    char addressBuffer[10][INET_ADDRSTRLEN] = {0};
    int d = 0;
    for (ifa = ifAddrStruct; ifa != nullptr; ifa = ifa->ifa_next) {
        if (ifa->ifa_addr->sa_family == ipvx) { // 检查它是否为IPv4
            // 获取接口地址
            tmpAddrPtr = &((struct sockaddr_in *)ifa->ifa_addr)->sin_addr;

            inet_ntop(AF_INET, tmpAddrPtr, addressBuffer[d++], INET_ADDRSTRLEN);
            //std::cout << "IPv4 Address: " << addressBuffer << std::endl;
        }
    }

    if (ifAddrStruct != nullptr)
        freeifaddrs(ifAddrStruct);
    return addressBuffer[Number];
}

bool CCSocket::Connect(const char* IP, unsigned short Port)
{
    Stop = true;
    struct sockaddr_in address;
    address.sin_family = IPVx;
    address.sin_addr.s_addr = inet_addr(IP);
    address.sin_port = htons(Port);
    int err = inet_pton(AF_INET, IP, &address.sin_addr);
    if(err <= 0)
    {
        return false;
    }
    int erra = connect(Socketbit, (struct sockaddr *)&address, sizeof(address));
    if(erra < 0)
    {
        return false;
    }
    return true;
}

bool CCSocket::Bind(const char* IP,int Port)
{
    // 设置地址结构
    struct sockaddr_in server_addr;
    memset(&server_addr, '0', sizeof(server_addr));
    server_addr.sin_family = IPVx;
    server_addr.sin_port = htons(8080);
    server_addr.sin_addr.s_addr = INADDR_ANY;

    // 绑定套接字到地址和端口
    if (bind(this->Socketbit, (struct sockaddr *)&server_addr, sizeof(server_addr)) < 0) {
        std::cerr << "Bind failed" << std::endl;
        return false;
    }
    return true;
}

bool CCSocket::Listen(unsigned short UserNum)
{
    Stop = true;
    int err = listen(Socketbit, UserNum);
    if(err < 0)
    {
        return false;
    }
    return true;
}

CCSocket CCSocket::Accept()
{
    CCSocket sc;
	sockaddr_in serveraddrfrom{};
	socklen_t Len = sizeof(serveraddrfrom);
    SOCKET s = accept(this->Socketbit, (struct sockaddr *)&serveraddrfrom, &Len);
    sc.IPVx = this->IPVx;
    sc.Socketbit = s;
    sc.client = serveraddrfrom;
	return sc;
}

bool CCSocket::Send(const char* str)
{
    int err = send(Socketbit, str, strlen(str), 0);
    if(err == -1)
    {
        return false;
    }
    return true;
}

bool CCSocket::SendByte(const char* str,int len)
{
    int err = send(Socketbit, str, len, 0);
    if(err == -1)
    {
        return false;
    }
    return true;
}

bool CCSocket::UDPSend(const char* str, const char* IP, int Port)
{
    sockaddr_in serverAddr;
    serverAddr.sin_family = AF_INET;
    serverAddr.sin_port = htons(Port);
    serverAddr.sin_addr.s_addr = inet_addr(IP);
    if (sendto(Socketbit, str, strlen(str), 0, (struct sockaddr *)&serverAddr, sizeof(serverAddr)) == -1)
    {
        return false;
    }
    else
    {
        return true;
    }
}

bool CCSocket::UDPSendByte(const char* str,ByteHander len ,const char* IP, int Port)
{
    sockaddr_in serverAddr;
    serverAddr.sin_family = AF_INET;
    serverAddr.sin_port = htons(Port);
    serverAddr.sin_addr.s_addr = inet_addr(IP);
    if (sendto(Socketbit, str, len, 0, (struct sockaddr *)&serverAddr, sizeof(serverAddr)) == -1)
    {
        return false;
    }
    else
    {
        return true;
    }
}

ByteHander CCSocket::RecvData(char* buffer,ByteHander lens)
{
    int len = recv(Socketbit,buffer,lens,0);
    return len;
}

bool CCSocket::GetDateHead(char* data, ByteHander* size)
{
    int i = sizeof(data);
    memcpy(size,data,i);
    if (size!=0)
    {
        return true;
    }
    return false;
}

bool CCSocket::GetStrHead(char* data, ByteHander size)
{
    for (int i = 0; i < HanderSize; i++)
    {
        data[i] = '\0';
    }
    data[0] = '-';
    memcpy(data, &size, sizeof(ByteHander));
    if (data[0] == '-')
    {
        return false;
    }
    return true;
}

ByteHander CCSocket::UDPRecvData(char* buffer,ByteHander lens,CCHostInfo* info)
{
    sockaddr_in A = {};
    socklen_t clientSize = sizeof(A);
	int bytesReceived = recvfrom(this->Socketbit, buffer, lens+1, 0,(struct sockaddr *)&A, &clientSize);
    char IP[INET_ADDRSTRLEN] = {0};
    inet_ntop(IPVx, &A.sin_addr, IP, INET_ADDRSTRLEN);
    info->IPAddress = IP;
    info->Port = ntohs(A.sin_port);
	return bytesReceived;
}

void CCSocket::Close()
{
    Stop = false;
    close(Socketbit);
}

bool CCSocket::Send(const void *__buf, size_t __n, int __flags)
{
    int err = send(Socketbit, __buf, __n, __flags);
    if(err == -1)
    {
        return false;
    }
    return true;
}

bool CCSocket::operator==(CCSocket socket)
{
    if(socket.Socketbit == this->Socketbit)
    {
        return true;
    }
    return false;
}

bool CCSocket::operator!=(CCSocket socket)
{
    if(socket.Socketbit != this->Socketbit)
    {
        return true;
    }
    return false;
}

CCHostInfo CCSocket::GetHost()
{
    CCHostInfo hostInfo;
    char IP[INET_ADDRSTRLEN] = {0};
    inet_ntop(IPVx, &client.sin_addr, IP, INET_ADDRSTRLEN);
    hostInfo.IPAddress = IP;
    hostInfo.Port = ntohs(client.sin_port);
    return hostInfo;
}

void CCSocket::SetSocketNonBlocking()
{
    int flags = fcntl(this->Socketbit, F_GETFL, 0);
    if (flags == -1)
    {
        perror("fcntl(F_GETFL)");
        return;
    }
    flags |= O_NONBLOCK;
    if (fcntl(this->Socketbit, F_SETFL, flags) == -1)
    {
        perror("fcntl(F_SETFL)");
    }
}

bool CCSocket::IsDataAvailable()
{
    pollfd pfd{};
    pfd.fd = this->Socketbit;
    pfd.events = POLLIN;
    pfd.revents = 0;
    int ret = poll(&pfd, 1, 0); // Non-blocking check
    if (ret == -1)
    {
        perror("poll");
        return false;
    }
    return pfd.revents & POLLIN;
}
bool CCSocket::isConnectionAlive() {
    fd_set fds;
    FD_ZERO(&fds);
    FD_SET(Socketbit, &fds);
    timeval timeout = { 0, 0 }; // 非阻塞模式立即返回
    int ret = select(Socketbit + 1, &fds, NULL, &fds, &timeout);
    if (ret < 0) {
        // 错误处理
        return false;
    } else if (ret > 0) {
        if (FD_ISSET(Socketbit, &fds)) {
            if (FD_ISSET(Socketbit, &fds)) {
                // 检查异常条件
                return false;
            }
        }
    }
    return true;
}

#else

#endif





