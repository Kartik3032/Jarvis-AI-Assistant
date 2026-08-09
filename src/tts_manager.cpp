#include "../include/tts_manager.h"

#include <windows.h>
#include <sapi.h>
#include <iostream>
#include <string>

#pragma comment(lib, "sapi.lib")

void TTSManager::Speak(const std::string &text)
{
    // Initialize COM
    HRESULT comHr = CoInitializeEx(nullptr, COINIT_APARTMENTTHREADED);

    if (FAILED(comHr) && comHr != RPC_E_CHANGED_MODE)
    {
        std::cout << "Unable to initialize COM. HRESULT = 0x"
                  << std::hex << comHr << std::dec << std::endl;
        return;
    }

    ISpVoice *voice = nullptr;

    HRESULT hr = CoCreateInstance(
        CLSID_SpVoice,
        nullptr,
        CLSCTX_ALL,
        IID_ISpVoice,
        reinterpret_cast<void **>(&voice));

    if (FAILED(hr))
    {
        std::cout << "Unable to initialize voice system. HRESULT = 0x"
                  << std::hex << hr << std::dec << std::endl;

        if (comHr != RPC_E_CHANGED_MODE)
        {
            CoUninitialize();
        }

        return;
    }

    int sizeNeeded = MultiByteToWideChar(
        CP_UTF8,
        0,
        text.c_str(),
        -1,
        nullptr,
        0);

    if (sizeNeeded <= 0)
    {
        voice->Release();

        if (comHr != RPC_E_CHANGED_MODE)
        {
            CoUninitialize();
        }

        return;
    }

    std::wstring wideText(sizeNeeded, L'\0');

    MultiByteToWideChar(
        CP_UTF8,
        0,
        text.c_str(),
        -1,
        wideText.data(),
        sizeNeeded);

    HRESULT speakHr = voice->Speak(
        wideText.c_str(),
        SPF_DEFAULT,
        nullptr);

    if (FAILED(speakHr))
    {
        std::cout << "Voice Speak failed. HRESULT = 0x"
                  << std::hex << speakHr << std::dec << std::endl;
    }

    voice->Release();

    if (comHr != RPC_E_CHANGED_MODE)
    {
        CoUninitialize();
    }
}