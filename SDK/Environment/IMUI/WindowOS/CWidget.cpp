#include "CWidget.h"

#include <utility>
#define STB_IMAGE_IMPLEMENTATION  //必须加上
#include "../ImGui/stb_image.h"

#ifdef _WIN32

HBITMAP CreateIconFrom(unsigned char* rgbaImage,int width,int height)
{
    // 创建DC和位图信息头
    HDC hdcMem = CreateCompatibleDC(nullptr);
    BITMAPINFO bmi = { 0 };
    bmi.bmiHeader.biSize = sizeof(BITMAPINFOHEADER);
    bmi.bmiHeader.biWidth = width;
    bmi.bmiHeader.biHeight = -height; // 负值表示位图从上到下
    bmi.bmiHeader.biPlanes = 1;
    bmi.bmiHeader.biBitCount = 32;
    bmi.bmiHeader.biCompression = BI_RGB;
    bmi.bmiHeader.biSizeImage = 0;
    bmi.bmiHeader.biClrUsed = 0;
    bmi.bmiHeader.biClrImportant = 0;

    // 创建位图
    HBITMAP hBitmap = CreateDIBSection(hdcMem, reinterpret_cast<const BITMAPINFO *>(&bmi.bmiHeader), DIB_RGB_COLORS, (void**)&rgbaImage, nullptr, 0);
    if (!hBitmap) {
        stbi_image_free(rgbaImage);
        DeleteDC(hdcMem);
        return nullptr;
    }

    // 选择位图到内存DC
    HBITMAP hOldBitmap = (HBITMAP)SelectObject(hdcMem, hBitmap);

    // 清理
    SelectObject(hdcMem, hOldBitmap);
    DeleteDC(hdcMem);

    return hBitmap;
}

#else
#endif

CWidget::CWidget(CWidget *parent)
{


}

void CWidget::Show(CApplication* pApplication)
{
#ifdef _WIN32
#else
#endif
    if(pApplication != nullptr)
    {
        this->argc = pApplication->argc;
        this->argv = pApplication->argv;
        pApplication->window = this->window;
    }
    WindowCreateFlag = WindowCreate();
    if(NoFrameFlag)
    {
        SetRound(Round);
    }
    UI_ID::Font_Init(18);
    ImGui::GetStyle().WindowPadding = {0,0};
    ImGui::GetStyle().ItemSpacing = {0,0};
    ImGui::GetStyle().WindowBorderSize = 0;
    //onInit();
    if(Fun_onInit)
    {
        Fun_onInit();
    }
    if (datas && Iconbool)
    {
        Icon[0].height = height;
        Icon[0].width = width;
        Icon[0].pixels = datas;
        Icon[1].height = height;
        Icon[1].width = width;
        Icon[1].pixels = datas;
        glfwSetWindowIcon(window, 2, Icon);
        stbi_image_free(datas);
    }
    else
    {
        // 图像加载失败
        printf("Icon loading Eorr!\n");
    }
    InitGL();
    while(NewFrame())
    {
        Framequeue = false;
        funcMutex.lock();
        while (!funcQueue.empty())
        {
            auto func = funcQueue.front();
            func();
            funcQueue.pop();
        }
        funcMutex.unlock();
        Framequeue = true;
        glfwPollEvents();
        ImGui_ImplOpenGL3_NewFrame();
        ImGui_ImplGlfw_NewFrame();
        ImGui::NewFrame();
        if(IsWindowFlag)
        {
            ImGui::SetNextWindowPos({0,0});
            this->Size = GetSize();
            if(this->ReSize.width != Size.width && this->ReSize.height != Size.height)
            {
                this->Size = this->Size = this->GetSize();
                this->ReSize = Size;
                if(Fun_ReSize)
                {
                    std::thread(Fun_ReSize).detach();
                }
            }
            ImGui::PushStyleColor(ImGuiCol_WindowBg,this->Background);
            ImGui::PushStyleColor(ImGuiCol_ChildBg,this->Background);
            ImGui::PushStyleVar(ImGuiStyleVar_WindowRounding,(float)Round);
            ImGui::PushStyleVar(ImGuiStyleVar_ChildRounding,(float)Round);
            ImGui::SetNextWindowSize({this->Size.width,this->Size.height});
            ImGui::Begin(Title.c_str(),nullptr,ImGuiWindowFlags_NoTitleBar
                                             | ImGuiWindowFlags_NoBackground | ImGuiWindowFlags_NoResize);
            ImGui::BeginChild(std::string("_AD" + Title).c_str() ,{this->Size.width,this->Size.height});
        }
        KeyEventHandler();
        //onParent();
        if(Fun_onParent)
        {
            Fun_onParent();
        }
        if(IsWindowFlag)
        {
            ImGui::EndChild();
            if (ImGui::IsMouseClicked(IMBN::ImGuiMouseButton_Left) && ImGui::IsItemHovered())
            {
                if (task_Click_L)
                {
                    AB = std::thread(task_Click_L);
                    AB.detach();
                }

            }
            else if (ImGui::IsMouseClicked(IMBN::ImGuiMouseButton_Right) && ImGui::IsItemHovered())
            {
                if (task_Click_R)
                {
                    ABC = std::thread(task_Click_R);
                    ABC.detach();
                }

            }
            else if (ImGui::IsMouseClicked(IMBN::ImGuiMouseButton_Middle) && ImGui::IsItemHovered())
            {
                if (task_Click_M)
                {
                    ABCD = std::thread(task_Click_M);
                    ABCD.detach();
                }

            }
            ImGui::End();
            ImGui::PopStyleVar(2);
            ImGui::PopStyleColor(2);
            MouseXY = ImGui::GetMousePos();
            if(ImGui::IsMouseReleased(IMBN::ImGuiMouseButton_Left))
            {
                mouseButtonPressed = 0;
            }
        }
        Render();
        Threading::Sleep(this->FrameTime);
    }
    SwapBuffers();
}

void CWidget::SetSize(HSize size)
{
   this->Size = size;
#ifdef _WIN32
#else
#endif
    if(window != nullptr)
    {
        glfwSetWindowSize(this->window,size.width,size.height);
    }
}

void CWidget::SetPosition(HPos position)
{
    this->Pos = position;
#ifdef _WIN32
#else
#endif
    if(window != nullptr)
    {
        glfwSetWindowPos(window,Pos.X,Pos.Y);
    }
}

void CWidget::SetOpacity(int opacity)
{
    this->opacity = (float)opacity;
    if(!NoFrameFlag) {
#ifdef _WIN32
#else
#endif
        if(window != nullptr)
        {
            //glfwSetWindowOpacity(window,this->opacity);
            auto da = this->Background;
            this->Background = RGBA(255,255,255,opacity);
            this->Background.x = da.x;
            this->Background.y = da.y;
            this->Background.z = da.z;
        }
    }
}

HPos CWidget::GetPosition()
{
#ifdef _WIN32
#else
#endif
    if(window != nullptr)
    {
        int xx,yy;
        glfwGetWindowPos(window,&xx,&yy);
        this->Pos.X = xx;
        this->Pos.Y = yy;
    }
    return this->Pos;
}

HSize CWidget::GetSize()
{
#ifdef _WIN32
#else
#endif
    if(window != nullptr)
    {
        int xx,yy;
        glfwGetWindowSize(window,&xx,&yy);
        this->Size = {(float)xx,(float)yy};
    }
    return this->Size;
}

bool CWidget::SetIcon(const unsigned char* Mem,const int MemSize)
{
    datas = stbi_load_from_memory(Mem, MemSize, &width, &height, &nrChannels, 3);
#ifdef _WIN32
    hicon = (HICON)CreateIconFrom(datas,width,height);
#else

#endif
    Iconbool = true;
    return true;
}

void CWidget::MouveWindow()
{
#ifdef _WIN32
    auto hwnd = glfwGetWin32Window(window);
    ::GetWindowRect(hwnd, &r1);
    ::GetCursorPos(&dragStartPos);
    XY.x = (float)(dragStartPos.x - r1.left);
    XY.y = (float)(dragStartPos.y - r1.top);
    mouseButtonPressed = true;
    int w = Size.width;
    int h = Size.height;
    std::thread([this,w,h,hwnd]()
    {
        while (mouseButtonPressed)
        {
            // 计算鼠标移动的距离
            ::GetCursorPos(&dragStartPos);
            this->Pos.X = (float)((float)dragStartPos.x - XY.x);
            this->Pos.Y = (float)((float)dragStartPos.y - XY.y);
            ::MoveWindow(hwnd,  (int)this->Pos.X, (int)this->Pos.Y, w, h, TRUE);
        }
    }).detach();
#else
    // 获取当前鼠标位置和窗口位置
    double xpos, ypos;
    glfwGetCursorPos(window, &xpos, &ypos);
    int windowX, windowY;
    glfwGetWindowPos(window, &windowX, &windowY);

    // 保存初始位置和窗口位置
    mouseInitialX = (int)xpos;
    mouseInitialY = (int)ypos;
    windowLastX = windowX;
    windowLastY = windowY;
    mouseButtonPressed = 1;

    std::thread([this]()
                {
                    while (mouseButtonPressed)
                    {
                        // 计算鼠标移动的距离
                        int dx = (int)MouseXY.x - mouseInitialX;
                        int dy = (int)MouseXY.y - mouseInitialY;

                        // 应用阻尼来平滑移动
                        int newX = windowLastX + (int)(dx * damping);
                        int newY = windowLastY + (int)(dy * damping);
                        display = glfwGetX11Display();
                        glfwSetWindowPos(window, newX, newY);
                        windowLastX = newX;
                        windowLastY = newY;
#ifdef __linux__
                        std::this_thread::sleep_for(std::chrono::milliseconds(25));
#endif
                    }
                }).detach();
    while(mouseButtonPressed);
#endif

}

void CWidget::SetRound(int x)
{
    if(NoFrameFlag)
    {
        Round = x;
    }

}

void CWidget::SetBackground(RGBA rgba)
{
    this->Background = rgba;
}

RGBA CWidget::GetBackground()
{
    return this->Background;
}

void CWidget::SetNoFrame()
{
    this->NoFrameFlag = true;
}

unsigned char* CWidget::LoadIMGMemory(char const *filename, int *x, int *y, int *comp, int req_comp)
{
    return stbi_load(filename,x,y,comp,req_comp);
}

unsigned char* CWidget::LoadIMGMemory(const unsigned char *buffer, int len, int *x, int *y, int *comp, int req_comp)
{
    return stbi_load_from_memory(buffer,len,x,y,comp,req_comp);
}

void CWidget::SetDamping(float d)
{
    this->damping = d;
}

void CWidget::Close()
{
    if(Fun_Close)
    {
        Fun_Close();
    }
    exit(0);
}

void CWidget::SetCenter()
{
#ifdef _WIN32
    // 获取屏幕尺寸
    int screenWidth = GetSystemMetrics(SM_CXSCREEN);
    int screenHeight = GetSystemMetrics(SM_CYSCREEN);

    // 计算窗口位置和大小
    int windowWidth = (int)Size.width;
    int windowHeight = (int)Size.height;
    int windowX = (screenWidth - windowWidth) / 2;
    int windowY = (screenHeight - windowHeight) / 2;
    this->SetPosition({(float)windowX, (float)windowY});
#else
    if(WindowCreateFlag)
    {
        int width = Size.width, height = Size.height;
        int xpos = (mode->width - width) / 2;
        int ypos = (mode->height - height) / 2;
        SetPosition({(float)xpos,(float)ypos});
    }
    else
    {
        CenterFlag = true;
    }
#endif
}

bool CWidget::SetIcon(const char* path)
{
#ifdef _WIN32
    hicon = (HICON)LoadImage( // returns a HANDLE so we have to cast to HICON
            nullptr,             // hInstance must be nullptr when loading from a file
            path,   // the icon file name
            IMAGE_ICON,       // specifies that the file is an icon
            0,                // width of the image (we'll specify default later on)
            0,                // height of the image
            LR_LOADFROMFILE|  // we want to load a file (as opposed to a resource)
            LR_DEFAULTSIZE|   // default metrics based on the type (IMAGE_ICON, 32x32)
            LR_SHARED         // let the system release the handle when it's no longer used
    );
#else
    datas = stbi_load(path, &width, &height, &nrChannels, 0);
#endif
    Iconbool = true;
    return true;
}

void CWidget::SetNewFrameTime(int ms)
{
    this->FrameTime = ms;
}

void CWidget::KeyEventHandler()
{
#ifdef IMGUI_DISABLE_OBSOLETE_KEYIO
    struct funcs { static bool IsLegacyNativeDupe(ImGuiKey) { return false; } };
#else
    struct funcs
    {
        static bool IsLegacyNativeDupe(ImGuiKey key)
        {
            return key >= 0 && key < 512 && ImGui::GetIO().KeyMap[key] != -1;
        }
    };
#endif
    for (ImGuiKey key = ImGuiKey_KeysData_OFFSET; key < ImGuiKey_COUNT; key = (ImGuiKey)(key + 1))
    {
        if (funcs::IsLegacyNativeDupe(key) || !ImGui::IsKeyDown(key))
            continue;
        if(DefaultKeyFun)
        {
            std::thread(DefaultKeyFun,ImGui::GetKeyName(key),KeyMode::KeyDown).detach();
        }

    }
    for (ImGuiKey key = ImGuiKey_KeysData_OFFSET; key < ImGuiKey_COUNT; key = (ImGuiKey)(key + 1))
    {
        if (funcs::IsLegacyNativeDupe(key) || !ImGui::IsKeyPressed(key))
            continue;
        if(DefaultKeyFun)
        {
            std::thread(DefaultKeyFun,ImGui::GetKeyName(key),KeyMode::KeyPressed).detach();
        }

    }
    for (ImGuiKey key = ImGuiKey_KeysData_OFFSET; key < ImGuiKey_COUNT; key = (ImGuiKey)(key + 1))
    {
        if (funcs::IsLegacyNativeDupe(key) || !ImGui::IsKeyReleased(key))
            continue;
        if(DefaultKeyFun)
        {
            std::thread(DefaultKeyFun,ImGui::GetKeyName(key),KeyMode::KeyReleased).detach();
        }

    }

}

void CWidget::SetWindowTitle(CCString title) {
    this->Title = std::move(title);
}




