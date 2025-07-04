cmake_minimum_required(VERSION 3.30)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
project(Video-Scraper-Server LANGUAGES CXX)
set(SRCS
    Server/src/main.cpp
)
add_executable(Server ${SRCS})
set_property(TARGET Server PROPERTY CXX_STANDARD 20)

target_include_directories(Server PRIVATE
    Server/src
    Network/src
)

target_link_libraries(Server
    Network
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

target_link_libraries(Client
    Network
)

set(SRCS
    Network/src/Network/Core/tmp.cpp
    Network/src/Network/Core/Core.cpp
    Network/src/Network/Core/Core.h
)
add_library(Network STATIC ${SRCS})
set_property(TARGET Network PROPERTY CXX_STANDARD 20)

target_include_directories(Network PRIVATE
    Network/src
)

