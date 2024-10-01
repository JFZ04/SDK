#include "Demo.h"

Demo::Demo(CWidget *parent) : CWidget(parent) {
    ui->Setup(this);
    this->SetCenter();
    this->SetWindowTitle("ImGuiWindow");
    threatening->SetThread(&Demo::Init,this);
    threatening->Start();
    mainRun.Init();
    thread->SetThread(&Demo::SelvetThread,this);
    thread->Start();
    this->Click(IMBN::ImGuiMouseButton_Left,&Demo::MouveWindow,this);
}

Demo::~Demo() {
    delete ui;
}

void Demo::SelvetThread() {
    mainRun.Run();
}

void Demo::Init() {
    while(threatening->Sign()){
        this->InvokeMethod([this](){
//            ui->label->Text = to_string(ImGui::GetIO().Framerate);
            ui->label->Text = to_string(mainRun.m_servlet.GetServletThreadPool().GetUnusedCount());
        });
    }
}

