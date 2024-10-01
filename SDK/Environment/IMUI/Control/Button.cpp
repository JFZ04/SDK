#include "Button.h"

Button::Button()
{

}

Button::Button(Button&& Other)
{

}

void Button::Protract()
{
	POR();
    if (ImGui::IsItemHovered())
    {
        if (task_Click_Mouse)
        {
            A = std::thread(task_Click_Mouse);
            A.detach();
        }
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

void Button::POR()
{
    if(Pos.x != 0 || Pos.y != 0)
    {
        ImGui::SetCursorPos(Pos);
    }
	ImGui::PushFont(UI_ID::Fons[this->Fontsize]);
	ImGui::PushStyleColor(ICLR::ImGuiCol_Button,BtnColor);
	ImGui::PushStyleColor(ICLR::ImGuiCol_ButtonActive,BtnAColor);
	ImGui::PushStyleColor(ICLR::ImGuiCol_ButtonHovered,BtnHColor);
    ImGui::PushStyleColor(ICLR::ImGuiCol_Text,TextColor);
    auto S1 = ImGui::GetStyle().FrameRounding;
    ImGui::GetStyle().FrameRounding = Round;
	UI_ID::ButtonEx(Text.c_str(),ID.c_str(),Size,Attribute);
    ImGui::GetStyle().FrameRounding = S1;
	ImGui::PopStyleColor(4);
	ImGui::PopFont();
}
