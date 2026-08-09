#include "../include/speech_manager.h"

#include <iostream>
#include <string>
#include <vector>
#include <cmath>

#include "whisper.h"
#include <SDL.h>

std::string SpeechManager::Listen()
{
    std::cout << "\n=================================\n";
    std::cout << "        JARVIS LISTENING\n";
    std::cout << "=================================\n";
    std::cout << "Speak now...\n";

    // Initialize SDL audio
    if (SDL_Init(SDL_INIT_AUDIO) != 0)
    {
        std::cout << "SDL audio initialization failed: "
                  << SDL_GetError() << "\n";
        return "";
    }

    SDL_AudioSpec want{};
    SDL_AudioSpec have{};

    want.freq = 16000;
    want.format = AUDIO_F32SYS;
    want.channels = 1;
    want.samples = 4096;
    want.callback = nullptr;

    SDL_AudioDeviceID device =
        SDL_OpenAudioDevice(
            nullptr,
            SDL_TRUE,
            &want,
            &have,
            0);

    if (device == 0)
    {
        std::cout << "Unable to open microphone: "
                  << SDL_GetError() << "\n";

        SDL_Quit();
        return "";
    }

    std::cout << "Microphone connected.\n";
    std::cout << "Listening for 5 seconds...\n";

    SDL_PauseAudioDevice(device, 0);

    std::vector<float> audio;
    audio.reserve(16000 * 5);

    const Uint32 start = SDL_GetTicks();

    while (SDL_GetTicks() - start < 5000)
    {
        Uint32 queued =
            SDL_GetQueuedAudioSize(device);

        if (queued > 0)
        {
            const Uint32 bytesPerSample = sizeof(float);

            Uint32 samples =
                queued / bytesPerSample;

            std::vector<float> buffer(samples);

            SDL_DequeueAudio(
                device,
                buffer.data(),
                queued);

            audio.insert(
                audio.end(),
                buffer.begin(),
                buffer.end());
        }

        SDL_Delay(10);
    }

    SDL_ClearQueuedAudio(device);
    SDL_CloseAudioDevice(device);
    SDL_Quit();

    if (audio.empty())
    {
        std::cout << "No audio captured.\n";
        return "";
    }

    std::cout << "Audio captured: "
              << audio.size()
              << " samples.\n";

    // Load Whisper model
    const char *modelPath =
        "third_party/whisper.cpp/ggml-base.en.bin";

    whisper_context_params cparams =
        whisper_context_default_params();

    whisper_context *ctx =
        whisper_init_from_file_with_params(
            modelPath,
            cparams);

    if (ctx == nullptr)
    {
        std::cout << "Failed to load Whisper model.\n";
        return "";
    }

    // Whisper parameters
    whisper_full_params params =
        whisper_full_default_params(
            WHISPER_SAMPLING_GREEDY);

    params.print_progress = false;
    params.print_special = false;
    params.print_realtime = false;
    params.print_timestamps = false;

    params.translate = false;
    params.language = "en";

    params.n_threads = 4;

    std::cout << "Transcribing...\n";

    int result =
        whisper_full(
            ctx,
            params,
            audio.data(),
            static_cast<int>(audio.size()));

    if (result != 0)
    {
        std::cout << "Whisper transcription failed.\n";

        whisper_free(ctx);
        return "";
    }

    std::string text;

    const int segments =
        whisper_full_n_segments(ctx);

    for (int i = 0; i < segments; ++i)
    {
        const char *segment =
            whisper_full_get_segment_text(
                ctx,
                i);

        if (segment)
        {
            text += segment;
        }
    }

    whisper_free(ctx);

    // Remove leading/trailing spaces
    const size_t first =
        text.find_first_not_of(" \t\n\r");

    const size_t last =
        text.find_last_not_of(" \t\n\r");

    if (first != std::string::npos)
    {
        text =
            text.substr(
                first,
                last - first + 1);
    }
    else
    {
        text.clear();
    }

    std::cout << "\nYou said: "
              << text
              << "\n";

    return text;
}