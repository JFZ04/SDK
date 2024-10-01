#define  __Drive_H__
#ifdef   __Drive_H__
#pragma once


#define IMGUI_DEFINE_MATH_OPERATORS
#define IMUI ImGui
#define Font_MAX 200

#include "../WindowOS/OSCA.h"
#include "../ImGui/Font.h"
#include "../ImGui/imgui.h"
#include "../ImGui/imgui_impl_glfw.h"
#include "../ImGui/imgui_impl_opengl3.h"
#include "../ImGui/imgui_internal.h"
#include "../ImGui/imstb_rectpack.h"
#include "../ImGui/imstb_textedit.h"
#include "../ImGui/imstb_truetype.h"


#include <iostream>
#include <map>
#include <vector>
#include <thread>
#include <functional>

typedef ImGuiConfigFlags_ IOCFG;
typedef ImColor RGBA;
typedef ImFont* Font_t;
typedef ImGuiWindowFlags_ IWFS;
typedef ImGuiCol_ ICLR;
typedef ImGuiMouseButton_ IMBN;
typedef ImGuiStyleVar_ IMSV;

namespace UI_ID 
{
    static int UIIDint = 0;
    extern void Font_Init(int FSizeItem);
	std::string UIIDSTR();
	extern Font_t* Fons;
	extern bool ButtonEx(const char* label, const char* ID, const ImVec2& size_arg, ImGuiButtonFlags flags);

}




















#else
#pragma once




#endif





