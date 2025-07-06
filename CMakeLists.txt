cmake_minimum_required(VERSION 3.25)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
project(Video-Scraper-Server LANGUAGES CXX)
set(SRCS
    Server/src/Server.cpp
    Server/src/main.cpp
    Server/src/Server.h
)
add_executable(Server ${SRCS})
set_property(TARGET Server PROPERTY CXX_STANDARD 20)

target_include_directories(Server PRIVATE
    Server/src
    Network/src
)

set(SRCS
    Client/src/main.cpp
)
add_executable(Client ${SRCS})
set_property(TARGET Client PROPERTY CXX_STANDARD 20)

target_include_directories(Client PRIVATE
    Client/src
    Network/src
)

