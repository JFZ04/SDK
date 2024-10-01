#include "Image.h"


Image::Image()
{

}

Image::Image(Image&& Other)
{

}

bool Image::LoadImage(const char* path)
{
    data = LoadIMGMemory(path, reinterpret_cast<int *>(&WH.width),
                                               reinterpret_cast<int *>(&WH.height), &Channels, 4);
    if (data)
    {
        glGenTextures(1, &ImageData);
        glBindTexture(GL_TEXTURE_2D, ImageData);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, WH.width, WH.height, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);

    }
    else
    {
        return false;
    }
    return true;
}

bool Image::LoadImage(const unsigned char* Memory, const int size)
{
    int x,y;
    data = LoadIMGMemory(Memory, size, &x,&y, &Channels, 4);
    WH.width = x;WH.height = y;
    if (data)
    {
        glGenTextures(1, &ImageData);
        glBindTexture(GL_TEXTURE_2D, ImageData);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, WH.width, WH.height, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);

    }
    else
    {
        return false;
    }
    return true;
}

void Image::Protract()
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


}

void Image::POR()
{
    if(Pos.x != 0 || Pos.y != 0)
    {
        ImGui::SetCursorPos(Pos);
    }
    if (data)
    {
        glGenTextures(1, &ImageData);
        glBindTexture(GL_TEXTURE_2D, ImageData);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, WH.width, WH.height, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
        data = NULL;
    }
    if(task_Click_img)
    {
        task_Click_img();
    }
    ImGui::Image((void*)(intptr_t)ImageData, {Size.x,Size.y},ImVec2(0, 0), ImVec2(1, 1), ImVec4(1, 1, 1, 1), ImVec4(1, 1, 1, 0));
}

void Image::BindTexture(unsigned char *Texture, int width, int height)
{
    glGenTextures(1, &ImageData);
    glBindTexture(GL_TEXTURE_2D, ImageData);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, 0x812F);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, 0x812F);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height,
                 0, GL_RGB, GL_UNSIGNED_BYTE, Texture);
}
