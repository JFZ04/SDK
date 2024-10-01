#include "Drive.h"
#include <string>




Font_t* UI_ID::Fons = new Font_t[Font_MAX];

bool UI_ID::ButtonEx(const char* label, const char* ID, const ImVec2& size_arg, ImGuiButtonFlags flags)
{
    using namespace ImGui;
    ImGuiWindow* window = GetCurrentWindow();
    if (window->SkipItems)
        return false;

    ImGuiContext& g = *GImGui;
    const ImGuiStyle& style = g.Style;
    const ImGuiID id = window->GetID(ID);
    const ImVec2 label_size = CalcTextSize(label, NULL, true);

    ImVec2 pos = window->DC.CursorPos;
    if ((flags & ImGuiButtonFlags_AlignTextBaseLine) && style.FramePadding.y < window->DC.CurrLineTextBaseOffset) // Try to vertically align buttons that are smaller/have no padding so that text baseline matches (bit hacky, since it shouldn't be a flag)
        pos.y += window->DC.CurrLineTextBaseOffset - style.FramePadding.y;
    ImVec2 size = CalcItemSize(size_arg, label_size.x + style.FramePadding.x * 2.0f, label_size.y + style.FramePadding.y * 2.0f);

    const ImRect bb(pos, pos + size);
    ItemSize(size, style.FramePadding.y);
    if (!ItemAdd(bb, id))
        return false;

    bool hovered, held;
    bool pressed = ButtonBehavior(bb, id, &hovered, &held, flags);

    // Render
    const ImU32 col = GetColorU32((held && hovered) ? ImGuiCol_ButtonActive : hovered ? ImGuiCol_ButtonHovered : ImGuiCol_Button);
    RenderNavHighlight(bb, id);
    RenderFrame(bb.Min, bb.Max, col, true, style.FrameRounding);

    if (g.LogEnabled)
        LogSetNextTextDecoration("[", "]");
    RenderTextClipped(bb.Min + style.FramePadding, bb.Max - style.FramePadding, label, NULL, &label_size, style.ButtonTextAlign, &bb);

    // Automatically close popups
    //if (pressed && !(flags & ImGuiButtonFlags_DontClosePopups) && (window->Flags & ImGuiWindowFlags_Popup))
    //    CloseCurrentPopup();

    IMGUI_TEST_ENGINE_ITEM_INFO(id, label, g.LastItemData.StatusFlags);
    return pressed;
}

void UI_ID::Font_Init(int FSizeItem)
{
    auto& io = ImGui::GetIO(); (void)io;
    //UI_ID::Fons[FSizeItem] = io.Fonts->AddFontFromFileTTF("C:\\Windows\\Fonts\\simsun.ttc", FSizeItem,NULL, io.Fonts->GetGlyphRangesChineseFull());
    UI_ID::Fons[FSizeItem] = io.Fonts->AddFontFromMemoryTTF((void*)Font_data, Font_size, FSizeItem, NULL, io.Fonts->GetGlyphRangesChineseFull());
}

std::string UI_ID::UIIDSTR()
{
    std::string UID = "ID:_" + std::to_string(UIIDint++);
    return UID;
}







