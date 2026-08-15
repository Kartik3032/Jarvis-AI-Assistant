cmake_minimum_required(VERSION 3.20)

project(JarvisAI VERSION 1.0)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# ==========================================
# Whisper.cpp options
# ==========================================

set(WHISPER_BUILD_TESTS OFF CACHE BOOL "" FORCE)
set(WHISPER_BUILD_EXAMPLES OFF CACHE BOOL "" FORCE)
set(WHISPER_BUILD_SERVER OFF CACHE BOOL "" FORCE)
set(WHISPER_SDL2 ON CACHE BOOL "" FORCE)

# ==========================================
# SDL2
# ==========================================

set(SDL2_DIR "C:/SDL2/SDL2-2.30.8/cmake")

find_package(SDL2 CONFIG REQUIRED)

# ==========================================
# Whisper.cpp
# ==========================================

add_subdirectory(third_party/whisper.cpp)

# ==========================================
# Jarvis AI
# ==========================================

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

    src/tts_manager.cpp
    src/speech_manager.cpp
    src/ai_manager.cpp
)

# ==========================================
# Include directories
# ==========================================

target_include_directories(JarvisAI PRIVATE

    include

    third_party/whisper.cpp/include
)

# ==========================================
# Libraries
# ==========================================

target_link_libraries(JarvisAI PRIVATE
winhttp
    whisper

    SDL2::SDL2
)