#include "SwitchBool.h"

SwitchBool::SwitchBool()
{
	/*this->ReadKeyBtn.insert(std::pair<int, bool>(0,true));
	this->ReadKeys.insert(std::pair<int, std::string>(0, "M1"));
	this->ReadKeyTpye.insert(std::pair<int, bool>(0, true));
	this->ReadKeyBtn.insert(std::pair<int, bool>(1,true));
	this->ReadKeys.insert(std::pair<int, std::string>(1, "A"));
	this->ReadKeyTpye.insert(std::pair<int, bool>(1, false));


	this->Task_ABCDEF = std::bind(&SwitchBool::RunKey, this);
	this->ABCDEF = std::thread(this->Task_ABCDEF);
	this->ABCDEF.detach();*/


}

SwitchBool::SwitchBool(SwitchBool&&)
{

}

void SwitchBool::Protract()
{
	ImGuiStyle* styles = &ImGui::GetStyle();
	ImVec4* colors = styles->Colors;

	colors[ImGuiCol_Button] = this->BtnColor;
	colors[ImGuiCol_ButtonHovered] = this->BtnHovered;
	colors[ImGuiCol_ButtonActive] = this->BtnActive;

	if(Pos.x != 0 || Pos.y != 0)
    {
        ImGui::SetCursorPos(Pos);
    }
	if (this->Sameline)
	{
		ImGui::SameLine();
	}
	using namespace ImGui;
	ImGuiWindow* window = ImGui::GetCurrentWindow();
	if (window->SkipItems)
		return;
	ImGuiContext& g = *GImGui;
	const ImGuiStyle& style = ImGui::GetStyle();
	ImGuiID id = window->GetID(ID.c_str());
	ImVec2 label_size = ImGui::CalcTextSize(Text.c_str(), NULL, true);
	const float square_sz = ImGui::GetFrameHeight();
	const ImVec2 pos = window->DC.CursorPos;
	const ImVec2 la = ImVec2(square_sz + (label_size.x > 0.0f ? style.ItemInnerSpacing.x + label_size.x : 0.0f), label_size.y + style.FramePadding.y * 2.0f);
	//const ImRect total_bb(pos, pos + la);
	const ImRect total_bb(pos, pos + Size);
	

	if (!this->Selected)
	{
		window->DrawList->AddRectFilled(window->DC.CursorPos, window->DC.CursorPos + ImVec2(Size.x, Size.y)
			, ImColor(0, 0, 0, 150), Round);
	}
	else
	{
		window->DrawList->AddRectFilled(window->DC.CursorPos, window->DC.CursorPos + ImVec2(Size.x, Size.y)
			, ImColor(15, 150, 200, 150), Round);
	}

	window->DrawList->AddRectFilled(window->DC.CursorPos + ImVec2(2 + animation, 2), window->DC.CursorPos + ImVec2(Size.y - 1 + animation, Size.y - 2)
		, ImColor(g.Style.Colors[ImGuiCol_Button]), Round);



	if (this->Selected && animation < Size.y - 1)
	{
		animation++;
	}
	if (!this->Selected && animation > 0)
	{
		animation--;
	}

	bool hovered, held;
	bool pressed = ImGui::ButtonBehavior(total_bb, id, &hovered, &held);
	if (pressed)
	{
		this->Selected = !(this->Selected);
		this->Selecteds = this->Selected;
		if (task_Click)
		{
			this->ABC = std::thread(task_Click);
			this->ABC.detach();
		}
		ImGui::MarkItemEdited(id);
	}
	const ImRect check_bb(window->DC.CursorPos + ImVec2(2 + animation, 2), window->DC.CursorPos + ImVec2((Size.y - 1) + animation, Size.y - 2));
	ImGui::RenderNavHighlight(total_bb, id);
	ImGui::RenderFrame(check_bb.Min, check_bb.Max, ImGui::GetColorU32((held && hovered) ? ImGuiCol_ButtonActive : hovered ? ImGuiCol_ButtonHovered : ImGuiCol_Button), true,
		Round);
	ImU32 check_col = ImGui::GetColorU32(ImGuiCol_CheckMark);

	ImGui::ItemSize(total_bb);
	if (!ImGui::ItemAdd(total_bb, id))
	{
		return;
	}
	colors[ImGuiCol_Button] = ImVec4(0.26f, 0.59f, 0.98f, 0.40f);
	colors[ImGuiCol_ButtonHovered] = ImVec4(0.26f, 0.59f, 0.98f, 1.00f);
	colors[ImGuiCol_ButtonActive] = ImVec4(0.06f, 0.53f, 0.98f, 1.00f);

	if (ImGui::IsItemHovered())
	{
		if (task_Click_Mouse)
		{
			this->ABCDE = std::thread(task_Click_Mouse);
			this->ABCDE.detach();
		}
	}
	if (ImGui::IsItemClicked(IMBN::ImGuiMouseButton_Right))
	{
		if (task_Click_R)
		{
			this->ABCD = std::thread(task_Click_R);
			this->ABCD.detach();
		}
	}
	

}

void SwitchBool::RunKey()
{
	while (true)
	{
		

	}

	
}




