#include "../Drive/Drive.h"
#pragma once
#define ComboBoxsMax 20
class ComboBoxs
{
public:
	ComboBoxs();
	ComboBoxs(ComboBoxs&& Other);
	void AddItem(const char* str);
	int Length();
	void Protract();
	template <typename Func, typename... Args>
	bool Mouse_Hovered(Func&& fun, Args&&... args);
	template<typename Func, typename ...Args>
	bool Click(ImGuiMouseButton Mouse, Func&& fun, Args && ...args);
	template<typename Func, typename ...Args>
	void ValueChange(Func&& fun, Args && ...args);
	//-----------------------------------------------
	ImVec2 Pos = { 0,0 };
	float Width = 100;
	int Index = 0;
	int Fontsize = 18;
	std::vector<const char*> Item;
	bool SelectedItems[ComboBoxsMax] = { false }; // ѡ��״̬������
	float Round = 2;
	std::string TXTA = "";
	ImVec2 Padding = { 0,3 };
	ImVec2 SelectableTextAlign = { 0.5,0.5 };
	ImVec4 BGColor = { 0, 0, 0, 150 };
	ImVec4 BGAColor = { 0, 0, 0, 150 };
	ImVec4 BGHColor = { 0, 0, 0, 150 };
	ImVec4 BtnColor = { 0, 0, 0, 150 };
	ImVec4 BtnAColor = { 0, 0, 0, 150 };
	ImVec4 BtnHColor = { 0, 0, 0, 150 };
	ImVec4 TextColor = { 1,1,1,1 };
	ImVec4 BGSelected = { 0.5,0.5,0.5,0.0 };
	ImVec4 BGASelected = { 0.5,0.5,0.5,0.0 };
	ImVec4 BGHSelected = { 0.5,0.5,0.5,0.0 };
	ImVec4 TextHColor = { 1,0,1,1 };
	ImVec4 TextAColor = { 1,0,0,1 };





private:
	void POR();
	bool SetItem();
	std::string ID = "##" + UI_ID::UIIDSTR();
	int a = -1;
	bool num1 = true;
	char* items[ComboBoxsMax];
	bool SelectedItemA[ComboBoxsMax] = { false };
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
inline bool ComboBoxs::Mouse_Hovered(Func&& fun, Args && ...args)
{
	task_Click_Mouse = std::bind(std::forward<Func>(fun), std::forward<Args>(args)...);
	return true;
}

template<typename Func, typename ...Args>
inline bool ComboBoxs::Click(ImGuiMouseButton Mouse, Func&& fun, Args && ...args)
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
inline void ComboBoxs::ValueChange(Func&& fun, Args && ...args)
{
	task_Click_Value = std::bind(std::forward<Func>(fun), std::forward<Args>(args)...);
}