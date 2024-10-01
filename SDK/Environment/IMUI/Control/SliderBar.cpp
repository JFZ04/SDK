#include "SliderBar.h"

bool SlideBar::Protract()
{
    Protrac();
    if (RValue != Value)
    {
        RValue = Value;
        if (task_Click_Value)
        {
            ABC = std::thread(task_Click_Value);
            ABC.detach();
        }
       
    }
    if (ImGui::IsItemHovered())
    {
        if (task_Click_Mouse)
        {
            ABCD = std::thread(task_Click_Mouse);
            ABCD.detach();
        }
    }
    if (ImGui::IsMouseClicked(IMBN::ImGuiMouseButton_Left) && ImGui::IsItemHovered())
    {
        if (task_Click_L)
        {
            ABCDE = std::thread(task_Click_L);
            ABCDE.detach();
        }
        
    }
    if (ImGui::IsMouseClicked(IMBN::ImGuiMouseButton_Right) && ImGui::IsItemHovered())
    {
        if (task_Click_R)
        {
            ABCDEF = std::thread(task_Click_R);
            ABCDEF.detach();
        }
    }
    if (ImGui::IsMouseClicked(IMBN::ImGuiMouseButton_Middle) && ImGui::IsItemHovered())
    {
        if (task_Click_M)
        {
            ABCDEG = std::thread(task_Click_M);
            ABCDEG.detach();
        }
    }
    return true;
}

bool SlideBar::Protrac()
{
    if(Pos.x != 0 || Pos.y != 0)
    {
        ImGui::SetCursorPos(Pos);
    }
    using namespace ImGui;

    ImGuiWindow* window = GetCurrentWindow();
    if (window->SkipItems)
        return false;

    ImGuiContext& g = *GImGui;
    ImGuiStyle& style = g.Style;
    const ImGuiID id = window->GetID(ID.c_str());

    const ImVec2 label_size = CalcTextSize(Text.c_str(), NULL, true);
    const ImRect frame_bb(window->DC.CursorPos, window->DC.CursorPos + ImVec2(Width, Height));
    const ImRect total_bb(frame_bb.Min, frame_bb.Max + ImVec2(label_size.x > 0.0f ? style.ItemInnerSpacing.x + label_size.x : 0.0f, 0.0f));

    const bool temp_input_allowed = (Attribute & ImGuiSliderFlags_NoInput) == 0;

    ItemSize(total_bb, FrameHeight);
    if (!ItemAdd(total_bb, id, &frame_bb, temp_input_allowed ? ImGuiItemFlags_Inputable : 0))
        return false;

    auto c1 = style.Colors[ImGuiCol_::ImGuiCol_SliderGrab];
    auto c2 = style.Colors[ImGuiCol_::ImGuiCol_SliderGrabActive];
    auto c3 = style.Colors[ImGuiCol_::ImGuiCol_FrameBg];
    auto c4 = style.Colors[ImGuiCol_::ImGuiCol_FrameBgActive];
    auto c5 = style.Colors[ImGuiCol_::ImGuiCol_FrameBgHovered];
    

    style.Colors[ImGuiCol_::ImGuiCol_SliderGrab] = Garb;
    style.Colors[ImGuiCol_::ImGuiCol_SliderGrabActive] = GarbActive;
    style.Colors[ImGuiCol_::ImGuiCol_FrameBgHovered] = FrameHColor;
    style.Colors[ImGuiCol_::ImGuiCol_FrameBgActive] = FrameAColor;
    style.Colors[ImGuiCol_::ImGuiCol_FrameBg] = FrameColor;
    g.Style.FrameRounding = Round;


    // Default format string when passing NULL
    if (format == NULL)
        format = (char*)DataTypeGetInfo(ImGuiDataType_Float)->PrintFmt;

    
    
    const bool hovered = ItemHoverable(frame_bb, id,0);
    const bool hoveredbtn = ItemHoverable(grab_bb,id,0);
    bool temp_input_is_active = temp_input_allowed && TempInputIsActive(id);
    if (!temp_input_is_active)
    {
        // Tabbing or CTRL-clicking on Slider turns it into an input box
        const bool clicked = hoveredbtn || hovered && IsMouseClicked(0, id);
        const bool make_active = (clicked || g.NavActivateId == id);
        if (make_active && clicked)
            SetKeyOwner(ImGuiKey_MouseLeft, id);
        if (make_active && temp_input_allowed)
            if ((clicked && g.IO.KeyCtrl) || (g.NavActivateId == id && (g.NavActivateFlags & ImGuiActivateFlags_PreferInput)))
                temp_input_is_active = true;

        if (make_active && !temp_input_is_active)
        {
            SetActiveID(id, window);
            SetFocusID(id, window);
            FocusWindow(window);
            g.ActiveIdUsingNavDirMask |= (1 << ImGuiDir_Left) | (1 << ImGuiDir_Right);
        }
    }

    if (temp_input_is_active)
    {
        // Only clamp CTRL+Click input when ImGuiSliderFlags_AlwaysClamp is set
        const bool is_clamp_input = (Attribute & ImGuiSliderFlags_AlwaysClamp) != 0;
        return TempInputScalar(frame_bb, id, Text.c_str(), ImGuiDataType_Float, &Value, format, is_clamp_input ? &MiniValue : NULL, is_clamp_input ? &MaxValue : NULL);
    }

    // Draw frame
    const ImU32 frame_col = GetColorU32(g.ActiveId == id ? ImGuiCol_FrameBgActive : hovered ? ImGuiCol_FrameBgHovered : ImGuiCol_FrameBg);
    RenderNavHighlight(frame_bb, id);
    RenderFrame(frame_bb.Min, frame_bb.Max, frame_col, true, g.Style.FrameRounding);

    // Slider behavior
    
    const bool value_changed = SliderBehavior(frame_bb, id, ImGuiDataType_Float, &Value, &MiniValue, &MaxValue, format, Attribute, &grab_bb);
    if (value_changed)
        MarkItemEdited(id);

    
    Padd.Min = frame_bb.Min;
    Padd.Max.x = grab_bb.Max.x;
    Padd.Max.y = grab_bb.Max.y + (frame_bb.Max.y - grab_bb.Max.y);

    window->DrawList->AddRectFilled(frame_bb.Min, Padd.Max, IM_COL32(PaddColor.x, PaddColor.y, PaddColor.z, PaddColor.w), g.Style.FrameRounding);

    grab_bb.Min.x -= BtnSize.x;
    grab_bb.Min.y -= BtnSize.y;
    grab_bb.Max.x += BtnSize.x;
    grab_bb.Max.y += BtnSize.y;

    // Render grab
    if (grab_bb.Max.x > grab_bb.Min.x)
        window->DrawList->AddRectFilled(grab_bb.Min, grab_bb.Max, GetColorU32(g.ActiveId == id ? ImGuiCol_SliderGrabActive : ImGuiCol_SliderGrab), BtnRound);

    

    // Display value using user-provided display format so user can add prefix/suffix/decorations to the value.
    char value_buf[64];
    if (RenderTextFlag)
    {
        const char* value_buf_end = value_buf + DataTypeFormatString(value_buf, IM_ARRAYSIZE(value_buf), ImGuiDataType_Float, &Value, format);
        if (g.LogEnabled)
            LogSetNextTextDecoration("{", "}");
        RenderTextClipped(frame_bb.Min, frame_bb.Max, value_buf, value_buf_end, NULL, ImVec2(0.5f, 0.5f));
    }

    if (label_size.x > 0.0f)
        RenderText(ImVec2(frame_bb.Max.x + style.ItemInnerSpacing.x, frame_bb.Min.y + style.FramePadding.y), Text.c_str());

    IMGUI_TEST_ENGINE_ITEM_INFO(id, Text.c_str(), g.LastItemData.StatusFlags);

    style.Colors[ImGuiCol_::ImGuiCol_SliderGrab] = c1;
    style.Colors[ImGuiCol_::ImGuiCol_SliderGrabActive] = c2;
    style.Colors[ImGuiCol_::ImGuiCol_FrameBg] = c3;
    style.Colors[ImGuiCol_::ImGuiCol_FrameBgActive] = c4;
    style.Colors[ImGuiCol_::ImGuiCol_FrameBgHovered] = c5;
    g.Style = style;
    return value_changed;
}