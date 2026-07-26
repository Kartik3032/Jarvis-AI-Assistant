cmake_minimum_required(VERSION 3.20)

project(JarvisAI VERSION 1.0)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

add_executable(JarvisAI
    src/main.cpp
    src/logger.cpp
    src/config.cpp
    src/command.cpp
)

target_include_directories(JarvisAI PRIVATE include)
