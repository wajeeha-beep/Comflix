cmake_minimum_required(VERSION 3.10)
project(Comflix3)

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

add_executable(Comflix3
        src/main.cpp
)

target_include_directories(Comflix3 PRIVATE
        src
)
