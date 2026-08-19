#pragma once
#pragma once

#include <SDL.h>

class Window
{
public:
    Window();

    bool Create(
        const char* title,
        int width,
        int height);

    void Clear();

    void Present();

    void Destroy();

    SDL_Renderer* GetRenderer();

private:
    SDL_Window* window;
    SDL_Renderer* renderer;
};