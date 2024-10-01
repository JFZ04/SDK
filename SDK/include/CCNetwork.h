#include "CC.h"
#include "CCSocket.h"
#include "CCByteArray.h"
#include "CCString.h"

#ifndef T_CCNETWORK_H
#define T_CCNETWORK_H

class CCPackets
{
public:
    CCPackets() = default;
    ByteHander size(){
        return _size;
    }
    void resize(int len){
        size_t _size;
    }
    void Set(const char* str,ByteHander len){
        for (int i = 0; i < len; ++i) {
            value.data().push_back((CCByte)str[i]);
        }
    }
    CCByteArray getByteArray(){
        return value;
    }
private:
    ByteHander _size;
    CCByteArray value;
};


class CCNetwork:public CCSocket
{
public:
    using CCSocket::CCSocket;
    CCNetwork();
    CCNetwork(IPVX ipvx,TORU toru);
    CCNetwork(CCSocket socket);
    CCNetwork(CCSocket& socket);
    bool ListenStart(CCString host, int port,int Max = 30);
private:

public:
    bool ReceiveData(CCPackets& packets);
private:

protected:

};


#endif
