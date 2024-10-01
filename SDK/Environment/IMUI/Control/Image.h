#include "../WindowOS/CWidget.h"

class Image:CWidget
{
public:
    Image();
    Image(Image&& Other);
    void Protract();
    bool LoadImage(const char* path);
    bool LoadImage(const unsigned char* Memory, const int size);
    void BindTexture(unsigned char* Texture,int width,int height);
	template <typename Func, typename... Args>
	bool Mouse_Hovered(Func&& fun, Args&&... args);
	template<typename Func, typename ...Args>
	bool Click(ImGuiMouseButton Mouse, Func&& fun, Args && ...args);
	template<typename Func, typename ...Args>
	bool LoadImageClick(Func&& fun, Args && ...args);
    //-------------------------------------
	ImVec2 Size = { 0,0 };
    HSize WH = { 0,0 };
	ImVec2 Pos = { 0,0 };
    GLuint ImageData = 0;
    int Channels = 0;








private:
    void POR();
    unsigned char* data;
	std::string ID = "ID_" + UI_ID::UIIDSTR();
	std::function<void()> task_Click_L;
	std::function<void()> task_Click_Mouse;
	std::function<void()> task_Click_R;
	std::function<void()> task_Click_M;
	std::function<void()> task_Click_img;
	std::thread A;
	std::thread AB;
	std::thread ABC;
	std::thread ABCD;












};


template<typename Func, typename ...Args>
inline bool Image::Mouse_Hovered(Func&& fun, Args && ...args)
{
	task_Click_Mouse = std::bind(std::forward<Func>(fun), std::forward<Args>(args)...);
	return true;
}

template<typename Func, typename ...Args>
inline bool Image::Click(ImGuiMouseButton Mouse, Func&& fun, Args && ...args)
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
inline bool Image::LoadImageClick(Func&& fun, Args && ...args)
{
	task_Click_img = std::bind(std::forward<Func>(fun), std::forward<Args>(args)...);
	return true;
}
