cmake_minimum_required(VERSION 2.0)
project(ThreadPoolDemo CXX)

add_subdirectory(SDK)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS}")
set(CMAKE_EXE_LINKER_FLAGS "-static-libgcc -static-libstdc++")

add_executable(ThreadPoolDemo
#        WIN32
        main.cpp
        MainRun.cpp
        MainRun.h
        UI/Demo.cpp
        UI/Demo.h
        UI/ui_Demo.h)

target_include_directories(ThreadPoolDemo PUBLIC ${CC_API_INC})
target_link_libraries(ThreadPoolDemo PUBLIC CC_API)
