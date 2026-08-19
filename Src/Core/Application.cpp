#include "Core/Application.h"

#include "Managers/InputManager.h"

#include <SDL.h>
#include <SDL_ttf.h>
#include <SDL_image.h>

#include <iostream>
#include <filesystem>
#include <fstream>

bool Application::Initialize()
{
    std::cout << "==================================" << std::endl;
    std::cout << "Current Path: "
              << std::filesystem::current_path()
              << std::endl;

    std::ifstream file("Assets/Fonts/NotoSans-VariableFont_wdth,wght.ttf");

    if (file.is_open())
    {
        std::cout << "Font file exists!" << std::endl;
    }
    else
    {
        std::cout << "Font file NOT found!" << std::endl;
    }

    std::cout << "==================================" << std::endl;

    // Khởi tạo SDL
    if (SDL_Init(SDL_INIT_VIDEO) != 0)
    {
        std::cout << "SDL_Init Error: "
                  << SDL_GetError() << std::endl;

        return false;
    }

    // Khởi tạo SDL_ttf
    if (TTF_Init() != 0)
    {
        std::cout << "TTF_Init Error: "
                  << TTF_GetError() << std::endl;

        SDL_Quit();
        return false;
    }

    // Khởi tạo SDL_image
    if (!(IMG_Init(IMG_INIT_PNG) & IMG_INIT_PNG))
    {
        std::cout << "IMG_Init Error: "
                  << IMG_GetError() << std::endl;

        TTF_Quit();
        SDL_Quit();
        return false;
    }

    // Tạo cửa sổ
    if (!window.Create("Crossing Road SDL", 1280, 720))
    {
        IMG_Quit();
        TTF_Quit();
        SDL_Quit();

        return false;
    }

    // Load font
    if (!fontManager.LoadFont(
            "default",
            "Assets/Fonts/NotoSans-VariableFont_wdth,wght.ttf",
            28))
    {
        std::cout << "Cannot load font!" << std::endl;

        window.Destroy();
        IMG_Quit();
        TTF_Quit();
        SDL_Quit();

        return false;
    }

    // Load texture
    /*
    if (!textureManager.LoadTexture(
            window.GetRenderer(),
            "player",
            "Assets/Images/Player/player.png"))
    {
        std::cout << "Cannot load player texture!" << std::endl;
        std::cout << IMG_GetError() << std::endl;

        fontManager.Destroy();
        window.Destroy();

        IMG_Quit();
        TTF_Quit();
        SDL_Quit();

        return false;
    }
    */
    return true;
}

void Application::Run()
{
    bool running = true;

    while (running)
    {
        timer.Tick();

        InputManager::Update();

        if (InputManager::QuitRequested())
        {
            running = false;
        }

        game.Update();

        window.Clear();

        game.Render(
            window.GetRenderer(),
            textRenderer,
            fontManager);

        window.Present();
    }
}

void Application::Shutdown()
{
    textureManager.Destroy();

    fontManager.Destroy();

    window.Destroy();

    IMG_Quit();

    TTF_Quit();

    SDL_Quit();
}