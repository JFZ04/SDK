#include "Label.h"

Label::Label()
{

}

Label::Label(Label&& other)
{

}

void Label::Protract()
{
	POR();
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

void Label::POR()
{
	if(Pos.x != 0 || Pos.y != 0)
    {
        ImGui::SetCursorPos(Pos);
    }
    static bool dacol = true;
    if(dacol)
    {
        if(TextHColor == ImVec4(RGBA(255,255,255,255)))
        {
            TextHColor = TextColor;
        }
        dacol = false;
    }
	ImGui::PushFont(UI_ID::Fons[this->FontSize]);
    ImGui::PushStyleColor(ICLR::ImGuiCol_Text,ImVec4(Colora));
	ImGui::Text(Text.c_str(),"");
    ImGui::PopStyleColor();
	if (ImGui::IsItemHovered())
	{
		Colora = TextHColor;
		if (task_Click_Mouse)
		{
			A = std::thread(task_Click_Mouse);
			A.detach();
		}
	}
	else
	{
		Colora = TextColor;
	}

	ImGui::PopFont();
}
