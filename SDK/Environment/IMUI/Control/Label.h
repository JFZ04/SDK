#include "../Drive/Drive.h"
#pragma once

class Label
{
public:
	Label();
	Label(Label&& other);
	void Protract();
	template <typename Func, typename... Args>
	bool Mouse_Hovered(Func&& fun, Args&&... args);
	template<typename Func, typename ...Args>
	bool Click(ImGuiMouseButton Mouse, Func&& fun, Args && ...args);
	//--------------------------------------
	ImVec2 Pos = { 0,0 };
	ImColor TextColor = { 255,255,255,255 };
	ImColor TextHColor = { 255,255,255,255 };
	std::string Text = "";
	int FontSize = 18;


private:
	void POR();
	ImColor Colora;
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
inline bool Label::Mouse_Hovered(Func&& fun, Args && ...args)
{
	task_Click_Mouse = std::bind(std::forward<Func>(fun), std::forward<Args>(args)...);
	return true;
}

template<typename Func, typename ...Args>
inline bool Label::Click(ImGuiMouseButton Mouse, Func&& fun, Args && ...args)
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