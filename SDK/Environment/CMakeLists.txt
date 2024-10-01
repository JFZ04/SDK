cmake_minimum_required(VERSION 3.0)

if(CMAKE_HOST_UNIX)
#    find_package(glfw3 REQUIRED)
    add_subdirectory(glfw)
else ()
    add_subdirectory(glfw)
endif ()

option(UIAPI "Use UI provided as qtimgui submodule" ON)
if (UIAPI)
    add_library(GUIAPI
            STATIC
            IMUI/ImGui/imconfig.h
            IMUI/ImGui/imgui.cpp
            IMUI/ImGui/imgui.h
            IMUI/ImGui/imgui_demo.cpp
            IMUI/ImGui/imgui_draw.cpp
            IMUI/ImGui/imgui_internal.h
            IMUI/ImGui/imgui_widgets.cpp
            IMUI/ImGui/imgui_impl_opengl3.cpp
            IMUI/ImGui/imgui_impl_glfw.cpp
            IMUI/ImGui/imgui_impl_win32.cpp
            IMUI/ImGui/imgui_tables.cpp
            IMUI/ImGui/imstb_rectpack.h
            IMUI/ImGui/imstb_textedit.h
            IMUI/ImGui/imstb_truetype.h
            IMUI/WindowOS/CApplication.cpp
            IMUI/WindowOS/CWidget.cpp

            IMUI/Control/Button.cpp
            IMUI/Control/Combox.cpp
            IMUI/Control/Comboxs.cpp
            IMUI/Control/Image.cpp
            IMUI/Control/ImageButton.cpp
            IMUI/Control/Label.cpp
            IMUI/Control/Panel.cpp
            IMUI/Control/SliderBar.cpp
            IMUI/Control/SwitchBool.cpp
            IMUI/Control/TextBox.cpp
            IMUI/Drive/Drive.cpp
    )
    target_include_directories(GUIAPI PUBLIC IMUI/ImGui IMUI/WindowOS IMUI/Control glfw/include)
    if(CMAKE_HOST_UNIX)
        target_link_libraries(GUIAPI GL glfw X11)
    elseif(CMAKE_HOST_WIN32)
        target_link_libraries(GUIAPI opengl32 dwmapi glfw)
    else()


    endif()
endif(UIAPI)

