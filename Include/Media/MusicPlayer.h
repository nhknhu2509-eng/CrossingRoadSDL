#pragma once

#include <SDL.h>

#include <cstddef>
#include <cstdint>
#include <string>
#include <vector>


class MusicPlayer
{
public:

    MusicPlayer();

    ~MusicPlayer();


    // ==========================================
    // LOAD / PLAY
    // ==========================================

    bool Load(
        const std::string& filePath);


    void Play();

    void Pause();

    void Resume();

    void Stop();


    bool IsLoaded() const;


private:

    // ==========================================
    // SDL AUDIO CALLBACK
    // ==========================================

    static void AudioCallback(
        void* userdata,
        Uint8* stream,
        int length);


    void FillAudio(
        Uint8* stream,
        int length);


private:

    SDL_AudioDeviceID audioDevice;

    std::vector<uint8_t> audioData;

    std::size_t audioPosition;

    bool loaded;
};
