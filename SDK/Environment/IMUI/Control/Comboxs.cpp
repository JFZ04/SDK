#include "Comboxs.h"

ComboBoxs::ComboBoxs()
{
    for (size_t i = 0; i < ComboBoxsMax; i++)
    {
        items[i] = (char*)"";
    }
}

ComboBoxs::ComboBoxs(ComboBoxs&& Other)
{
    for (size_t i = 0; i < ComboBoxsMax; i++)
    {
        items[i] = (char*)"";
    }
}

void ComboBoxs::AddItem(const char* str)
{
    for (size_t i = 0; i < ComboBoxsMax; i++)
    {
        if (this->items[i] == u8"" || this->items[i] == "")
        {
            this->items[i] = (char*)str;
            break;
        }
    }
}

int ComboBoxs::Length()
{
    for (size_t i = 0; i < ComboBoxsMax; i++)
    {
        if (this->items[i] == u8"" || this->items[i] == "")
        {
            return i;
        }
    }
    return 0;
}

bool ComboBoxs::SetItem()
{
    for (size_t i = 0; i < ComboBoxsMax; i++)
    {
        if (SelectedItems[i]!= SelectedItemA[i])
        {
            for (size_t i = 0; i < ComboBoxsMax; i++)
            {
                SelectedItemA[i] = SelectedItems[i];
            }
            return true;
        }
    }
    return false;
}

void ComboBoxs::Protract()
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
    if (SetItem())
    {
        int P = 0;
        TXTA = "";
        for (size_t j = 0; j < this->Length(); j++)
        {
            if (SelectedItems[j])
            {
                std::string da = (const char*)items[j];
                if (P == 0)
                {
                    TXTA += da;
                    P++;
                }
                else
                {
                    TXTA += "," + da;
                }
            }
        }
        if (task_Click_Value)
        {
            ABCDE = std::thread(task_Click_Value);
            ABCDE.detach();
        }
    }

}

void ComboBoxs::POR()
{
    std::vector<const char*>::iterator it;
    int i = 0;
    for (it = Item.begin(); it != Item.end(); ++it)
    {
        items[i] = (char*)Item.data()[i];
        i++;
    }
    

    if(Pos.x != 0 || Pos.y != 0)
    {
        ImGui::SetCursorPos(Pos);
    }
    ImGui::PushFont(UI_ID::Fons[this->Fontsize]);
    auto da = ImGui::GetStyle();
    ImGui::GetStyle().FramePadding = Padding;
    ImGui::GetStyle().FrameRounding = Round;
    ImGui::GetStyle().ItemSpacing = {0,10};
    ImGui::PushItemWidth(Width);
    ImGui::PushStyleColor(ICLR::ImGuiCol_FrameBg, BGColor);
    ImGui::PushStyleColor(ICLR::ImGuiCol_FrameBgActive, BGAColor);
    ImGui::PushStyleColor(ICLR::ImGuiCol_FrameBgHovered, BGHColor);
    ImGui::PushStyleColor(ICLR::ImGuiCol_Button, BtnColor);
    ImGui::PushStyleColor(ICLR::ImGuiCol_ButtonActive, BtnAColor);
    ImGui::PushStyleColor(ICLR::ImGuiCol_ButtonHovered, BtnHColor);
    ImGui::PushStyleColor(ICLR::ImGuiCol_Text, TextColor);
    ImGui::PushStyleColor(ICLR::ImGuiCol_Border, BGColor);
    
    

    if (ImGui::BeginCombo(ID.c_str(), TXTA.c_str()))
    {
        ImGui::PushStyleColor(ICLR::ImGuiCol_HeaderActive, BGASelected);
        ImGui::PushStyleColor(ICLR::ImGuiCol_HeaderHovered, BGHSelected);
        ImGui::PushStyleColor(ICLR::ImGuiCol_Header, BGSelected);
        ImGui::PushItemWidth(Width);
        ImGui::PushStyleVar(ImGuiStyleVar_SelectableTextAlign,SelectableTextAlign);
        auto ia = this->Length();

        ImGui::BeginListBox("##MyListbox");
        for (int i = 0; i < ia; i++)
        {
            if (a==i)
            {
                ImGui::GetStyle().Colors[ImGuiCol_Text] = TextHColor;
            }
            else
            {
                ImGui::GetStyle().Colors[ImGuiCol_Text] = TextColor;
            }
            if (SelectedItems[i])
            {
                ImGui::GetStyle().Colors[ImGuiCol_Text] = TextAColor;
            }
            if (ImGui::Selectable(items[i], &SelectedItems[i]))
            {
                SelectedItems[i] != SelectedItems[i];
            }
            if (ImGui::IsItemHovered())
            {
                a = i;
            }
            if (SelectedItems[i])
            {
                ImGui::GetStyle().Colors[ImGuiCol_Text] = TextColor;
            }


        }
        ImGui::EndListBox();

        ImGui::PopStyleVar();
        ImGui::PopStyleColor(3);
        ImGui::PopItemWidth();
        ImGui::EndCombo();
    }


    ImGui::GetStyle() = da;
    ImGui::PopStyleColor(8);
    ImGui::PopItemWidth();
    ImGui::PopFont();
}
