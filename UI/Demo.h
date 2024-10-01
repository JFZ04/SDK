#ifndef THREADPOOLDEMO_DEMO_H
#define THREADPOOLDEMO_DEMO_H
#include "../MainRun.h"
#include "ui_Demo.h"
#include "CCByteArray.h"
#include "CCFIleInStream.h"
#include "CCNetwork.h"

class Demo:public CWidget
{
public:
    Demo(CWidget* parent = nullptr);
    ~Demo();
    void SelvetThread();
    void Init();

private:
    UIDemo* ui = new UIDemo();
    MainRun mainRun;
    CCThread *thread = new CCThread();
    CCThread *threatening = new CCThread();
    CCNetwork net;

};


#endif
