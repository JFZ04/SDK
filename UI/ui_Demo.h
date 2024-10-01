#ifndef THREADPOOLDEMO_UI_DEMO_H
#define THREADPOOLDEMO_UI_DEMO_H
#include "CWidget.h"
#include "Contorls.h"

class UIDemo:public CWidget
{
public:
    Label* label;
    Label* label1;


public:
    void Setup(CWidget* UI)
    {
        UI->SetOnInit([this](){this->onInit();});
        UI->SetOnParent([this](){this->onParent();});
        UI->SetPosition({0, 0});
        UI->SetSize({800,600});

        label = new Label();
        label->Pos = {140,5};
        label->Text = "FPS:114";
        label->TextColor = RGBA(0,0,0,255);

        label1 = new Label();
        label1->Pos = {5,5};
        label1->Text = "当前线程空闲数:";
        label1->TextColor = RGBA(0,0,0,255);


    }
    void onParent()
    {
        label1->Protract();
        label->Protract();

    }

    void onInit()
    {
//        UI_ID::Font_Init(25);


    }




};


#endif
