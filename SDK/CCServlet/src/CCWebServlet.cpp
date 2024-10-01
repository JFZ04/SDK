#include "CCWebServlet.h"

bool CCWebServlet::Start(CCString IP,int port){
    FlagRun = true;
    RootLen = this->PathSix.size();
    m_Socket.Socket(IPVX::IPV4,TORU::TCP);
    m_Socket.SetSocketNonBlocking();
    m_ThreadPool.SetThreadTimeout(1000 * 16);
    m_ThreadPool.InitStart(numThreads);
    ServerIP = IP;
    if(!m_Socket.Bind(ServerIP.c_str(), port))
    {
        return false;
    }
    if(!m_Socket.Listen(ListenMax))
    {
        return false;
    }
    m_Thread.SetThread(&CCWebServlet::ProcessRequest, this);
    m_Thread.Start();
    return true;
}

void CCWebServlet::ProcessRequest(){
    while (m_Thread.Sign()){
        CCSocket client = m_Socket.Accept();
        if(client.GetHost().Port != 0){
            m_ThreadPool.AddTask(&CCWebServlet::ResponseData, this, client);
        }
    }
}

void CCWebServlet::ResponseData(CCSocket &socket){
    std::vector<char> data;
    while (true){
        if(socket.IsDataAvailable()){
            char buffer[Buffer_Max] = {0};
            auto len = socket.RecvData(buffer,Buffer_Max);
            if(len > 0){
                for(auto i : buffer){
                    data.push_back(i);
                }
                CCString string1(data.begin(), data.end());
                auto l = string1.find("\r\n\r\n");
                if(l != CCString::npos){
                    break;
                }
                else {
                    socket.Close();
                    return;
                }
            }
            else{
                return;
            }
        }
        else{
            break;
        }
    }
    if(data.empty()) {
        socket.Close();
        return;
    }
    CCString RecvMessage = CCString(data.begin(), data.end());
    data.clear();
    RequestProcess Stuart = RequestProcess_First;
    std::istringstream stream(RecvMessage.substr());
    CCRequest HTTPRequests(socket);
    BufferReader A;
    CCResponse HTTPResponse(socket,CORSConfig);
    while (!RecvMessage.empty()){
        auto strlen = RecvMessage.find("\r\n");
        std::istringstream stream(RecvMessage.substr());
        if(strlen == 0){
            Stuart = RequestProcess_Body;
            for (const auto& str : HTTPRequests.Headers) {
                if(str.first == "Sec-WebSocket-Key"){
                    CCString Key = str.second;
                    HTTPResponse.GetWriter().ResopenseWebSocket(Key);
                    while (true){
                        if(socket.IsDataAvailable()){
                            char buffer[Buffer_Max] = {0};
                            auto len = socket.RecvData(buffer,Buffer_Max);
                            if(len > 0){
                                if(WebSocketSignal){
                                    WebSocketSignal();
                                }
                            }
                            else{

                                return;
                            }
                        }
                    }
                }
            }
        }
        if(strlen != std::string::npos) {
            if (Stuart == RequestProcess_First) {
                stream >> A.Method;
                stream >> A.Path;
                auto l = A.Path.find('?');
                auto la = A.Path.length();
                if(l > 0 && l != std::string::npos && A.Method=="GET"){
                    RecvMessage.append(A.Path.substr(l, la));
                    A.Path = A.Path.substr(0, l);
                }
                stream >> A.Version;

                RecvMessage.erase(0, strlen + 2);
                Stuart = RequestProcess_Header;
            } else if (Stuart == RequestProcess_Header) {
                std::string Key, Value;
                stream >> Key;
                stream >> Value;
                Key.erase(remove(Key.begin(), Key.end(), ':'), Key.end());
                HTTPRequests.Headers.insert(std::pair<std::string, std::string>(Key, Value));
                RecvMessage.erase(0, strlen + 2);
            }
            else
            {
                auto st = HTTPRequests.Headers["content-type"];
                if(st.empty()){
                    st = HTTPRequests.Headers["Content-Type"];
                }
                if(st != "multipart/form-data;" && st != "multipart/form-data"){
                    A.Body = RecvMessage;
                }
                else {
                    RecvMessage = GetFileHeaders(RecvMessage,socket,HTTPRequests.FileHeaders);
                    for (auto i : RecvMessage) {
                        A.Buffer.push_back(i);
                    }
                }
                RecvMessage = "";
                HTTPRequests.SetBuffer(A);
                break;
            }
        }
    }
    if(A.Method == "OPTIONS"){
        HTTPResponse.GetWriter().ResponseOK();
    }
    else{
        CCString SPath = HTTPRequests.GetReader().Path;
        auto it = RequestFun[SPath];
        if(it){
            try {
                it(HTTPRequests, HTTPResponse);
            }
            catch (const std::exception &e) {
                CCTimeData time;
                CCString str = "[" + time.to_String() + "] -> " + CC::to_String(e.what());
                CC::Println(str);
            }
        }
        else{
            bool da = false;
            for (int i = 0; i < RootLen; ++i)
            {
                CCString Path = PathSix[i] + HTTPRequests.GetReader().Path;
                if (HTTPResponse.GetWriter().HtmlWrite(Path))
                {
                    Threading::Sleep(1000);
                    da = true;
                    break;
                }
            }
            if (!da)
            {
                if(WebSignalFun){
                    try {
                        WebSignalFun(HTTPRequests, HTTPResponse);
                    }
                    catch (const std::exception &e) {
                        CCTimeData time;
                        CCString str = "[" + time.to_String() + "] -> " + CC::to_String(e.what());
                        CC::Println(str);
                    }
                }
            }
        }
    }
    socket.Close();
}

void CCWebServlet::SetThreadNumber(int headcount) {
    this->numThreads = headcount;
}

void CCWebServlet::Close() {
    m_Thread.Stop();
    m_ThreadPool.Stop();
    m_Socket.Close();
    FlagRun = false;
}

bool CCWebServlet::Sign() const {
    return FlagRun;
}

void CCWebServlet::SetWebServlet(CCString UrlPath, CCWebServlet::RequestFunc RFunc) {
    RequestFun.insert(std::pair<std::string, RequestFunc>(UrlPath, RFunc));
}

void CCWebServlet::SetAddRootPath(const CCString &rootPath) {
    PathSix.push_back(rootPath);
}

void CCWebServlet::SetCorsConfig(CORS &cors) {
    CORSConfig = cors;
}

CCString CCWebServlet::GetFileHeaders(CCString& str,CCSocket &socket,std::map<CCString, CCString> &map) {
    std::vector<char> data;
    CCString A,B = str;
    while (true){
        if(socket.IsDataAvailable()){
            char Buffer[1] = {0};
            auto l = socket.RecvData(Buffer, 1);
            if(l > 0){
                for(auto i : Buffer){
                    data.push_back(i);
                }
                A = B + std::string(data.begin(), data.end());
                if(A.find("\r\n\r\n") != std::string::npos){
                    break;
                }
            }
        }
        else {
            break;
        }
    }
    CCString C = GetFileDateHeader(A, map);
    return C;
}

void CCWebServlet::SetWebServletFun(CCWebServlet::RequestFunc RFunc) {
    WebSignalFun = std::move(RFunc);
}

CCThreadPool CCWebServlet::GetServletThreadPool() {
    return m_ThreadPool;
}

template<typename Fun>
void CCWebServlet::SetWebSocketSignalSlots(Fun &&fun) {
    WebSocketSignal = std::forward<Fun>(fun);
}
