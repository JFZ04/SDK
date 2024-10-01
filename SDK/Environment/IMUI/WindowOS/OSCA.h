#ifndef IMGUI_OSCA_H
#define IMGUI_OSCA_H

#define GL_SILENCE_DEPRECATION
#if defined(IMGUI_IMPL_OPENGL_ES2)
#include <GLES2/gl2.h>
#endif

#include "imgui.h"

#if defined(_MSC_VER) && (_MSC_VER >= 1900) && !defined(IMGUI_DISABLE_WIN32_FUNCTIONS)
#pragma comment(lib, "legacy_stdio_definitions")
#endif

// This example can also compile and run with Emscripten! See 'Makefile.emscripten' for details.
#ifdef __EMSCRIPTEN__
#include "../libs/emscripten/emscripten_mainloop_stub.h"
#endif

#ifdef _WIN32

#define GLFW_EXPOSE_NATIVE_WIN32
#include "imgui_impl_opengl3.h"
#include "imgui_impl_glfw.h"
#include "GLFW/glfw3.h"
#include <GLFW/glfw3native.h>
#include "imgui_impl_glfw.h"
#ifndef WIN32_LEAN_AND_MEAN
#define WIN32_LEAN_AND_MEAN
#endif
#include <windows.h>
#include <GL/GL.h>
#include <tchar.h>
#include <dwmapi.h>
#endif

#ifdef __linux__
#define GLFW_EXPOSE_NATIVE_X11
#include <GLFW/glfw3.h>
#include <GLFW/glfw3native.h>
#include "imgui_impl_glfw.h"
#include <X11/Xlib.h>
#include <unistd.h>
#include <X11/Xutil.h>
#endif

#include "../Drive/Drive.h"
#include "iostream"
#include <queue>
#include "mutex"

struct HSize
{

    float                                   width, height;
    constexpr HSize()                      : width(0.0f), height(0.0f) { }
    constexpr HSize(float _x, float _y)    : width(_x), height(_y) { }
    float& operator[] (size_t idx)
    { IM_ASSERT(idx == 0 || idx == 1); return ((float*)(void*)(char*)this)[idx]; }
    float  operator[] (size_t idx) const
    { IM_ASSERT(idx == 0 || idx == 1); return ((const float*)(const void*)(const char*)this)[idx]; }
    HSize(ImVec2 wh)
    {
        width = wh.x;
        height = wh.y;
    };

};

struct HPos
{


    float                                   X, Y;
    constexpr HPos()                      : X(0.0f), Y(0.0f) { }
    constexpr HPos(float _x, float _y)    : X(_x), Y(_y) { }
    float& operator[] (size_t idx)
    { IM_ASSERT(idx == 0 || idx == 1); return ((float*)(void*)(char*)this)[idx]; }
    float  operator[] (size_t idx) const
    { IM_ASSERT(idx == 0 || idx == 1); return ((const float*)(const void*)(const char*)this)[idx]; }
    HPos(ImVec2 wh)
    {
        X = wh.x;
        Y = wh.y;
    };
};

static struct Keyboard_
{
    std::string Tab = "Tab";
    std::string LeftArrow = "LeftArrow";
    std::string RightArrow =  "RightArrow";
    std::string UpArrow = "UpArrow";
    std::string DownArrow = "DownArrow";
    std::string PageUp = "PageUp";
    std::string PageDown = "PageDown";
    std::string Home = "Home";
    std::string End = "End";
    std::string Insert = "Insert";
    std::string Delete = "Delete";
    std::string Backspace = "Backspace";
    std::string Space = "Space";
    std::string Enter = "Enter";
    std::string Escape = "Escape";
    std::string LeftCtrl = "LeftCtrl";
    std::string LeftShift = "LeftShift";
    std::string LeftAlt = "LeftAlt";
    std::string LeftSuper = "LeftSuper";
    std::string RightCtrl = "RightCtrl";
    std::string RightShift = "RightShift";
    std::string RightAlt = "RightAlt";
    std::string RightSuper = "RightSuper";
    std::string Menu = "Menu";
    std::string Num0 = "0";
    std::string Num1 = "1";
    std::string Num2 = "2";
    std::string Num3 = "3";
    std::string Num4 = "4";
    std::string Num5 = "5";
    std::string Num6 = "6";
    std::string Num7 = "7";
    std::string Num8 = "8";
    std::string Num9 = "9";
    std::string Key_A = "A";
    std::string Key_B = "B";
    std::string Key_C = "C";
    std::string Key_D = "D";
    std::string Key_E = "E";
    std::string Key_F = "F";
    std::string Key_G = "G";
    std::string Key_H = "H";
    std::string Key_I = "I";
    std::string Key_J = "J";
    std::string Key_K = "K";
    std::string Key_L = "L";
    std::string Key_M = "M";
    std::string Key_N = "N";
    std::string Key_O = "O";
    std::string Key_P = "P";
    std::string Key_Q = "Q";
    std::string Key_R = "R";
    std::string Key_S = "S";
    std::string Key_T = "T";
    std::string Key_U = "U";
    std::string Key_V = "V";
    std::string Key_W = "W";
    std::string Key_X = "X";
    std::string Key_Y = "Y";
    std::string Key_Z = "Z";
    std::string Key_F1 = "F1";
    std::string Key_F2 = "F2";
    std::string Key_F3 = "F3";
    std::string Key_F4 = "F4";
    std::string Key_F5 = "F5";
    std::string Key_F6 = "F6";
    std::string Key_F7 = "F7";
    std::string Key_F8 = "F8";
    std::string Key_F9 = "F9";
    std::string Key_F10 = "F10";
    std::string Key_F11 = "F11";
    std::string Key_F12 = "F12";
    std::string Key_F13 = "F13";
    std::string Key_F14 = "F14";
    std::string Key_F15 = "F15";
    std::string Key_F16 = "F16";
    std::string Key_F17 = "F17";
    std::string Key_F18 = "F18";
    std::string Key_F19 = "F19";
    std::string Key_F20 = "F20";
    std::string Key_F21 = "F21";
    std::string Key_F22 = "F22";
    std::string Key_F23 = "F23";
    std::string Key_F24 = "F24";
    std::string Apostrophe = "Apostrophe";
    std::string Comma = "Comma";
    std::string Minus = "Minus";
    std::string Period = "Period";
    std::string Slash = "Slash";
    std::string Semicolon = "Semicolon";
    std::string Equal = "Equal";
    std::string LeftBracket = "LeftBracket";
    std::string Backslash = "Backslash";
    std::string RightBracket = "RightBracket";
    std::string GraveAccent = "GraveAccent";
    std::string CapsLock = "CapsLock";
    std::string ScrollLock = "ScrollLock";
    std::string NumLock = "NumLock";
    std::string PrintScreen = "PrintScreen";
    std::string Pause = "Pause";
    std::string Key_0 = "Keypad0";
    std::string Key_1 = "Keypad1";
    std::string Key_2 = "Keypad2";
    std::string Key_3 = "Keypad3";
    std::string Key_4 = "Keypad4";
    std::string Key_5 = "Keypad5";
    std::string Key_6 = "Keypad6";
    std::string Key_7 = "Keypad7";
    std::string Key_8 = "Keypad8";
    std::string Key_9 = "Keypad9";
    std::string KeypadDecimal = "KeypadDecimal";
    std::string KeypadDivide = "KeypadDivide";
    std::string KeypadMultiply = "KeypadMultiply";
    std::string KeypadSubtract = "KeypadSubtract";
    std::string KeypadAdd = "KeypadAdd";
    std::string KeypadEnter = "KeypadEnter";
    std::string KeypadEqual = "KeypadEqual";
    std::string AppBack = "AppBack";
    std::string AppForward = "AppForward";
    std::string GamepadStart = "GamepadStart";
    std::string GamepadBack = "GamepadBack";
    std::string GamepadFaceLeft = "GamepadFaceLeft";
    std::string GamepadFaceRight = "GamepadFaceRight";
    std::string GamepadFaceUp = "GamepadFaceUp";
    std::string GamepadFaceDown = "GamepadFaceDown";
    std::string GamepadDpadLeft = "GamepadDpadLeft";
    std::string GamepadDpadRight = "GamepadDpadRight";
    std::string GamepadDpadUp = "GamepadDpadUp";
    std::string GamepadDpadDown = "GamepadDpadDown";
    std::string GamepadL1 = "GamepadL1";
    std::string GamepadR1 = "GamepadR1";
    std::string GamepadL2 = "GamepadL2";
    std::string GamepadR2 = "GamepadR2";
    std::string GamepadLStickLeft = "GamepadLStickLeft";
    std::string GamepadLStickRight = "GamepadLStickRight";
    std::string GamepadLStickUp = "GamepadLStickUp";
    std::string GamepadLStickDown = "GamepadLStickDown";
    std::string GamepadRStickLeft = "GamepadRStickLeft";
    std::string GamepadRStickRight = "GamepadRStickRight";
    std::string GamepadRStickUp = "GamepadRStickUp";
    std::string GamepadRStickDown = "GamepadRStickDown";
    std::string MouseLeft = "MouseLeft";
    std::string MouseRight = "MouseRight";
    std::string MouseMiddle = "MouseMiddle";
    std::string MouseX1 = "MouseX1";
    std::string MouseX2 = "MouseX2";
    std::string MouseWheelX = "MouseWheelX";
    std::string MouseWheelY = "MouseWheelY";
    std::string ModCtrl = "ModCtrl";
    std::string ModShift = "ModShift";
    std::string ModAlt = "ModAlt";
    std::string ModSuper = "ModSuper";
} Keyboard;

enum KeyMode
{
    KeyDown,
    KeyPressed,
    KeyReleased
};

#endif
