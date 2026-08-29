cmake_minimum_required(VERSION 3.20)

project(JarvisAI VERSION 1.0 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

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

set(SDL2_DIR
    "C:/SDL2/SDL2-2.30.8/cmake"
)

find_package(SDL2 CONFIG REQUIRED)

# ==========================================
# Whisper.cpp
# ==========================================

add_subdirectory(
    third_party/whisper.cpp
)

# ==========================================
# Jarvis AI Sources
# ==========================================

add_executable(JarvisAI

    src/main.cpp

    # Core
    src/logger.cpp
    src/config.cpp
    src/command.cpp
    src/parser.cpp

    # System
    src/app_launcher.cpp
    src/system_manager.cpp
    src/battery_manager.cpp
    src/volume_manager.cpp

    # Files
    src/file_manager.cpp
    src/notes_manager.cpp
    src/reminder_manager.cpp

    # Web / Information
    src/web_manager.cpp
    src/weather_manager.cpp
    src/news_manager.cpp

    # Utilities
    src/calculator.cpp

    # Media
    src/music_manager.cpp
    src/tts_manager.cpp

    # Voice / AI
    src/speech_manager.cpp
    src/ai_manager.cpp

    # API
    src/api_server.cpp
)

# ==========================================
# Include directories
# ==========================================

target_include_directories(JarvisAI PRIVATE

    ${CMAKE_CURRENT_SOURCE_DIR}/include

    ${CMAKE_CURRENT_SOURCE_DIR}/third_party/whisper.cpp/include
)

# ==========================================
# Libraries
# ==========================================

target_link_libraries(JarvisAI PRIVATE

    whisper

    SDL2::SDL2

    winhttp

    winmm

    ole32

    shell32

    user32

    advapi32
)

# ==========================================
# Windows
# ==========================================

if(WIN32)

    target_compile_definitions(
        JarvisAI
        PRIVATE
        WIN32_LEAN_AND_MEAN
    )

endif()

# ==========================================
# Output directory
# ==========================================

set_target_properties(
    JarvisAI
    PROPERTIES
    RUNTIME_OUTPUT_DIRECTORY
    "${CMAKE_BINARY_DIR}/Release"
)

# ==========================================
# Copy SDL2 DLL beside executable
# ==========================================

if(WIN32)

    add_custom_command(
        TARGET JarvisAI
        POST_BUILD

        COMMAND
        ${CMAKE_COMMAND}
        -E copy_if_different

        "$<TARGET_FILE:SDL2::SDL2>"

        "$<TARGET_FILE_DIR:JarvisAI>"
    )

endif()