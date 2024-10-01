#include "TextBox.h"



TextBox::TextBox()
{
    for (size_t i = 0; i < TextSize; i++)
    {
        Txt[i] = '\0';
    }
}

TextBox::TextBox(TextBox&& Other)
{
    for (size_t i = 0; i < TextSize; i++)
    {
        Txt[i] = '\0';
    }
}

void TextBox::Protract()
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
    if (RValue != Text)
    {
        RValue = Text;
        if (task_Click_Value)
        {
            ABCDE = std::thread(task_Click_Value);
            ABCDE.detach();
        }

    }

}

void TextBox::POR()
{
    if(Pos.x != 0 || Pos.y != 0)
    {
        ImGui::SetCursorPos(Pos);
    }
    ImGui::PushStyleColor(ICLR::ImGuiCol_Text, TextColor);
    ImGui::PushStyleColor(ICLR::ImGuiCol_FrameBg, BGColor);
    ImGui::PushStyleColor(ICLR::ImGuiCol_FrameBgActive, BGAColor);
    ImGui::PushStyleColor(ICLR::ImGuiCol_FrameBgHovered, BGHColor);
    auto dada = ImGui::GetStyle().FrameRounding;
    ImGui::GetStyle().FrameRounding = Round;
    ImGui::PushFont(UI_ID::Fons[this->Fontsize]);
    ImGui::PushItemWidth(Width);

    ImGui::InputText(ID.c_str(), Txt, TextSize, ImGuiInputTextFlags_AllowTabInput|Attribute);
    Text = Txt;

    ImGui::GetStyle().FrameRounding = dada;
    ImGui::PopItemWidth();
    ImGui::PopFont();
    ImGui::PopStyleColor(4);
}
