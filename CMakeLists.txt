cmake_minimum_required(VERSION 3.10)
project(simple_http_server)

# Configuração do C++ standard
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Configurações específicas para Windows
if(WIN32)
    add_definitions(-D_WIN32_WINNT=0x0601)
    set(CMAKE_WINDOWS_EXPORT_ALL_SYMBOLS ON)
endif()

# Arquivo fonte
add_executable(server 
    server/main.cpp
    server/server.cpp
    server/server.h
)

# Linkagem de bibliotecas específicas por plataforma
if(UNIX)
    target_link_libraries(server pthread)
endif()

if(WIN32)
    target_link_libraries(server ws2_32)
endif()
