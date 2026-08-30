#pragma once

#include <SDL.h>
#include <string>

class VideoPlayer
{
public:
    VideoPlayer();
    ~VideoPlayer();

    bool Play(
        SDL_Renderer* renderer,
        const std::string& videoPath,
        bool& quitRequested);
};