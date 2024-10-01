# CPP_SDKAPI

#### 介绍
无。。。。。。。。。。。。。。。。。。。。。。。。。。。

#### 软件架构
软件架构说明


#### 安装教程

linux:运行 SDK/Install.sh
Windows:无

#### 使用说明

CmakeList.txt :
add_subdirectory(SDK)
target_include_directories(ThreadPoolDemo PUBLIC ${CC_API_INC})
target_link_libraries(ThreadPoolDemo PUBLIC CC_API)

