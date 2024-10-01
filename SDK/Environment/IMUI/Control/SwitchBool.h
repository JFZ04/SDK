#define  __Switch_H_
#ifdef   __Switch_H_
#pragma once

#include "../Drive/Drive.h"

struct SlideButton_Float {
	float animation;
};

class SwitchBool
{
public:
	SwitchBool();
	SwitchBool(SwitchBool&&);
	void Protract();
	template <typename Func, typename... Args>
	void Click_LDown(Func&& fun, Args&&... args);
	template <typename Func, typename... Args>
	void Click_RDown(Func&& fun, Args&&... args);
	template <typename Func, typename... Args>
	void Mouse_Hovered(Func&& fun, Args&&... args);
	ImVec2 Size;
	ImVec2 Pos;
	ImVec4 BtnColor = ImVec4(0.06f, 0.53f, 0.98f, 1.00f);
	ImVec4 BtnHovered = ImVec4(0.26f, 0.59f, 0.98f, 1.00f);
	ImVec4 BtnActive = ImVec4(0.06f, 0.53f, 0.98f, 1.00f);
	std::string Text;
	int Round = 0;
	bool Selected;
	bool TextFlag = false;
	bool Sameline = false;
	std::map<int, bool> ReadKeyBtn;
	std::map<int, bool> ReadKeyTpye;


private:
	float animation = 0;
	std::map<int,std::string> ReadKeys;
	void RunKey();
	bool Selecteds;
	std::function<void()> task_Click_R; // ���ڴ洢�����Ͳ���
	std::function<void()> task_Click; // ���ڴ洢�����Ͳ���
	std::function<void()> task_Click_Mouse; // ���ڴ洢�����Ͳ���
	std::function<void()> Task_ABCDEF; // ���ڴ洢�����Ͳ���
	template <typename Func, typename... Args>
	void RunKey_Click(Func&& fun, Args&&... args);
	std::thread ABC;
	std::thread ABCD;
	std::thread ABCDE;
	std::thread ABCDEF;
	std::string ID = "ID_" + UI_ID::UIIDSTR();

};


template <typename Func, typename... Args>
inline void SwitchBool::Click_LDown(Func&& fun, Args&&... args)
{
	task_Click = std::bind(std::forward<Func>(fun), std::forward<Args>(args)...);
}

template<typename Func, typename ...Args>
inline void SwitchBool::Click_RDown(Func&& fun, Args && ...args)
{
	task_Click_R = std::bind(std::forward<Func>(fun), std::forward<Args>(args)...);
}

template<typename Func, typename ...Args>
inline void SwitchBool::Mouse_Hovered(Func&& fun, Args && ...args)
{
	task_Click_Mouse = std::bind(std::forward<Func>(fun), std::forward<Args>(args)...);
}

template <typename Func, typename... Args>
inline void SwitchBool::RunKey_Click(Func&& fun, Args&&... args)
{
	Task_ABCDEF = std::bind(std::forward<Func>(fun), std::forward<Args>(args)...);
}



#endif 
