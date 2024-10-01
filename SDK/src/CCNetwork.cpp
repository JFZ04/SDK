#include "CCNetwork.h"

bool CCNetwork::ReceiveData(CCPackets &packets) {
    auto size = packets.size();
    char* buffer = new char[size];
    memset(buffer,0,size);
    while (true){
        if(IsDataAvailable()){
            auto len = RecvData(buffer, size);
//            CCString str = CCEncode::Format(buffer,Encode.UTF8,Encode.GBK);
            if(len > 0){
                packets.Set(buffer,len);
                return true;
            }
            else {
                return false;
            }
        }
    }
}

CCNetwork::CCNetwork(IPVX ipvx, TORU toru) {
    if(toru == TORU::TCP){
        Socket(ipvx, toru,TYPE::STREAM);
    }
    else if(toru == TORU::UDP){
        Socket(ipvx, toru,TYPE::DGRAM);
    }
}

CCNetwork::CCNetwork() {
    Socket(IPVX::IPV4, TORU::TCP,TYPE::STREAM);
}

bool CCNetwork::ListenStart(CCString host, int port, int Max) {
    if(Bind(host.c_str(), port)){
        if(Listen(Max)){
            return true;
        }
    }
    return false;
}

CCNetwork::CCNetwork(CCSocket socket) {
    Socketbit = socket.Socketbit;
}

CCNetwork::CCNetwork(CCSocket &socket) {
    Socketbit = socket.Socketbit;
}
