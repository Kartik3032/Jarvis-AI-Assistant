# 🤖 JARVIS AI Assistant

> A Windows-based AI voice assistant built from scratch using **C++**, **Whisper.cpp**, **SDL2**, and a modern **React** frontend.

JARVIS is a personal desktop AI assistant capable of understanding voice commands, executing system operations, opening applications and websites, managing files, controlling media, checking system information, and communicating with an AI backend.

The project combines a **C++ native backend** with a **React-based command center UI** to create a complete desktop AI assistant experience.

---

## ✨ Features

### 🎙️ Speech Recognition
- Real-time microphone input
- Audio capture using SDL2
- Speech-to-text using Whisper.cpp
- English language recognition
- Noise/silence filtering
- Configurable recording duration
- Audio-level validation

### 🧠 AI Integration
- Natural-language fallback through AIManager
- Command routing between deterministic commands and AI responses
- Text-to-speech responses
- Modular AI architecture

### 🌐 Web Control
JARVIS can open:

- Google
- YouTube
- GitHub
- ChatGPT
- Gmail
- LinkedIn
- LeetCode
- GeeksforGeeks

It can also perform:

- Google searches
- YouTube searches
- Weather queries

### 💻 System Control

JARVIS can:

- Open applications
- Open File Explorer
- Check system information
- Check battery status
- Increase volume
- Decrease volume
- Mute/unmute volume

### 📁 File Management

Supported operations include:

- Create files
- Delete files
- Create folders
- Delete folders
- Rename files/folders
- List files
- Open File Explorer

### 🎵 Music Control

JARVIS supports:

- Opening the music folder
- Playing music
- Pausing music
- Resuming music
- Stopping music

### 📝 Productivity

JARVIS includes:

- Create notes
- Show notes
- Create reminders
- Show reminders

### 🧮 Calculator

Supports natural expressions such as:

```text
calculate 25 plus 10
calculate 100 divided by 4
what is 20 multiplied by 5
what is 50 minus 12
