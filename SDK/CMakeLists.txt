cmake_minimum_required(VERSION 3.0)

add_subdirectory(Depend/portaudio)
add_subdirectory(Depend/CSerialPort)
add_subdirectory(Depend/mirrors_nlohmann_json)
add_subdirectory(Depend/cppp-reiconv)
add_subdirectory(Environment)

find_path(MYSQL_ROOT_DIR ${PROJECT_SOURCE_DIR}/SDK/Depend/mysql-9.0.1)
include(FindPackageHandleStandardArgs)
find_package_handle_standard_args(mysql REQUIRED_VARS MYSQL_ROOT_DIR)

set(INC_DIR ${PROJECT_SOURCE_DIR}/SDK/CCServlet/include)
set(LINK_DIR ${PROJECT_SOURCE_DIR}/SDK/Depend/Lib/libs)
set(LINK_DLL_DIR ${PROJECT_SOURCE_DIR}/SDK/Depend/Bin/bin)
set(CC_API_INC ${PROJECT_SOURCE_DIR}/SDK/Depend/CSerialPort/include
        ${PROJECT_SOURCE_DIR}/SDK/Depend/mirrors_nlohmann_json/include
        ${PROJECT_SOURCE_DIR}/SDK/Depend/mirrors_nlohmann_json/include/nlohmann
        ${PROJECT_SOURCE_DIR}/SDK/Depend/portaudio/include
        ${PROJECT_SOURCE_DIR}/SDK/Multimedia
        ${PROJECT_SOURCE_DIR}/SDK/CCServlet/SQL
        ${PROJECT_SOURCE_DIR}/SDK/CCServlet/include
        ${PROJECT_SOURCE_DIR}/SDK/CCServlet/SQL include
        ${PROJECT_SOURCE_DIR}/SDK/Environment/IMUI/Control
        ${PROJECT_SOURCE_DIR}/SDK/Environment/IMUI/Drive
        ${PROJECT_SOURCE_DIR}/SDK/Environment/IMUI/ImGui
        ${PROJECT_SOURCE_DIR}/SDK/Environment/IMUI/WindowOS
        ${PROJECT_SOURCE_DIR}/SDK/Environment/glfw/include
        ${INC_DIR}
        ${PROJECT_SOURCE_DIR}/SDK/Depend/mysql-9.0.1/include
)
option(UIAPIS "APIS" ON)
if (UIAPIS)
    add_library(CC_API
            STATIC
            src/CCThread.cpp
            src/CCSocket.cpp
            src/CCString.cpp
            src/CCSerialPort.cpp
            CCServlet/src/CCWeb.cpp
            src/CCAudio.cpp
            src/CCThreadPool.cpp
            include/CCThreadPool.h
            src/CCJSONObject.cpp
            include/CCJSONObject.h
            src/CCFile.cpp
            CCServlet/src/CCWebServlet.cpp
            CCServlet/include/CCWebServlet.h
            CCServlet/src/CCResponse.cpp
            CCServlet/include/CCResponse.h
            CCServlet/src/CCRequest.cpp
            CCServlet/include/CCRequest.h
            CCServlet/include/CORS.h
            CCServlet/src/CCSQLite3.cpp
            CCServlet/include/CCSQLite3.h
            src/CCEpoll.cpp
            src/CCByte.cpp
            include/CCEpoll.h
            include/CCObject.h
            src/CCFileOutStream.cpp
            src/CCEncode.cpp
            include/CCFileOutStream.h
            src/CCByteArray.cpp
            include/CCByteArray.h
            src/CCFIleInStream.cpp
            include/CCFIleInStream.h
            include/CC.h
            src/CCNetwork.cpp
            include/CCNetwork.h
            src/CCTimeData.cpp
            include/CCTimeData.h
            src/CCMySql.cpp
            include/CCMySql.h
    )
    target_include_directories(CC_API PUBLIC
            ${PROJECT_SOURCE_DIR}/SDK/Depend/CSerialPort/include
            ${PROJECT_SOURCE_DIR}/SDK/Depend/mirrors_nlohmann_json/include
            ${PROJECT_SOURCE_DIR}/SDK/Depend/mirrors_nlohmann_json/include/nlohmann
            ${PROJECT_SOURCE_DIR}/SDK/Depend/portaudio/include
            ${PROJECT_SOURCE_DIR}/SDK/Multimedia
            ${PROJECT_SOURCE_DIR}/SDK/CCServlet/SQL
            ${PROJECT_SOURCE_DIR}/SDK/CCServlet/include
            ${PROJECT_SOURCE_DIR}/SDK/CCServlet/SQL include
            ${PROJECT_SOURCE_DIR}/SDK/Environment/IMUI/Control
            ${PROJECT_SOURCE_DIR}/SDK/Environment/IMUI/Drive
            ${PROJECT_SOURCE_DIR}/SDK/Environment/IMUI/ImGui
            ${PROJECT_SOURCE_DIR}/SDK/Environment/IMUI/WindowOS
            ${PROJECT_SOURCE_DIR}/SDK/Environment/glfw/include
            ${INC_DIR}
            ${PROJECT_SOURCE_DIR}/SDK/Depend/mysql-9.0.1/include/

    )
    if(CMAKE_HOST_UNIX)
        target_link_libraries(CC_API PRIVATE libcserialport sqlite3 nlohmann_json PortAudio
                ${CMAKE_CURRENT_SOURCE_DIR}/Depend/Lib/libs/x86_64/libbass.so
                ${LINK_DIR}/libcppp-reiconv.static.a
                GUIAPI crypto
#                ${PROJECT_SOURCE_DIR}/SDK/Depend/mysql-9.0.1/lib/LinuxLib/libmysqlclient.so
                ${PROJECT_SOURCE_DIR}/SDK/Depend/mysql-9.0.1/lib/LinuxLib/lib64/libmysqlcppconn.so
                ${PROJECT_SOURCE_DIR}/SDK/Depend/mysql-9.0.1/lib/LinuxLib/lib64/libmysqlcppconnx.so
        )
    elseif(CMAKE_HOST_WIN32)
        target_link_libraries(CC_API PRIVATE ws2_32 libcserialport
                nlohmann_json
                ${CMAKE_CURRENT_SOURCE_DIR}/Depend/Bin/bin/sqlite3.dll PortAudio
                ${CMAKE_CURRENT_SOURCE_DIR}/Depend/Lib/libs/x86_64/bass.lib
                GUIAPI
                ${LINK_DIR}/VC/X64/MD/openssl.lib
                ${LINK_DIR}/VC/X64/MD/libcrypto.lib
                ${LINK_DIR}/libcppp-reiconv.static-win.a
                ${PROJECT_SOURCE_DIR}/SDK/Depend/mysql-9.0.1/lib/libmysql.lib
        )
    else()


    endif()

endif(UIAPIS)