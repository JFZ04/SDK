#include "ImageButton.h"

void ImageButton::Protract()
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

bool ImageButton::LoadImage(const char *path)
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

bool ImageButton::LoadImage(const unsigned char *Memory, const int size)
{
    data = LoadIMGMemory(Memory, size,reinterpret_cast<int *>(&WH.width),
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

void ImageButton::POR()
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
    ImGui::PushStyleColor(ICLR::ImGuiCol_Button,BtnColor);
    ImGui::PushStyleColor(ICLR::ImGuiCol_ButtonActive,BtnAColor);
    ImGui::PushStyleColor(ICLR::ImGuiCol_ButtonHovered,BtnHColor);
    ImGui::PushStyleColor(ICLR::ImGuiCol_Text,TextColor);
    auto S1 = ImGui::GetStyle().FrameRounding;
    ImGui::GetStyle().FrameRounding = Round;
    ImGui::ImageButtonEx(ImGui::GetID(ID.c_str()),(void*)(intptr_t)ImageData,Size,uv0,uv1,bg_col,tint_col);
    ImGui::GetStyle().FrameRounding = S1;
    ImGui::PopStyleColor(4);
}

void ImageButton::BindTexture(unsigned char *Texture, int width, int height)
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
