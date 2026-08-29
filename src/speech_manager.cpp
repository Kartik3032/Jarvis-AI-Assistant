#include "../include/speech_manager.h"

#include <iostream>
#include <string>
#include <vector>
#include <cmath>
#include <algorithm>

#include "whisper.h"
#include <SDL.h>

std::string SpeechManager::Listen()
{
    std::cout << "\n=================================\n";
    std::cout << "        JARVIS LISTENING\n";
    std::cout << "=================================\n";
    std::cout << "Speak now...\n";

    if (SDL_Init(SDL_INIT_AUDIO) != 0)
    {
        std::cout << "SDL initialization failed: "
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
        std::cout << "Microphone error: "
                  << SDL_GetError() << "\n";

        SDL_Quit();
        return "";
    }

    std::cout << "Microphone connected.\n";
    std::cout << "Actual mic format: "
              << have.freq
              << " Hz, "
              << static_cast<int>(have.channels)
              << " channel(s)\n";

    // Give microphone a moment to initialize
    SDL_Delay(300);

    SDL_PauseAudioDevice(device, 0);

    constexpr int RECORD_SECONDS = 6;
    constexpr int SAMPLE_RATE = 16000;

    std::vector<float> audio;
    audio.reserve(SAMPLE_RATE * RECORD_SECONDS);

    std::cout << "Listening for "
              << RECORD_SECONDS
              << " seconds...\n";

    const Uint32 start = SDL_GetTicks();

    while (SDL_GetTicks() - start <
           RECORD_SECONDS * 1000)
    {
        Uint32 queued =
            SDL_GetQueuedAudioSize(device);

        if (queued > 0)
        {
            Uint32 bytesPerSample =
                sizeof(float);

            Uint32 samples =
                queued / bytesPerSample;

            std::vector<float> buffer(samples);

            Uint32 received =
                SDL_DequeueAudio(
                    device,
                    buffer.data(),
                    queued);

            Uint32 receivedSamples =
                received / bytesPerSample;

            audio.insert(
                audio.end(),
                buffer.begin(),
                buffer.begin() + receivedSamples);
        }

        SDL_Delay(10);
    }

    SDL_ClearQueuedAudio(device);
    SDL_CloseAudioDevice(device);
    SDL_Quit();

    // --------------------------------
    // Check whether microphone actually
    // captured meaningful audio
    // --------------------------------

    if (audio.empty())
    {
        std::cout
            << "No audio captured.\n";

        return "";
    }

    float maxAmplitude = 0.0f;

    for (float sample : audio)
    {
        float value =
            std::abs(sample);

        if (value > maxAmplitude)
            maxAmplitude = value;
    }

    std::cout
        << "Audio captured: "
        << audio.size()
        << " samples\n";

    std::cout
        << "Maximum audio level: "
        << maxAmplitude
        << "\n";

    // Very quiet microphone = don't send
    // garbage to Whisper
    if (maxAmplitude < 0.01f)
    {
        std::cout
            << "Audio level too low. "
            << "Please speak closer to the microphone.\n";

        return "";
    }

    // --------------------------------
    // Whisper
    // --------------------------------

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
        std::cout
            << "Failed to load Whisper model.\n";

        return "";
    }

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

    // Better short-command recognition
    params.no_context = true;
    params.single_segment = false;

    // Reduce hallucinations during silence/noise
    params.no_speech_thold = 0.6f;

    // Deterministic decoding
    params.temperature = 0.0f;
    params.temperature_inc = 0.0f;

    std::cout << "Transcribing...\n";

    int result =
        whisper_full(
            ctx,
            params,
            audio.data(),
            static_cast<int>(audio.size()));

    if (result != 0)
    {
        std::cout
            << "Whisper transcription failed.\n";

        whisper_free(ctx);
        return "";
    }

    std::string text;

    int segments =
        whisper_full_n_segments(ctx);

    for (int i = 0; i < segments; ++i)
    {
        const char *segment =
            whisper_full_get_segment_text(
                ctx,
                i);

        if (segment)
            text += segment;
    }

    whisper_free(ctx);

    // --------------------------------
    // Trim
    // --------------------------------

    const size_t first =
        text.find_first_not_of(
            " \t\n\r");

    const size_t last =
        text.find_last_not_of(
            " \t\n\r");

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

    // --------------------------------
    // Reject Whisper garbage
    // --------------------------------

    if (text.empty())
        return "";

    if (text == "[BLANK_AUDIO]" ||
        text == "[BLANK AUDIO]")
    {
        return "";
    }

    std::cout
        << "\nYou said: "
        << text
        << "\n";

    return text;
}