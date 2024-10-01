#ifndef IMGUI_CAPPLICATION_H
#define IMGUI_CAPPLICATION_H

#include "imgui.h"
#include "imgui_impl_opengl3.h"
#include "imgui_internal.h"
#include "iostream"
#include "../../../include/CCString.h"
#include "OSCA.h"


static void glfw_error_callback(int error, const char* description)
{
    fprintf(stderr, "GLFW Error %d: %s\n", error, description);
}


class CApplication
{
public:
#ifdef _WIN32
    GLFWwindow* window;
    GLFWvidmode* mode;
#elif __linux__
    GLFWwindow* window;
    GLFWvidmode* mode;
    Window x11Window;
    Display* display;
    int screen;
#endif


public:
    HSize Size = {0,0};
    HSize ReSize = {0,0};
    HPos Pos = {0,0};
    std::string Title = "ImGui";
    bool WindowCreateFlag = false,NoFrameFlag = false,CenterFlag = false;
    ImVec4 Background = RGBA(255,255,255,255);
    ImVec4 ClearGround = RGBA(0,0,0,0);
    int argc = 0;
    std::string* argv;
    float opacity;


public:
    CApplication();
    CApplication(CApplication&& other) noexcept ;
    CApplication(int argcs,char** argv) noexcept ;
    explicit CApplication(CApplication* CApp);
    //----------------------------------------------------------------
    bool WindowCreate();
    void InitGL();
    virtual void StyleWindow(){};
    bool NewFrame();
    void Render();
    int SwapBuffers();
    int Stop();
    //----------------------------------------------------------------


private:
    std::string glsl_version;
    bool closea = false;





};


#endif
