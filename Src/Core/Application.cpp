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

    std::ifstream file(
        "Assets/Fonts/NotoSans-VariableFont_wdth,wght.ttf");

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
            << SDL_GetError()
            << std::endl;

        return false;
    }

    // Khởi tạo SDL_ttf
    if (TTF_Init() != 0)
    {
        std::cout << "TTF_Init Error: "
            << TTF_GetError()
            << std::endl;

        SDL_Quit();

        return false;
    }

    // Khởi tạo SDL_image
    if (!(IMG_Init(IMG_INIT_PNG) & IMG_INIT_PNG))
    {
        std::cout << "IMG_Init Error: "
            << IMG_GetError()
            << std::endl;

        TTF_Quit();
        SDL_Quit();

        return false;
    }

    // Tạo cửa sổ
    if (!window.Create(
        "Crossing Road SDL",
        1280,
        720))
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
        std::cout << "Cannot load font!"
            << std::endl;

        window.Destroy();

        IMG_Quit();
        TTF_Quit();
        SDL_Quit();

        return false;
    }

    // Load background
    if (!textureManager.LoadTexture(
        window.GetRenderer(),
        "background",
        "Assets/Images/Background/chapter1_background.png"))
    {
        std::cout << "Cannot load background texture!"
            << std::endl;

        std::cout << IMG_GetError()
            << std::endl;

        fontManager.Destroy();
        window.Destroy();

        IMG_Quit();
        TTF_Quit();
        SDL_Quit();

        return false;
    }

    std::cout << "Background loaded successfully!"
        << std::endl;
    // Load vehicle textures

    if (!textureManager.LoadTexture(
        window.GetRenderer(),
        "wagon_01",
        "Assets/Images/Vehicles/wagon_01.png"))
    {
        std::cout << "Cannot load wagon_01!"
            << std::endl;

        return false;
    }

    if (!textureManager.LoadTexture(
        window.GetRenderer(),
        "wagon_02",
        "Assets/Images/Vehicles/wagon_02.png"))
    {
        std::cout << "Cannot load wagon_02!"
            << std::endl;

        return false;
    }

    if (!textureManager.LoadTexture(
        window.GetRenderer(),
        "wagon_03",
        "Assets/Images/Vehicles/wagon_03.png"))
    {
        std::cout << "Cannot load wagon_03!"
            << std::endl;

        return false;
    }

    if (!textureManager.LoadTexture(
        window.GetRenderer(),
        "wagon_04",
        "Assets/Images/Vehicles/wagon_04.png"))
    {
        std::cout << "Cannot load wagon_04!"
            << std::endl;

        return false;
    }

    std::cout << "Vehicle textures loaded successfully!"
        << std::endl;
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

        // Xóa frame trước
        window.Clear();

        // ==============================
        // VẼ BACKGROUND
        // ==============================

        Texture* background =
            textureManager.GetTexture("background");

        if (background != nullptr &&
            background->GetTexture() != nullptr)
        {
            SDL_Rect destination;

            destination.x = 0;
            destination.y = 0;
            destination.w = 1280;
            destination.h = 720;

            SDL_RenderCopy(
                window.GetRenderer(),
                background->GetTexture(),
                nullptr,
                &destination);
        }

        // ==============================
        // VẼ GAME
        // ==============================

        game.Render(
            window.GetRenderer(),
            textRenderer,
            fontManager,
            textureManager);

        // Hiển thị frame
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