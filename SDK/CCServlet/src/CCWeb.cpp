#include "CCWeb.h"
#include <stdarg.h>

std::string CCWeb::GetMethod(const char* str)
{
    char buf[10] = { 0 };
    for (int i = 0; i < strlen(str); ++i)
    {
        if(str[i] != ' ')
        {
            buf[i] = str[i];
        }
        else
        {
            break;
        }
    }
    return buf;
}

std::string CCWeb::GetPath(const char* Path)
{
    bool ret = false;
    char buf[50] = { 0 };
    int len = 0;
    for (int i = 0; i < strlen(Path); ++i)
    {
        if(Path[i] == '/' && !ret)
        {
            ret = true;
        }
        else if(ret)
        {
            if(Path[i] != ' ')
            {
                buf[len] = Path[i];
                ++len;
            }
            else
            {
                break;
            }
        }

    }
    return buf;
}

bool CCWeb::SendResources(unsigned int ID, const char *Path, const char *Mode)
{
    FILE *file = fopen(Path, Mode); // 替换为你的文件名
    if (file == NULL)
    {
        return false;
    }
    Client[ID].Send("HTTP/1.1 200 OK\r\n");
    Client[ID].Send("Server:HTTP->CCWeb->OK\r\n");
    char buf[50] = {0};
    sprintf(buf, "Content-type:%s\r\n", GetFileType(Path).c_str());
    Client[ID].Send(buf);
    sprintf(buf, "Content-Length:%d\r\n", GetFileSize(Path));
    Client[ID].Send(buf);//Content-Length:
    Client[ID].Send("\r\n");

    int cout = 0;
    char bufs[4096] = { 0 };
    int fpId = -1;
    struct stat st;
    fpId = fileno( file );
    if( fstat( fpId ,&st) == -1  )
    {
        return false;
    }

    while (!feof(file))
    {
        int ret = fread(bufs,sizeof(char),sizeof bufs,file);
        Client[ID].Send(bufs,sizeof bufs,0);
        cout += ret;
    }
    //Client[ID].Send("\r\n",sizeof "\r\n",0);
    if(PrintBool)
    {
        printf("Send to Clent len:%d\r\n",cout);
    }
    fclose(file);
    return true;
}

bool CCWeb::HTTPStart(const char *IP, int port)
{
    this->SetAddRootPath("");
    lent = PathSix.size();
    Server.Socket(IPVX::IPV4,TORU::TCP);
    if(!Server.Bind(IP,port))
    {
        return false;
    }
    if(!Server.Listen(UserMAX))
    {
        return false;
    }
    Threadappect.SetThread(&CCWeb::Appect,this);
    Threadappect.Start();
    return true;
}

CC_Thread CCWeb::Appect()
{
    while (Threadappect.Sign())
    {

        Client[ThreadCount] = Server.Accept();
        Thread[ThreadCount].SetThread(&CCWeb::ReadThread,this,ThreadCount);
        Thread[ThreadCount].Start();
        ThreadCount++;
        if(ThreadCount >= UserMAX)
        {
            ThreadCount = 0;
        }
    }
    std::cout << "END" << std::endl;
    return Threadappect.Stop();
}

CC_Thread CCWeb::ReadThread(unsigned int ID)
{
    mutexs.lock();
    bool ReadFlag = false;
    ByteHander len = BufferSize;
    char Buffer[BufferSize] = {0};
    HTTPRequests.Bufferstr.clear();
    HTTPRequests.BodyData.clear();
    HTTPRequests.Headers.clear();
    HTTPRequests.FileData.Buffer.clear();
    HTTPRequests.FileData.Headers.clear();
    while (len >= BufferSize)
    {
        len = Client[ID].RecvData(Buffer, sizeof(Buffer));
        for (int i = 0; i < len; ++i) {
            HTTPRequests.Bufferstr.push_back(Buffer[i]);
        }
        memset(Buffer,0, BufferSize);
    }
    std::string RecvMessage(HTTPRequests.Bufferstr.begin(), HTTPRequests.Bufferstr.end());
    HTTPRequests.Bufferstr.clear();
    if (len > 0)
    {
        if (PrintBool)
        {
            std::cout << Buffer << std::endl;
            std::cout<< "ThreadID:" << ID << std::endl;
        }
        RequestProcess Stuart = RequestProcess_First;
        while(true)
        {
            auto strlen = RecvMessage.find("\r\n");
            std::istringstream stream(RecvMessage.substr());
            if(strlen == 0)
            {
                Stuart = RequestProcess_Body;
            }
            if(strlen != std::string::npos)
            {
                if(Stuart == RequestProcess_First)
                {
                    stream >> HTTPRequests.Method;
                    stream >> HTTPRequests.Path;
                    stream >> HTTPRequests.Version;
                    RecvMessage.erase(0,strlen + 2);
                    Stuart = RequestProcess_Header;
                }
                else if(Stuart == RequestProcess_Header)
                {
                    std::string Key,Value;
                    stream >> Key;
                    stream >> Value;
                    Key.erase(remove(Key.begin(),Key.end(),':'),Key.end());
                    HTTPRequests.Headers.insert(std::pair<std::string,std::string>(Key,Value));
                    RecvMessage.erase(0,strlen + 2);
                }
                else if(Stuart == RequestProcess_Body)
                {
                    if(HTTPRequests.Headers["Content-Type"] == "multipart/form-data;")
                    {
                        auto lenstr = HTTPRequests.Headers["Content-Length"];
                        auto FileSize = atoi(lenstr.c_str());
                        while (FileSize > 0)
                        {
                            len = Client[ID].RecvData(Buffer, sizeof(Buffer));
                            for (int i = 0; i < len; ++i) {
                                HTTPRequests.Bufferstr.push_back(Buffer[i]);
                            }
                            memset(Buffer,0, BufferSize);
                            FileSize = FileSize - len;
                        }
                        std::string FileData(HTTPRequests.Bufferstr.begin(), HTTPRequests.Bufferstr.end());
                        auto END = Getboundary(FileData);
                        std::string name,filename;
                        strlen = FileData.find("name=\"");
                        for(int i = strlen + 6;i < FileData.length(); i++)
                        {
                            if(FileData[i] != '\"')
                            {
                                name += FileData[i];
                            }
                            else
                            {
                                break;
                            }
                        }
                        HTTPRequests.FileData.Headers.insert(std::pair<std::string,std::string>("name",name));
                        strlen = FileData.find("filename=\"");
                        for(int i = strlen + 10;i < FileData.length(); i++)
                        {
                            if(FileData[i] != '\"')
                            {
                                filename += FileData[i];
                            }
                            else
                            {
                                break;
                            }
                        }
                        HTTPRequests.FileData.Headers.insert(std::pair<std::string,std::string>("filename",filename));
                        while(true)
                        {
                            strlen = FileData.find("\r\n");
                            std::string Key,Value;
                            std::istringstream stra(FileData.substr());
                            stra >> Key;
                            stra >> Value;
                            Key.erase(remove(Key.begin(),Key.end(),':'),Key.end());
                            HTTPRequests.FileData.Headers.insert(std::pair<std::string,std::string>(Key,Value));
                            FileData.erase(0,strlen + 2);
                            if(strlen == 0)
                            {
                                break;
                            }
                        }
                        strlen = FileData.find("\r\n" + END + "--");
                        for (int i = 0; i < strlen; ++i) {
                            HTTPRequests.FileData.Buffer.push_back(FileData[i]);
                        }
                        HTTPRequests.FileData.Name = HTTPRequests.FileData.Headers["filename"];
                        HTTPRequests.FileData.Type = HTTPRequests.FileData.Headers["name"];
                        break;
                    }
                    else if(HTTPRequests.Headers["Content-Type"] == "application/json")
                    {

                    }
                    else
                    {
                        std::string str;
                        stream >> str;//"username=admin&password=123456"
                        while(true)
                        {
                            strlen = str.find('&');
                            std::string Key,Value;
                            bool FA = false,ENDF = false;
                            if(strlen == std::string::npos)
                            {
                                strlen = str.length();
                                ENDF = true;
                            }
                            for (int i = 0; i < strlen; ++i)
                            {
                                if(str[i] != '=')
                                {
                                    if(FA)
                                    {
                                        Value += str[i];
                                    }
                                    else
                                    {
                                        Key += str[i];
                                    }
                                }
                                else
                                {
                                    FA = true;
                                }
                            }
                            HTTPRequests.BodyData.insert(std::pair<std::string,std::string>(Key,Value));
                            str.erase(0,strlen + 1);
                            if(ENDF)
                            {
                                break;
                            }
                        }
                        break;
                    }

                }
            }
            else
            {
                break;
            }
        }
        HTTPRequests.ID = ID;
        if (HTTPRequests.Method == "GET" || HTTPRequests.Method == "POST" || HTTPRequests.Method == "PUT")
        {
            std::map<std::string, RequestFunc>::iterator t;
            for (t = RequestFun.begin(); t != RequestFun.end(); t++)
            {
                if (t->first == HTTPRequests.Path)
                {
                    t->second(HTTPRequests, this);
                    ReadFlag = false;
                    break;
                }
                else
                {
                    ReadFlag = true;
                }
            }
            if (ReadFlag)
            {
                bool da = false;
                for (int i = 0; i < lent; ++i)
                {
                    if (this->SendResources(ID, std::string(PathSix[i] + HTTPRequests.Path).c_str()))
                    {
                        da = true;
                        break;
                    }
                }
                if (!da)
                {
                    char bufsa[100] = {0};
                    sprintf(bufsa, "无法打开文件%s\n", HTTPRequests.Path.c_str());
                    perror(bufsa);
                }
            }

        }

    }
    else
    {
        Thread[ID].Stop();
    }
    mutexs.unlock();
    Client[ID].Close();
    return Thread[ID].Stop();
}

std::string CCWeb::GetFileSuffix(const std::string& filePath)
{
    size_t dotPosition = filePath.rfind('.'); // 从右向左查找最后一个'.'的位置
    if (dotPosition == std::string::npos)
    {
        return ""; // 如果没有找到'.'，则返回空字符串
    }
    return filePath.substr(dotPosition + 1); // 返回'.'之后的部分，即文件后缀名
}

std::string CCWeb::GetFileType(const std::string &str)
{
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

void CCWeb::SetAddRootPath(const std::string& rootPath)
{
    PathSix.push_back(rootPath);
}

void CCWeb::SetPrint(bool print)
{
    PrintBool = print;
}

std::string CCWeb::Filtration(const char *buf)
{
    char Date[BufferSize] = {0};
    std::string buffer;
    bool Flag = false;
    int len = 0;
    std::string le = buf;
    auto d = le.length();
    for (int i = 0; i < d; ++i)
    {
        if(buf[i] == '\r' && buf[i+1] == '\n' && buf[i+2] == '\r' && !Flag)
        {
            i = i+2;
            Flag = true;
        }
        else if(Flag && buf[i] != '\0')
        {
            Date[len] = buf[i];
            len++;
            if(len == sizeof Date)
            {
                buffer.append(Date);
                memset(Date,'\000',BufferSize);
                len = 0;
            }
        }

    }
    buffer.append(Date);
    return buffer;
}

std::string CCWeb::FiltrationJson(const char *buf)
{
    char Date[BufferSize] = {0};
    std::string buffer;
    bool Flag = false;
    int len = 0;
    std::string le = buf;
    auto d = le.length();
    for (int i = 0; i < d; ++i)
    {
        if(buf[i] == '\r' && buf[i+1] == '\n' && buf[i+2] == '\r' && !Flag)
        {
            i = i+2;
            Flag = true;
        }
        else if(Flag)
        {
            Date[len] = buf[i];
            if(Date[len] == '}')
            {
                Date[len+1] = '\0';
                buffer.append(Date);
                break;
            }
            len++;
            if(len == sizeof Date)
            {
                buffer.append(Date);
                len = 0;
            }
        }

    }
    return buffer;
}

std::string CCWeb::GetData(std::string name, HTTPRequest request)
{
    const HTTPRequest Date = request;
    for(auto i : Date.BodyData)
    {
        if(i.first == name)
        {
            return i.second;
        }
    }
    return "NO_Data";
}

std::string CCWeb::FilterString(const char *str, char Begin, char End)
{
    char Date[BufferSize] = {0};
    bool Flag = false;
    int len = 0;
    for (int i = 0; i < strlen(str); ++i)
    {
        if(str[i] == Begin && !Flag)
        {
            Flag = true;
        }
        if(Flag)
        {
            if(str[i] != End)
            {
                Date[len] = str[i];
                len++;
            }
            else
            {
                break;
            }
        }
    }
    return Date;
}

void CCWeb::SetRequestFunction(std::string RootPath, RequestFunc RFunc)
{
    RequestFun.insert(std::pair<std::string, RequestFunc>(RootPath, RFunc));
}

void CCWeb::HTTPStop()
{
    Threadappect.Stop();
    Server.Close();
    for (int i = 0; i < ThreadCount; ++i)
    {
        Thread[i].Stop();
        CCSocket sc = {};
        if(Client[i] != sc)
        {
            Client[i].Close();
        }
    }
    //Threading::Sleep(1000*5000);
    ThreadCount = 0;
}

std::string CCWeb::GetIP(IPVX port)
{
    return Server.GetlocadIP(port).c_str();
}

std::string CCWeb::GetRequestData(const char * str)
{
    char Date[BufferSize] = {0};
    bool Flag = false;
    int len = 0;
    auto da = strlen(str);
    for (int i = 0; i < da; ++i)
    {
        if(str[i] == '7' && str[i+1] == 'B' && str[i+2] == '%' && str[i+3] == '7' && str[i+4] == 'B' && !Flag)
        {
            Flag = true;
            i = i+4;
        }
        else if(Flag)
        {
            if(str[i] != '%')
            {
                Date[len] = str[i];
                len++;
            }
            else
            {
                break;
            }
        }
    }
    return Date;
}

bool CCWeb::SendResourcesData(unsigned int ID, const char *Data)
{
    Client[ID].Send("HTTP/1.1 200 OK\r\n");
    Client[ID].Send("Server:HTTP->CCWeb->OK\r\n");
    char buf[50] = {0};
    sprintf(buf, "Content-type:text/html\r\n");
    Client[ID].Send(buf);
    sprintf(buf, "Content-Length:%d\r\n", (unsigned int)strlen(Data));
    Client[ID].Send(buf);//Content-Length:
    Client[ID].Send("\r\n");
    Client[ID].Send(Data);
    //Client[ID].Send("\r\n");
    return true;
}

unsigned int CCWeb::GetFileSize(const char *Path)
{
    FILE *file = fopen(Path, "rb");
    if (file == nullptr) {
        perror("Error opening file");
        return 1;
    }
    fclose(file);
    struct stat fileStat{};
    if (stat(Path, &fileStat) == 0)
    {
        return fileStat.st_size;
    }
    else
    {
        perror("Error getting file size");
        return 0;
    }

}

std::string CCWeb::StrSplicing(const char * format,...)
{
    char buf[BufferSize] = {0};
    va_list args;
    va_start(args, format);
    int l = vsnprintf(buf, sizeof(buf), format, args);
    if (l < 0 || l >= sizeof(buf)) {
        return "ERROR";
    }
    va_end(args);
    return buf;
}

bool CCWeb::SendData(unsigned int ID, const char *Data,const char* type)
{
    Client[ID].Send("HTTP/1.1 200 OK\r\n");
    Client[ID].Send("Server:HTTP->CCWeb->OK\r\n");
    char buf[50] = {0};
    sprintf(buf, "Content-type:%s; charset=utf-8\r\n",type);
    Client[ID].Send(buf);
    sprintf(buf, "Content-Length:%zu\r\n", strlen(Data));
    Client[ID].Send(buf);//Content-Length:
    Client[ID].Send("\r\n");
    Client[ID].Send(Data);
    //Client[ID].Send("\r\n");
    return true;
}

template<class name>
name CCWeb::GetHTTPRequestData(const char * Date,std::string Request)
{
    std::string Data = Date, RequestData;
    auto len = Data.find(Request);
    bool f = false;
    for (int i = len; i < Data.length(); ++i) {
        if(Data[i] == ' ' && !f)
        {
            f = true;
        }
        else if(f)
        {
            if(Data[i] != '\r')
            {
                RequestData += Data[i];
            }
            else
            {
                break;
            }
        }
    }
    name n;
    std::istringstream ss(RequestData);
    ss >> n;
    return n;
}

std::string CCWeb::GetHTTPRequestData(const char * Date,std::string Request)
{
    std::string Data = Date, RequestData;
    auto len = Data.find(Request);
    bool f = false;
    for (int i = len; i < Data.length(); ++i) {
        if(Data[i] == ' ' && !f)
        {
            f = true;
        }
        else if(f)
        {
            if(Data[i] != '\r')
            {
                RequestData += Data[i];
            }
            else
            {
                break;
            }
        }
    }
    return RequestData;
}

std::string CCWeb::Getboundary(std::string& FileData)
{
    auto len = FileData.find("\r\n");
    std::string str;
    for(int i = 0; i < len; ++i)
    {
        str += FileData[i];
    }
    FileData.erase(0,len + 2);
    return str;
}


bool HTTPRequestFile::Save(std::string Path) const
{
    std::ofstream file(Path,std::ios::out|std::ios::binary); // 如果文件不存在，则创建它
    // 检查文件是否成功打开
    if (!file.is_open()) {
        std::cerr << "无法打开文件" << std::endl;
        return false;
    }
    std::string str(Buffer.begin(),Buffer.end());
    file.write(str.c_str(),str.size());
    // 关闭文件
    file.close();
    return true;
}
