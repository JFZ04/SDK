#include "CCResponse.h"

#include <utility>

CCResponse::CCResponse(CCSocket &socket,CORS& cors) {
    this->Client = socket;
    Cors = cors;
}
HTTPResponse CCResponse::GetWriter()
{
    return HTTPResponse(Client,Cors);
}
void HTTPResponse::HtmlTextWrite(const CCString& str) {
    this->Write(str, "text/html");
}
HTTPResponse::HTTPResponse(CCSocket &socket,CORS& cors) {
    Client = socket;
    CORSConfig = cors;
}
void HTTPResponse::Write(const CCString& str, const char *type){
    char buf[200] = {0};
    Client.Send("HTTP/1.1 200 OK\r\n");
    Client.Send("Server:HTTP->CCWebServlet->OK\r\n");
    CCString Origin;
    auto len = CORSConfig.GetAllowOrigin().size();
    std::map<CCString,CCString> headers = CORSConfig.GetHeader();
    for (int i = 0;i < len;i++){
        CCString string = CORSConfig.GetAllowOrigin()[i];
        Origin.append(string);
        if(i != len - 1)
        {
            Origin.append(",");
        }
    }
    for (const auto& i : headers) {
        sprintf(buf, "%s:%s\r\n",i.first.c_str(),i.second.c_str());
        Client.Send(buf);
    }
    if(Origin.empty())
    {
        Origin = "null";
    }
    sprintf(buf, "Access-Control-Allow-Origin:%s\r\n",Origin.c_str());
    Client.Send(buf);
    Client.Send("Access-Control-Allow-Headers:Content-Type, X-Requested-With\r\n");
    Client.Send("Access-Control-Allow-Methods:GET,POST,PUT,DELETE,OPTIONS\r\n");
    try {
        if(!JSON::parse(str).empty())
        {
            type = "application/json";
        }
    }
    catch (const std::exception& e) {}
    sprintf(buf, "Content-type:%s; charset=utf-8\r\n",type);
    Client.Send(buf);
    sprintf(buf, "Content-Length:%zu\r\n", str.length());
    Client.Send(buf);//Content-Length:
    Client.Send("\r\n");
    Client.Send(str.c_str());
    //Client.Send("\r\n\r\n");
}
bool HTTPResponse::HtmlWrite(const CCString &str) {
    CCFile file = CCFile(str.c_str());
    if (file.IsOpen()) {
        CCVar info = file.GetFileInfo();
        size_t fileSize = info.Size;
        file.Close();
        SetHeader("Content-Type", GetFileType(str));
        SetHeader("Content-Length", std::to_string(fileSize));

        // 获取文件流
        std::ifstream inputFile(info.Path, std::ios::binary);
        if (!inputFile.is_open()) {
            std::cerr << "无法打开文件" << std::endl;
            return false;
        }

        // 开始发送文件数据
        const size_t bufferSize = 4096;
        char buffer[bufferSize];
        size_t remaining = fileSize;
        HTTPWrite("");  // 初始化 HTTP 写入

        while (remaining > 0) {
            size_t bytesToRead = std::min(remaining, bufferSize);
            inputFile.read(buffer, bytesToRead);
            size_t bytesRead = inputFile.gcount();
            // 将读取的数据写入 HTTP 响应
            if(IsConnect()){
                Write(buffer,bytesRead);
            }
            remaining -= bytesRead;
        }

        inputFile.close();
        return true;
    }
    else {
        return false;
    }
}
bool HTTPResponse::SendResources(const char *Path, const char *Mode) {
    Mutex.lock();
    FILE *file = fopen(Path, Mode); // 替换为你的文件名
    if (file == nullptr)
    {
        Mutex.unlock();
        return false;
    }
    Client.Send("HTTP/1.1 200 OK\r\n");
    Client.Send("Server:HTTP->CCWeb->OK\r\n");
    char buf[50] = {0};
    sprintf(buf, "Content-type:%s\r\n", GetFileType(Path).c_str());
    Client.Send(buf);
    sprintf(buf, "Content-Length:%d\r\n", GetFileSize(Path));
    Client.Send(buf);//Content-Length:
    Client.Send("\r\n");

    int cout = 0;
    char bufs[4096] = { 0 };
    int fpId = -1;
    struct stat st;
    fpId = fileno( file );
    if( fstat( fpId ,&st) == -1  )
    {
        Mutex.unlock();
        return false;
    }

    while (!feof(file))
    {
        int ret = fread(bufs,sizeof(char),sizeof bufs,file);
        Client.Send(bufs,sizeof bufs,0);
        cout += ret;
    }
    fclose(file);
    Mutex.unlock();
    return true;
}
void HTTPResponse::Send(const CCString &string) {
    Client.Send(string.c_str());
}
void HTTPResponse::ResponseOK() {
    char buf[200] = {0};
    Client.Send("HTTP/1.1 200 OK\r\n");
    Client.Send("Server:HTTP->CCWeb->OK\r\n");
    CCString Origin;
    auto len = CORSConfig.GetAllowOrigin().size();
    std::map<CCString,CCString> headers = CORSConfig.GetHeader();
    for (int i = 0;i < len;i++){
        CCString string = CORSConfig.GetAllowOrigin()[i];
        Origin.append(string);
        if(i != len - 1)
        {
            Origin.append(",");
        }
    }
    if(Origin.empty())
    {
        Origin = "null";
    }
    sprintf(buf, "Access-Control-Allow-Origin:%s\r\n",Origin.c_str());
    Client.Send(buf);
    Client.Send("Access-Control-Allow-Headers:*, X-Requested-With\r\n");
    Client.Send("Access-Control-Allow-Methods:GET,POST,PUT,DELETE,OPTIONS\r\n");
    Client.Send("\r\n");
}
void HTTPResponse::ResopenseWebSocket(const CCString& Key) {
    CCString acceptValue = sha1(Key);
    CCString st = "HTTP/1.1 101 Switching Protocols\r\n";
    st.append("Upgrade: websocket\r\n");
    st.append("Connection: Upgrade\r\n");
    st.append("Sec-WebSocket-Accept:" + acceptValue + "\r\n");
    st.append("\r\n");
    Client.Send(st.c_str());
}
CCString HTTPResponse::base64_encode(const unsigned char *data, size_t length) {
    BIO *bio, *b64;
    BUF_MEM *bufferPtr;

    b64 = BIO_new(BIO_f_base64());
    bio = BIO_new(BIO_s_mem());
    bio = BIO_push(b64, bio);

    BIO_write(bio, data, (int)length);
    BIO_flush(bio);
    BIO_get_mem_ptr(bio, &bufferPtr);
    BIO_set_close(bio, BIO_NOCLOSE);
    BIO_free_all(bio);
    CCString ret(bufferPtr->data, bufferPtr->length-1);
    return ret;
}
CCString HTTPResponse::sha1(const CCString &input) {
    CCString str = input + "258EAFA5-E914-47DA-95CA-C5AB0DC85B11";
    unsigned char hash[SHA_DIGEST_LENGTH];
    SHA_CTX sha1;
    SHA1_Init(&sha1);
    SHA1_Update(&sha1, str.c_str(), str.length());
    SHA1_Final(hash, &sha1);
    return base64_encode(hash, SHA_DIGEST_LENGTH);
}

std::streamoff HTTPResponse::getFileSize(const string &filePath) {
    std::ifstream file(filePath, std::ios::binary | std::ios::ate);
    if (!file.is_open()) {
        return -1; // 文件打开失败
    }

    // 获取文件指针当前位置，即文件大小
    std::streamoff fileSize = file.tellg();

    // 将文件指针移回文件开头
    file.seekg(0, std::ios::beg);

    file.close();
    return fileSize;
}

bool HTTPResponse::WriteMultipart(const CCString &path) {
    // 打开文件
    std::ifstream file(path, std::ios::binary);
    if (!file.is_open()) {
        Client.Send("HTTP/1.1 404 Not Found\r\n");
        Client.Send("Content-Length: 0\r\n");
        Client.Send("\r\n");
        return false;
    }

    // 获取文件大小
    std::streamoff fileSize = getFileSize(path);

    // 设置响应头
    std::string boundary = "----WebKitFormBoundary7MA4YWxkTrZu0gW";
    std::stringstream response;

    // 发送 HTTP 响应头
    response << "HTTP/1.1 200 OK\r\n";
    response << "Content-Type: multipart/form-data; boundary=" << boundary << "--\r\n";
    response << "Content-Length: " << (fileSize + boundary.length() + 128) << "\r\n";
    response << "\r\n";

    // 发送 multipart/form-data 数据
    response << "--" << boundary << "\r\n";
    response << R"(Content-Disposition: form-data; name="file"; filename=")" << path << "\"\r\n";
    response << "Content-Type: application/octet-stream\r\n";
    response << "\r\n";

    // 发送文件内容
    char buffer[4096];
    while (file.read(buffer, sizeof(buffer))) {
//        size_t bytesRead = file.gcount();
        Client.Send(buffer);
    }

    // 发送文件末尾
    Client.Send("\r\n");
    CCString end = "--" + boundary + "--\r\n";
    Client.Send(end.c_str());

    file.close();

    return true;
}

bool HTTPResponse::FileDownload(const CCString &path) {
    CCFile file = CCFile(path.c_str());
    if (file.IsOpen()) {
        CCVar info = file.GetFileInfo();
        size_t fileSize = info.Size;
        file.Close();
        SetHeader("Content-Type", "application/octet-stream; charset=utf-8");
        SetHeader("Content-Disposition", "attachment; filename=" + info.Name);
        SetHeader("Content-Length", std::to_string(fileSize));

        // 获取文件流
        std::ifstream inputFile(info.Path, std::ios::binary);
        if (!inputFile.is_open()) {
            std::cerr << "无法打开文件" << std::endl;
            return false;
        }

        // 开始发送文件数据
        const size_t bufferSize = 4096;
        char buffer[bufferSize];
        size_t remaining = fileSize;
        HTTPWrite("");  // 初始化 HTTP 写入

        while (remaining > 0) {
            size_t bytesToRead = std::min(remaining, bufferSize);
            inputFile.read(buffer, bytesToRead);
            size_t bytesRead = inputFile.gcount();
            // 将读取的数据写入 HTTP 响应
            if(IsConnect()){
                Write(buffer,bytesRead);
            }
            remaining -= bytesRead;
        }

        inputFile.close();
        return true;
    }
    else {
        return false;
    }
}

void HTTPResponse::SetHeader(const CCString& Key, CCString Value) {
    Headers[Key] = std::move(Value);
}

std::map<CCString, CCString> HTTPResponse::GetHeaders() {
    return Headers;
}

void HTTPResponse::HTTPWrite(const CCString &str) {
    CCString SData;
    SData.append("HTTP/1.1 200 OK\r\n");
    SData.append("CCWebServlet: HTTP->CCWeb->OK\r\n");
    for (CCVar i : Headers) {
        SData.append(i.first + ": " + i.second + "\r\n");
    }
    SData.append("\r\n");
    SData.append(str);
    Client.Send(SData.c_str());
}

void HTTPResponse::Write(char *data, size_t length) {
    Client.Send(data, length,0);
}

bool HTTPResponse::IsConnect() {
    return Client.isConnectionAlive();
}
