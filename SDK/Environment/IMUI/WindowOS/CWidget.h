#ifndef IMGUI_CWIDGET_H
#define IMGUI_CWIDGET_H
#include "CApplication.h"
#include "../../../include/CCThread.h"

#define C_Key1 std::placeholders::_1
#define C_Key2 std::placeholders::_2

class CWidget:public CApplication
{
private:
    virtual void onInit(){};
    virtual void onParent(){};
    void KeyEventHandler();
    GLFWimage Icon[2] = { 0 };
    // 全局变量，用于跟踪鼠标状态和窗口移动
    int mouseInitialX = 0;
    int mouseInitialY = 0;
    int windowLastX = 0;
    int windowLastY = 0;
#ifdef _WIN32
    POINT dragStartPos;
    RECT r1;
    HICON hicon;
#else



#endif
    ImVec2 MouseXY,XY;
    bool mouseButtonPressed = false,Iconbool = false,Framequeue = false,Funlook = true;
    double damping = 0.5; // 阻尼系数，用于平滑移动
    int Round = 0,FrameTime = 10;
    unsigned char* datas;int width, height, nrChannels;
    CApplication* application = nullptr;
    std::function<void()> task_Click_L;
    std::function<void()> task_Click_R;
    std::function<void()> task_Click_M;
    std::function<void()> Fun_Close;
    std::function<void()> Fun_onInit;
    std::function<void()> Fun_onParent;
    std::function<void()> Fun_ReSize;
    std::function<void(std::string Keyboard,KeyMode KeyMode)> DefaultKeyFun;
    std::thread AB;
    std::thread ABC;
    std::thread ABCD;
    std::queue<std::function<void()>> funcQueue;
    std::mutex funcMutex;



public:
#ifdef _WIN32

#elif __linux__

#endif
    void SetSize(HSize size);
    void SetPosition(HPos position);
    void SetOpacity(int opacity);
    HPos GetPosition();
    HSize GetSize();
    bool SetIcon(const unsigned char* Mem,const int MemSize);
    bool SetIcon(const char* path);
    void MouveWindow();
    void SetRound(int x);
    void SetBackground(RGBA rgba);
    RGBA GetBackground();
    void SetNoFrame();
    void SetDamping(float d);
    void Close();
    void SetCenter();
    void SetNewFrameTime(int ms);
    void SetWindowTitle(CCString title);





public:
    explicit CWidget(CWidget* parent = nullptr);
    //------------------------------------------------------------------
    virtual void Show(CApplication* pApplication = nullptr);
    unsigned char* LoadIMGMemory(char const *filename, int *x, int *y, int *comp, int req_comp);
    unsigned char* LoadIMGMemory(const unsigned char *buffer, int len, int *x, int *y, int *comp, int req_comp);
    bool IsWindowFlag = true;
    //------------------------------------------------------------------
    template <typename Func, typename... Args>
    bool Close_Click(Func&& fun, Args&&... args);
    template <typename Func, typename... Args>
    bool Click(ImGuiMouseButton Mouse,Func&& fun, Args&&... args);
    template <typename Func>
    bool SetOnInit(Func&& fun);
    template <typename Func>
    bool SetOnParent(Func&& fun);
    template <typename Func, typename... Args>
    bool ReSizeClick(Func&& fun, Args&&... args);
    template <typename Func, typename... Args>
    bool SetKeyboardClick(Func&& fun, Args&&... args);
    template <typename Func, typename... Args>
    void InvokeMethod(Func&& fun, Args&&... args);

};

template <typename Func, typename... Args>
inline bool CWidget::Close_Click(Func&& fun, Args&&... args)
{
    Fun_Close = std::bind(std::forward<Func>(fun), std::forward<Args>(args)...);
    return true;
}

template <typename Func, typename... Args>
inline bool CWidget::Click(ImGuiMouseButton Mouse,Func&& fun, Args&&... args)
{
    if (Mouse == IMBN::ImGuiMouseButton_Left)
    {
        task_Click_L = std::bind(std::forward<Func>(fun), std::forward<Args>(args)...);
    }
    else if (Mouse == IMBN::ImGuiMouseButton_Right)
    {
        task_Click_R = std::bind(std::forward<Func>(fun), std::forward<Args>(args)...);
    }
    else if (Mouse == IMBN::ImGuiMouseButton_Middle)
    {
        task_Click_M = std::bind(std::forward<Func>(fun), std::forward<Args>(args)...);
    }
    return true;
}

template <typename Func>
inline bool CWidget::SetOnParent(Func&& fun)
{
    Fun_onParent = std::bind(std::forward<Func>(fun));
    return true;
}

template <typename Func>
inline bool CWidget::SetOnInit(Func&& fun)
{
    Fun_onInit = std::bind(std::forward<Func>(fun));
    return true;
}

template <typename Func, typename... Args>
inline bool CWidget::ReSizeClick(Func&& fun, Args&&... args)
{
    Fun_ReSize = std::bind(std::forward<Func>(fun), std::forward<Args>(args)...);
    return true;
}

template <typename Func, typename... Args>
inline bool  CWidget::SetKeyboardClick(Func&& fun, Args&&... args)
{
    DefaultKeyFun = std::bind(std::forward<Func>(fun), std::forward<Args>(args)...);
    return true;
}

template <typename Func, typename... Args>
inline void CWidget::InvokeMethod(Func&& fun, Args&&... args)
{
    if(Framequeue)
    {
        Threading::Sleep(1000*16);
        if(funcMutex.try_lock())
        {
            //Funlook = false;
            funcQueue.push(std::bind(std::forward<Func>(fun), std::forward<Args>(args)...));
            funcMutex.unlock();
        }
    }
}

#endif
