cmake_minimum_required(VERSION 3.20)

project(JarvisAI VERSION 1.0)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

   add_executable(JarvisAI
    src/main.cpp
    src/logger.cpp
    src/config.cpp
    src/command.cpp
    src/parser.cpp
    src/app_launcher.cpp
    src/file_manager.cpp
    src/system_manager.cpp
    src/web_manager.cpp
    src/calculator.cpp
    src/notes_manager.cpp
    src/reminder_manager.cpp
    src/weather_manager.cpp
    src/news_manager.cpp
    src/battery_manager.cpp
    src/volume_manager.cpp
    src/music_manager.cpp
)

target_include_directories(JarvisAI PRIVATE include)
