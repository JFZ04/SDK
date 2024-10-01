#include "CCRequest.h"

#include <CCThread.h>
#include <utility>

CCString CCRequest::GetFileType(const CCString &str) {
    std::string suffix = GetFileSuffix(str);
    if(suffix == "html")
    {
        return "text/html";
    }
    else if(suffix == "css")
    {
        return "text/css";
    }
    else if(suffix == "js")
    {
        return "text/javascript";
    }
    else if(suffix == "png")
    {
        return "image/png";
    }
    else if(suffix == "jpg")
    {
        return "image/jpg";
    }
    else if(suffix == "jpeg")
    {
        return "image/jpeg";
    }
    else if(suffix == "gif")
    {
        return "image/gif";
    }
    else if(suffix == "ico")
    {
        return "image/x-icon";
    }
    else if(suffix == "svg")
    {
        return "image/svg+xml";
    }
    else if(suffix == "txt")
    {
        return "text/plain";
    }
    else if(suffix == "xml")
    {
        return "text/xml";
    }
    else if(suffix == "json")
    {
        return "application/json";
    }
    else if(suffix == "pdf")
    {
        return "application/pdf";
    }
    else if(suffix == "zip")
    {
        return "application/zip";
    }
    else
    {
        return suffix;
    }
}

CCString CCRequest::GetFileSuffix(const CCString &filePath) {
    size_t dotPosition = filePath.rfind('.'); // 从右向左查找最后一个'.'的位置
    if (dotPosition == std::string::npos)
    {
        return ""; // 如果没有找到'.'，则返回空字符串
    }
    return filePath.substr(dotPosition + 1); // 返回'.'之后的部分，即文件后缀名
}

unsigned int CCRequest::GetFileSize(const char *path) {
    FILE *file = fopen(path, "rb");
    if (file == nullptr) {
        perror("Error opening file");
        return 1;
    }
    fclose(file);
    struct stat fileStat{};
    if (stat(path, &fileStat) == 0)
    {
        return fileStat.st_size;
    }
    else
    {
        perror("Error getting file size");
        return 0;
    }
}

void CCRequest::SetBuffer(BufferReader &buffer) {
    Buffer = buffer;
}

BufferReader CCRequest::GetReader() {
    return Buffer;
}

JSON CCRequest::GetJson(const BufferReader& instr) {
    try
    {
        return JSON::parse(instr.Body);
    }
    catch (const std::exception& e)
    {
        return nullptr;
    }
}

BufferFile CCRequest::GetFile(BufferReader in) {
    BufferFile File;
    File.Buffer = ReaderFormData(std::move(in),File.Headers);
    return File;
}

std::map<std::string, std::string> CCRequest::parseKeyValuePairs(const std::string &input) {
    std::map<std::string, std::string> keyValuePairs;
    size_t start = 0;
    size_t end = 0;

    while ((end = input.find('\r', start)) != std::string::npos) {
        std::string line = input.substr(start, end - start);

        size_t colonPos = line.find(':');
        if (colonPos == std::string::npos) {
            // 处理键值对
            size_t eqPos = line.find('=');
            if (eqPos != std::string::npos) {
                std::string key = line.substr(0, eqPos);
                size_t quoteStart = line.find('"', eqPos);
                size_t quoteEnd = line.find('"', quoteStart + 1);
                if (quoteStart != std::string::npos && quoteEnd != std::string::npos) {
                    std::string value = line.substr(quoteStart + 1, quoteEnd - quoteStart - 1);
                    keyValuePairs[key] = value;
                }
            }
        } else {
            // 处理以冒号分隔的键值对
            std::string key = line.substr(0, colonPos);
            std::string value = line.substr(colonPos + 2); // 跳过冒号和空格
            keyValuePairs[key] = value;
        }
        start = end + 2; // 跳过 "\r\n"
    }

    return keyValuePairs;
}

void CCRequest::GetFormData(CCString &input, CCString &key, CCString &value) {
    // 寻找等号的位置
    size_t equalPos = input.find('=');
    if (equalPos != std::string::npos) {
        // 提取键（等号前的部分）
        key = input.substr(0, equalPos);
        key.erase(0, key.find_first_not_of(' ')); // 去除前导空格
        key.erase(key.find_last_not_of(' ') + 1); // 去除尾随空格

        // 寻找第一个双引号的位置
        size_t startPos = input.find('"', equalPos);
        if (startPos != std::string::npos) {
            // 寻找最后一个双引号的位置
            size_t endPos = input.find('"', startPos + 1);
            if (endPos != std::string::npos) {
                // 提取值（两个双引号之间的部分）
                value = input.substr(startPos + 1, endPos - startPos - 1);
            }
        }
    }
}

CCString CCRequest::GetFileDateHeader(BufferReader in, std::map<CCString, CCString> &map) {
    CCString A = GetFileDateHeader(in.Body,map);
    return A;
}

std::vector<char> CCRequest::ReaderFormData(BufferReader in, std::map<CCString, CCString> &map) {
    std::vector<char> data;
    CCString str = GetFileDateHeader(std::move(in),map);
    CCString B = "\r\n" + map["Boundary"] + "--\r\n";
    auto len = str.find(B);
    if(len != std::string::npos){
        for (int i = 0; i < len; ++i) {
            data.push_back(str[i]);
        }
    }
    return data;
}

CCString CCRequest::GetParameter(CCString string) {
    CCString str,f = CCString(string + "=");
    auto len = Buffer.Body.length();
    int pos = 0,A = f.length();
    for (int i = 0; i < len; ++i) {
        CCString B = Buffer.Body.substr(i,A);
        if(f == B){
          pos = i + A;
          break;
        }
    }
    for (int i = pos; i < len; ++i) {
        if(Buffer.Body[i] != '&'){
            str += Buffer.Body[i];
        }
        else{
            break;
        }
    }
    return str;
}

OutPutStream CCRequest::GetWriter() {
    return this->Socket;
}

CCRequest::CCRequest(CCSocket &sc) {
    this->Socket = sc;
}

BufferReader CCRequest::ReaderFileData() {
    auto OutPutSteam = GetWriter().GetSocket();
    std::vector<char> BF;
    while(true){
        char buffer[1024] = {0};
        if(OutPutSteam.IsDataAvailable()){
            auto length = OutPutSteam.RecvData(buffer,1024);
            if(length > 0){
                for (auto i : buffer) {
                    BF.push_back(i);
                }
            }
            else {
                break;
            }
        }
        else{
            break;
        }
    }
    auto F = GetReader();
    CCString str(BF.begin(), BF.end());
    F.Body = str;
    return F;
}

CCString CCRequest::GetFileDateHeader(CCString& in, map <CCString, CCString> &map) {
    CCString str = in,A,B;
    auto len = str.find("\r\n");
    while (true){
        CCString Boundary = str.substr(0,len);
        if(Boundary.empty()){
            str.erase(0,len+2);
        }
        else {
            str.erase(0,len+2);
            map["Boundary"] = Boundary;
            break;
        }
        len = str.find("\r\n");
    }
    std::stringstream ss(str);
    ss >> A;
    ss >> B;
    len = A.length() + B.length();
    A.erase(A.length()-1,A.length());
    B.erase(B.length()-1,B.length());
    map[A] = B;
    str.erase(0,len+2);
    GetFormData(str,A,B);
    map[A] = B;
    len = A.length() + B.length();
    str.erase(0,len + 5);
    len = str.find("\r\n\r\n");
    CCString str2 = str.substr(0,len+4);
    str.erase(0,len+4);
    auto m = parseKeyValuePairs(str2);
    for (const auto& i : m) {
        map[i.first] = i.second;
    }
    return str;
}

InPutStream CCRequest::GetInputStream() {
    return {this};
}

void OutPutStream::SetMethod(MethodType method) {
    if(method == MethodType_GET){
        this->SendHeaders["Method"] = "GET";
    }
    if(method == MethodType_POST){
        this->SendHeaders["Method"] = "POST";
    }
    if(method == MethodType_PUT){
        this->SendHeaders["Method"] = "PUT";
    }
    if(method == MethodType_DELETE){
        this->SendHeaders["Method"] = "DELETE";
    }
    if(method == MethodType_OPTIONS){
        this->SendHeaders["Method"] = "OPTIONS";
    }
    if(method == MethodType_HEAD){
        this->SendHeaders["Method"] = "HEAD";
    }
}

void OutPutStream::AddHeader(const CCString &Key, const CCString &Value) {
    this->SendHeaders[Key] = Value;
}

void OutPutStream::SetHost(const CCString &host) {
    this->RPath = host;
}

void OutPutStream::SetBufferBody(CCString body) {
    this->RBody = body;
}

OutPutStream::OutPutStream(CCSocket &sc) {
    this->Socket = sc;
}

void OutPutStream::Send() {
    CCString str = this->SendHeaders["Method"] + " " + this->RPath + " HTTP/1.1\r\n";
    str.append("Server:HTTP->CCWeb->OK\r\n");
    for (int i = 0; i < this->SendHeaders.size(); ++i) {
        CCString A = this->SendHeaders.begin()->first+":"+this->SendHeaders.begin()->second+"\r\n";
        str.append(A);
    }
    str.append("\r\n");
    str.append(this->RBody);
    Socket.SendByte(str.c_str(),(int)str.length());
}

CCSocket OutPutStream::GetSocket() {
    return Socket;
}

bool BufferFile::Save(const CCString &Path) {
    CCFile file = CCFile(Path.c_str(),CC::ios::wb);
    if(file.IsOpen()){
        file.Write((char*)Buffer.data(),Buffer.size());
        Threading::Sleep(1000*16);
        file.Close();
        return true;
    }
    return false;
}

CCString BufferFile::GetFileName() {
    return this->Headers["filename"];
}
InPutStream::InPutStream(CCRequest* Res) {
   this->Socket = Res->GetWriter().GetSocket();
   this->bufferReader = Res->GetReader();
   this->FileHeaders = Res->FileHeaders;
   this->Headers = Res->Headers;
}
void InPutStream::WriteBufferToFile(const CCString& Path) {
    CCFile(Path.c_str(),CC::ios::wb,false).Deleted();
    CCString str = "\r\n" + FileHeaders["Boundary"] + "--\r\n";
    auto strlength = str.length();
    auto S = Headers["Content-Length"];
    if(S.empty()){
        S = Headers["content-length"];
    }
    long long Maxlength = std::stoll(S);
    std::vector<char> data = bufferReader.Buffer;
    int F = 0;
    CCFileOutStream file(Path, std::ios::app | std::ios::binary);
    // 检查文件是否成功打开
    if (!file.is_open()) {
        std::cerr << "无法打开文件: " << Path << std::endl;
        return;
    }
    while (true){
        if(Socket.IsDataAvailable()){
            char buffer[Buffer_Max] = {0};
            auto l = Socket.RecvData(buffer, Buffer_Max);
            if(l > 0 && F == 0){
                if(l != Buffer_Max){
                    for (int i = 0; i < l; ++i) {
                        data.push_back(buffer[i]);
                    }
                }
                else {
                    for (auto i : buffer) {
                        data.push_back(i);
                    }
                }
                Maxlength = Maxlength - data.size();
                if(Maxlength < Buffer_Max * 2){
                    CCString DB = CCString(data.begin(),data.end());
                    auto len = DB.find(str);
                    if(len != std::string::npos){
                        data = std::vector<char>(data.begin(),data.begin() + len);
                        F = 1;
                    }
                }
                file.write(data.data(), data.size());
                data.clear();
            }
            else {
                file.close();
                CCFile(Path.c_str(),CC::ios::wb,false).Deleted();
                std::cout << "连接中断" << std::endl;
                return;
            }
        }
        else {
            break;
        }
    }
    file.close();
}

CCObject InPutStream::Get(CCString key) {
    return FileHeaders[key].c_str();
}

void InPutStream::DeleteTailOf(CCString Path, CCString str) {
    // 读取文件尾部数据
    std::string tail = readTailOfFile(Path, Buffer_Max);
    if (!tail.empty()) {
        if (filterAndPrint(tail, str)) {
            std::cout << "文件: " << Path << std::endl;
        }
    }
}

std::string InPutStream::readTailOfFile(const string &filepath, size_t bufferSize) {
    std::ifstream file(filepath, std::ios::binary);
    if (!file.is_open()) {
        std::cerr << "无法打开文件: " << filepath << std::endl;
        return "";
    }

    // 获取文件大小
    file.seekg(0, std::ios::end);
    std::streampos fileSize = file.tellg();
    file.seekg(0, std::ios::beg);

    // 计算从文件末尾读取的字节数
    size_t bytesToRead = static_cast<size_t>(fileSize);
    if (bytesToRead > bufferSize) {
        bytesToRead = bufferSize;
    }

    // 读取文件尾部数据
    std::vector<char> buffer(bytesToRead);
    file.seekg((size_t)fileSize - bytesToRead, std::ios::beg);
    file.read(buffer.data(), bytesToRead);

    // 将缓冲区转换为字符串
    std::string tail(buffer.begin(), buffer.end());
    return tail;
}

bool InPutStream::filterAndPrint(const string &tail, const string &filter) {
    size_t pos = tail.find(filter);
    if (pos != std::string::npos) {
        std::cout << "找到指定数据: " << filter << std::endl;
        std::cout << "尾部数据: " << tail << std::endl;
        return true;
    }
    return false;
}
