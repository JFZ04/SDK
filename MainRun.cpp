#include "MainRun.h"
#include "CCMySql.h"

bool MainRun::Init() {
    CORS Cors = CORS();
    Cors.AddAllowOrigin("*");
    m_servlet.SetCorsConfig(Cors);
    m_servlet.SetAddRootPath("../static/Html");
    m_servlet.SetAddRootPath("../static/Html/assets");
    m_servlet.SetAddRootPath("../static/File");
    m_servlet.SetThreadNumber(20);
    m_servlet.SetWebServletFun([this](CCRequest& request,CCResponse& response){this->NoServletFun(request,response);});
    m_servlet.SetWebServlet("/", CC_ServletFunClass(Roots));
    m_servlet.SetWebServlet("/Ping", [this](CCRequest& request,CCResponse& response){this->Ping(request,response);});
    m_servlet.SetWebServlet("/login", [this](CCRequest& request,CCResponse& response){this->login(request,response);});
    m_servlet.SetWebServlet("/Home25", [this](CCRequest& request,CCResponse& response){this->Home25(request,response);});
    m_servlet.SetWebServlet("/Home25/FileAdd", [this](CCRequest& request,CCResponse& response){this->FileAdd(request,response);});
    m_servlet.SetWebServlet("/Home25/File", [this](CCRequest& request,CCResponse& response){this->File(request,response);});
    m_servlet.SetWebServlet("/Home25/Delete", [this](CCRequest& request,CCResponse& response){this->FileDelete(request,response);});
    m_servlet.SetWebServlet("/Home25/AddFolder", [this](CCRequest& request,CCResponse& response){this->FolderAdd(request,response);});
    m_servlet.SetWebServlet("/Home25/Resvie", [this](CCRequest& request,CCResponse& response){this->ResFile(request,response);});
    m_servlet.SetWebServlet("/Home25/FileDelete", [this](CCRequest& request,CCResponse& response){this->FileDeleteD(request,response);});
    m_servlet.SetWebServlet("/Download", CC_ServletFunClass(FileDownload));
    return m_servlet.Start("0.0.0.0",10065);
}

int MainRun::Run() {//前后端分离开发
    while (m_servlet.Sign()){
        CCThread::Sleep(1000 * 1000);
    }
    return 0;
}
void MainRun::Ping(CCRequest &request, CCResponse &response) {
    response.GetWriter().Write("OK");
}
void MainRun::login(CCRequest &request, CCResponse &response) {
    BufferReader in = request.GetReader();
    CCJSONObject json = request.GetJson(in);
    CCString User = json.get("User");
    CCString Password = json.get("Password");
    if(User == "Admin" && Password == "Admin"){
        response.GetWriter().Write("OK");
    }
    else{
        response.GetWriter().Write("Error");
    }
}
void MainRun::Home25(CCRequest &request, CCResponse &response) {
    BufferReader in = request.GetReader();
    auto Json = JSON::parse(in.Body);
    CCString str = Json["Path"];
    CCFile File = CCFile(CCString(Paths + "/" + str).c_str());
    auto FData = File.GetDirectoryList();
    std::vector<JSON> Array;
    for (int i = 0; i < FData.size(); i++) {
        JSON J;
        J["ID"] = i;
        CCString Name = CCEncode::Format(FData[i].Name,Encode.GBK,Encode.UTF8);
        J["Name"] = Name;
        J["Class"] = FData[i].Directory ? 0 : 1;
        J["Size"] = FData[i].Size;
        Array.push_back(J);
    }
    JSON JsonData;
    JsonData["tableData"] = Array;
    response.GetWriter().Write(JsonData.dump());
}
void MainRun::FileAdd(CCRequest &request, CCResponse &response) {
    BufferReader in = request.GetReader();
    auto Json = request.GetJson(in);
    CCString Path = Json["Path"];
    std::vector<JSON> JArray = Json["Name"];
    for (auto & i : JArray) {
        CCString name = i["name"];
        CCString str = BuffPaths + "/" + name;
        CCString str2 = Paths + "/" + Path + "/" + name;
        CCFile File = CCFile(str.c_str(), CC::ios::rb, false);
        CCString FA = CCFile::GetnormalizePath(str2);
        File.Move(FA);
    }
    response.GetWriter().Write("OK");
}
void MainRun::File(CCRequest &request, CCResponse &response) {
    InPutStream in = request.GetInputStream();
    CCString Name = in.Get("filename").ToString();
    Name = Name.Format(Encode.GBK);
    in.WriteBufferToFile(BuffPaths + "/" + Name);
    response.GetWriter().Write("OK");
}
void MainRun::FileDelete(CCRequest &request, CCResponse &response) {
    BufferReader in = request.GetReader();
    auto Json = request.GetJson(in);
    CCString Name = Json["Name"];
    CCString str = Paths + "/" + Name;
    CCFile File = CCFile(str.c_str(), CC::ios::rb, false);
    bool F = File.Deleted();
    response.GetWriter().Write(F ? "OK" : "NO");
}
void MainRun::FolderAdd(CCRequest &request, CCResponse &response) {
    BufferReader in = request.GetReader();
    auto Json = request.GetJson(in);
    CCString Path = Json["Path"];
    CCString str = Paths + "/" + Path;
    bool F = CCFile::CreateFolder(str);
    response.GetWriter().Write(F ? "OK" : "NO");
}
void MainRun::ResFile(CCRequest &request, CCResponse &response) {
    BufferReader in = request.GetReader();
    auto Json = request.GetJson(in);
    CCString PathOld = Json["PathOld"];
    CCString PathNew = Json["PathNew"];
    CCString A = CCFile::GetnormalizePath(Paths + "/" + PathOld);
    CCString B = CCFile::GetnormalizePath(Paths + "/" + PathNew);
    bool F = CCFile::Revise(A,B);
    response.GetWriter().Write(F ? "OK" : "NO");
}
void MainRun::Roots(CCRequest &req, CCResponse &res) {
    bool F = res.GetWriter().HtmlWrite("../static/Html/index.html");
    if(!F){
        std::cout << "404" << std::endl;
    }
}
void MainRun::FileDeleteD(CCRequest &request, CCResponse &response){
    BufferReader in = request.GetReader();
    auto Json = request.GetJson(in);
    if(Json != nullptr && !Json.is_null()){
        CCString Name = Json["Name"];
        CCString Path = BuffPaths + "/" + Name;
        CCFile File = CCFile(Path.c_str(), CC::ios::rb, false);
        bool F = File.Deleted();
        response.GetWriter().Write(F ? "OK" : "NO");
    }
    else{
        response.GetWriter().Write("NO");
    }
}
void MainRun::WebSocketFun() {

}

void MainRun::NoServletFun(CCRequest &request, CCResponse &response) {
    std::cout << "404->" << request.GetReader().Path << std::endl;
    response.GetWriter().Send("HTTP/1.1 404 OK\r\n\r\n");
}

void MainRun::FileDownload(CCRequest &request, CCResponse &response) {
    CCString Path = request.GetParameter("File");
    CCString str = Paths + "/" + Path;
    response.GetWriter().FileDownload(str);
}



