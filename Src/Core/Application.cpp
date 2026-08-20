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
    std::cout
        << "=================================="
        << std::endl;

    std::cout
        << "Current Path: "
        << std::filesystem::current_path()
        << std::endl;

    std::ifstream file(
        "Assets/Fonts/NotoSans-VariableFont_wdth,wght.ttf");

    if (file.is_open())
    {
        std::cout
            << "Font file exists!"
            << std::endl;
    }
    else
    {
        std::cout
            << "Font file NOT found!"
            << std::endl;
    }

    std::cout
        << "=================================="
        << std::endl;

    if (SDL_Init(SDL_INIT_VIDEO) != 0)
    {
        std::cout
            << "SDL_Init Error: "
            << SDL_GetError()
            << std::endl;

        return false;
    }

    if (TTF_Init() != 0)
    {
        std::cout
            << "TTF_Init Error: "
            << TTF_GetError()
            << std::endl;

        SDL_Quit();
        return false;
    }

    if (!(IMG_Init(IMG_INIT_PNG) &
        IMG_INIT_PNG))
    {
        std::cout
            << "IMG_Init Error: "
            << IMG_GetError()
            << std::endl;

        TTF_Quit();
        SDL_Quit();
        return false;
    }

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

    if (!fontManager.LoadFont(
        "default",
        "Assets/Fonts/NotoSans-VariableFont_wdth,wght.ttf",
        28))
    {
        std::cout
            << "Cannot load font!"
            << std::endl;

        window.Destroy();
        IMG_Quit();
        TTF_Quit();
        SDL_Quit();
        return false;
    }

    // ==============================
    // BACKGROUND
    // ==============================

    if (!textureManager.LoadTexture(
        window.GetRenderer(),
        "background",
        "Assets/Images/Background/chapter1_background.png"))
    {
        std::cout
            << "Cannot load background texture!"
            << std::endl;

        std::cout
            << IMG_GetError()
            << std::endl;

        fontManager.Destroy();
        window.Destroy();
        IMG_Quit();
        TTF_Quit();
        SDL_Quit();
        return false;
    }

    // ==============================
    // VEHICLE TEXTURES
    // ==============================

    if (!textureManager.LoadTexture(
        window.GetRenderer(),
        "wagon_01",
        "Assets/Images/Vehicles/wagon_01.png"))
        return false;

    if (!textureManager.LoadTexture(
        window.GetRenderer(),
        "wagon_02",
        "Assets/Images/Vehicles/wagon_02.png"))
        return false;

    if (!textureManager.LoadTexture(
        window.GetRenderer(),
        "wagon_03",
        "Assets/Images/Vehicles/wagon_03.png"))
        return false;

    if (!textureManager.LoadTexture(
        window.GetRenderer(),
        "wagon_04",
        "Assets/Images/Vehicles/wagon_04.png"))
        return false;

    // ==============================
    // ANIMAL TEXTURES
    // ==============================

    if (!textureManager.LoadTexture(
        window.GetRenderer(),
        "deer",
        "Assets/Images/Animals/deer.png"))
        return false;

    if (!textureManager.LoadTexture(
        window.GetRenderer(),
        "squirrel",
        "Assets/Images/Animals/squirrel.png"))
        return false;

    if (!textureManager.LoadTexture(
        window.GetRenderer(),
        "rabbit",
        "Assets/Images/Animals/rabbit.png"))
        return false;

    // ==============================
    // TRAFFIC LIGHT SPRITES
    // ==============================

    if (!textureManager.LoadTexture(
        window.GetRenderer(),
        "lantern_green",
        "Assets/Images/Lantern/lantern_green.png"))
        return false;

    if (!textureManager.LoadTexture(
        window.GetRenderer(),
        "lantern_red",
        "Assets/Images/Lantern/lantern_red.png"))
        return false;

    // ==============================
    // PLAYER SPRITE
    // ==============================

    if (!textureManager.LoadTexture(
        window.GetRenderer(),
        "player",
        "Assets/Images/Player/player_idle.png"))
        return false;

    std::cout
        << "All textures loaded successfully!"
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

        window.Clear();

        Texture* background =
            textureManager.GetTexture(
                "background");

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

            // ==========================================
            // REMOVE TOP GREEN BANNER FROM BACKGROUND
            // ==========================================
            //
            // chapter1_background.png contains a 60px
            // green banner at the very top.
            //
            // Replace that strip with the clean background
            // immediately below it. This keeps all lane
            // coordinates unchanged.
            //

            constexpr int TOP_BANNER_HEIGHT = 60;

            SDL_Rect cleanSource;
            cleanSource.x = 0;
            cleanSource.y = TOP_BANNER_HEIGHT;
            cleanSource.w = 1280;
            cleanSource.h = TOP_BANNER_HEIGHT;

            SDL_Rect cleanDestination;
            cleanDestination.x = 0;
            cleanDestination.y = 0;
            cleanDestination.w = 1280;
            cleanDestination.h = TOP_BANNER_HEIGHT;

            SDL_RenderCopy(
                window.GetRenderer(),
                background->GetTexture(),
                &cleanSource,
                &cleanDestination);
        }

        game.Render(
            window.GetRenderer(),
            textRenderer,
            fontManager,
            textureManager);

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
