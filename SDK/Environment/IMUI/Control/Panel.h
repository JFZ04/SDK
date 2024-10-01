#ifndef IMGUI_OPENGL_PANEL_H
#define IMGUI_OPENGL_PANEL_H
#pragma once
#include "CApplication.h"

class Panel:CApplication
{

public:
    ImVec2 Size = { 0,0 };
    ImVec2 Pos = { 0,0 };
    int Fontsize = 18;
    int Round = 0;
    ImVec4 BGColor = Background;
    ImVec4 BGHColor = Background;


public:
    Panel(){};
    Panel(Panel&& other){};
    template <typename Func, typename... Args>
    void Protract(Func&& fun, Args&&... args);
    template <typename Func, typename... Args>
    bool Mouse_Hovered(Func&& fun, Args&&... args);
    template<typename Func, typename ...Args>
    bool Click(ImGuiMouseButton Mouse, Func&& fun, Args && ...args);




private:
    ImVec4 BGColors = BGColor;
    std::string ID = "ID_" + UI_ID::UIIDSTR();
    std::function<void()> task_Click_L;
    std::function<void()> task_Click_Mouse;
    std::function<void()> task_Click_R;
    std::function<void()> task_Click_M;
    std::thread A;
    std::thread AB;
    std::thread ABC;
    std::thread ABCD;



};

template <typename Func, typename... Args>
inline void Panel::Protract(Func&& fun, Args&&... args)
{
    if (BGHColor.x == Background.x && BGHColor.y == Background.y &&
    BGHColor.z == Background.z && BGColor.w == Background.w)
    {
        BGHColor = BGColor;
    }
    if(Pos.x != 0 || Pos.y != 0)
    {
        ImGui::SetCursorPos(Pos);
    }
    ImGui::PushStyleColor(ICLR::ImGuiCol_ChildBg,ImVec4(BGColors));
    ImGui::PushStyleVar(IMSV::ImGuiStyleVar_ChildRounding,Round);
    ImGui::PushStyleVar(IMSV::ImGuiStyleVar_FramePadding,{0,0});
    ImGui::PushFont(UI_ID::Fons[this->Fontsize]);
    if(Size.x != 0 || Size.y != 0)
    {
        ImGui::BeginChild(ID.c_str(),Size);
    }
    else
    {
        ImGui::BeginChild(ID.c_str());
    }
    fun(args...);
    ImGui::EndChild();
    ImGui::PopFont();
    ImGui::PopStyleVar(2);
    ImGui::PopStyleColor();
    if (ImGui::IsItemHovered())
    {
        BGColors = BGHColor;
        if (task_Click_Mouse)
        {
            A = std::thread(task_Click_Mouse);
            A.detach();
        }
    }
    else
    {
        BGColors = BGColor;
    }
    if (ImGui::IsMouseClicked(IMBN::ImGuiMouseButton_Left) && ImGui::IsItemHovered())
    {
        if (task_Click_L)
        {
            AB = std::thread(task_Click_L);
            AB.detach();
        }

    }
    if (ImGui::IsMouseClicked(IMBN::ImGuiMouseButton_Right) && ImGui::IsItemHovered())
    {
        if (task_Click_R)
        {
            ABC = std::thread(task_Click_R);
            ABC.detach();
        }
    }
    if (ImGui::IsMouseClicked(IMBN::ImGuiMouseButton_Middle) && ImGui::IsItemHovered())
    {
        if (task_Click_M)
        {
            ABCD = std::thread(task_Click_M);
            ABCD.detach();
        }
    }

}

template<typename Func, typename ...Args>
inline bool Panel::Mouse_Hovered(Func&& fun, Args && ...args)
{
    task_Click_Mouse = std::bind(std::forward<Func>(fun), std::forward<Args>(args)...);
    return true;
}

template<typename Func, typename ...Args>
inline bool Panel::Click(ImGuiMouseButton Mouse, Func&& fun, Args && ...args)
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

#endif
