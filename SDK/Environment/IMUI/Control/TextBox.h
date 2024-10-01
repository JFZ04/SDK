#include "../Drive/Drive.h"
#pragma once
class TextBox
{
public:
	TextBox();
	TextBox(TextBox&& Other);
	void Protract();
	template <typename Func, typename... Args>
	bool Mouse_Hovered(Func&& fun, Args&&... args);
	template<typename Func, typename ...Args>
	bool Click(ImGuiMouseButton Mouse, Func&& fun, Args && ...args);
	template<typename Func, typename ...Args>
	void ValueChange(Func&& fun, Args && ...args);
	//-----------------------------------------------
	ImVec4 TextColor = { 15, 150, 200, 150 };
	ImVec4 BGColor = { 0, 0, 0, 150 };
	ImVec4 BGAColor = { 0, 0, 0, 150 };
	ImVec4 BGHColor = { 0, 0, 0, 150 };
	ImVec2 Pos = { 0,0 };
	int Fontsize = 18;
	float Width = 100;
	int TextSize = 50;
	float Round = 2;
	std::string Text = "";
	ImGuiInputTextFlags Attribute = 0;

private:
	void POR();
	char* Txt = new char[TextSize];
	std::string RValue = Text;
	std::string ID = "##" + UI_ID::UIIDSTR();
	std::function<void()> task_Click_L;
	std::function<void()> task_Click_Mouse;
	std::function<void()> task_Click_R;
	std::function<void()> task_Click_M;
	std::function<void()> task_Click_Value;
	std::thread A;
	std::thread AB;
	std::thread ABC;
	std::thread ABCD;
	std::thread ABCDE;


};


template<typename Func, typename ...Args>
inline bool TextBox::Mouse_Hovered(Func&& fun, Args && ...args)
{
	task_Click_Mouse = std::bind(std::forward<Func>(fun), std::forward<Args>(args)...);
	return true;
}

template<typename Func, typename ...Args>
inline bool TextBox::Click(ImGuiMouseButton Mouse, Func&& fun, Args && ...args)
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

template<typename Func, typename ...Args>
inline void TextBox::ValueChange(Func&& fun, Args && ...args)
{
	task_Click_Value = std::bind(std::forward<Func>(fun), std::forward<Args>(args)...);
}