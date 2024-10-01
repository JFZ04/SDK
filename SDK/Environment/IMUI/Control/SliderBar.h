#define  __SliderBar__H__
#ifdef   __SliderBar__H__
#pragma once
#include "../Drive/Drive.h"

class SlideBar
{
public:
	SlideBar() {};
	SlideBar(const SlideBar&&) {};
	bool Protract();
	template <typename Func, typename... Args>
	bool Mouse_Hovered(Func&& fun, Args&&... args);
	template<typename Func, typename ...Args>
	bool Click(ImGuiMouseButton Mouse, Func&& fun, Args && ...args);
	template<typename Func, typename ...Args>
	void ValueChange(Func&& fun, Args && ...args);

	std::string Text = "";
	ImVec2 Pos = { 0,0 };
	ImVec2 BtnSize = { 3,8 };
	int Fontsize = 18;
	float Value = 0.5f;
	float MiniValue = 0.0f;
	float MaxValue = 10.0f;
	float Width = 150.0f;
	float Height = 5.0f;
	float FrameHeight = 0.0f;
	float BtnRound = 9.0f;
	float Round = 5.0f;
	char* format = (char*)"%.3f";
	ImVec4 Garb = { 0.24f, 0.52f, 0.88f, 2.0f };
	ImVec4 GarbActive = { 0, 0.95f, 0, 0.9f };
	ImVec4 FrameColor = { 0, 0, 0, 150 };
	ImVec4 FrameAColor = FrameColor;
	ImVec4 FrameHColor = FrameColor;
	ImVec4 PaddColor = { 15, 150, 200, 150 };
	ImGuiSliderFlags Attribute = 0;
	bool RenderTextFlag = false;

private:
	bool Protrac();
	ImRect grab_bb;
	ImRect Padd;
	float RValue = Value;
	std::string ID = "ID_" + UI_ID::UIIDSTR();
	std::function<void()> task_Click_L;
	std::function<void()> task_Click_Mouse;
	std::function<void()> task_Click_R;
	std::function<void()> task_Click_M;
	std::function<void()> task_Click_Value;

	std::thread ABC;
	std::thread ABCD;
	std::thread ABCDE;
	std::thread ABCDEF;
	std::thread ABCDEG;

};

template<typename Func, typename ...Args>
inline bool SlideBar::Mouse_Hovered(Func&& fun, Args && ...args)
{
	task_Click_Mouse = std::bind(std::forward<Func>(fun), std::forward<Args>(args)...);
	return true;
}

template<typename Func, typename ...Args>
inline bool SlideBar::Click(ImGuiMouseButton Mouse, Func&& fun, Args && ...args)
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
inline void SlideBar::ValueChange(Func&& fun, Args && ...args)
{
	task_Click_Value = std::bind(std::forward<Func>(fun), std::forward<Args>(args)...);
}

#endif // DEBUG
