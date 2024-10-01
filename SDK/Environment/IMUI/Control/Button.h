#include "../Drive/Drive.h"
#pragma once
class Button
{
public:
	Button();
	Button(Button&& Other);
	void Protract();
	template <typename Func, typename... Args>
	bool Mouse_Hovered(Func&& fun, Args&&... args);
	template<typename Func, typename ...Args>
	bool Click(ImGuiMouseButton Mouse, Func&& fun, Args && ...args);
	//-------------------------------------
	ImVec2 Size = { 100,30 };
	ImVec2 Pos = { 0,0 };
	int Fontsize = 18;
	int Round = 9;
	std::string Text = "";
	ImGuiButtonFlags Attribute = 0;
	ImVec4 BtnColor = ImVec4(0.26f, 0.59f, 0.98f, 0.40f);
	ImVec4 BtnAColor = ImVec4(0.26f, 0.59f, 0.98f, 1.00f);
	ImVec4 BtnHColor = ImVec4(0.06f, 0.53f, 0.98f, 1.00f);
	ImVec4 TextColor = ImVec4(1.00f, 1.00f, 1.00f, 1.00f);


private:
	void POR();
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

template<typename Func, typename ...Args>
inline bool Button::Mouse_Hovered(Func&& fun, Args && ...args)
{
	task_Click_Mouse = std::bind(std::forward<Func>(fun), std::forward<Args>(args)...);
	return true;
}

template<typename Func, typename ...Args>
inline bool Button::Click(ImGuiMouseButton Mouse, Func&& fun, Args && ...args)
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