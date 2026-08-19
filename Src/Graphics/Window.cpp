#include "Graphics/Window.h"

#include <iostream>

Window::Window()
{
    window = nullptr;
    renderer = nullptr;
}

bool Window::Create(
    const char* title,
    int width,
    int height)
{
    window = SDL_CreateWindow(
        title,
        SDL_WINDOWPOS_CENTERED,
        SDL_WINDOWPOS_CENTERED,
        width,
        height,
        SDL_WINDOW_SHOWN);

    if (window == nullptr)
    {
        std::cout << SDL_GetError() << std::endl;
        return false;
    }

    renderer = SDL_CreateRenderer(
        window,
        -1,
        SDL_RENDERER_ACCELERATED |
        SDL_RENDERER_PRESENTVSYNC);

    if (renderer == nullptr)
    {
        std::cout << SDL_GetError() << std::endl;
        return false;
    }

    return true;
}

void Window::Clear()
{
   /* SDL_SetRenderDrawColor(
        renderer,
        40,
        160,
        40,
        255);*/
    SDL_SetRenderDrawColor(
        renderer,
        135,   // Red
        206,   // Green
        235,   // Blue
        255);  // Alpha

    
    
    SDL_RenderClear(renderer);
}

void Window::Present()
{
    SDL_RenderPresent(renderer);
}

void Window::Destroy()
{
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
}

SDL_Renderer* Window::GetRenderer()
{
    return renderer;
}