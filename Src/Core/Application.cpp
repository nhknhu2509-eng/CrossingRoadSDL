#include "Core/Application.h"

#include "Managers/InputManager.h"
#include "Config/GameConfig.h"

#include "Media/VideoPlayer.h"

#include <SDL.h>
#include <SDL_ttf.h>
#include <SDL_image.h>

#include <iostream>
#include <filesystem>
#include <fstream>


// ==================================================
// INITIALIZE
// ==================================================

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


    // ==================================================
    // SDL
    // ==================================================

    if (
        SDL_Init(
            SDL_INIT_VIDEO |
            SDL_INIT_AUDIO) != 0)
    {
        std::cout
            << "SDL_Init Error: "
            << SDL_GetError()
            << std::endl;

        return false;
    }


    // ==================================================
    // SDL TTF
    // ==================================================

    if (TTF_Init() != 0)
    {
        std::cout
            << "TTF_Init Error: "
            << TTF_GetError()
            << std::endl;

        SDL_Quit();

        return false;
    }


    std::cout
        << "TTF initialized: "
        << TTF_WasInit()
        << std::endl;


    // ==================================================
    // SDL IMAGE
    // ==================================================

    const int imageFlags =
        IMG_INIT_PNG |
        IMG_INIT_JPG;


    if (
        (IMG_Init(imageFlags) & imageFlags)
        != imageFlags)
    {
        std::cout
            << "IMG_Init Error: "
            << IMG_GetError()
            << std::endl;

        TTF_Quit();
        SDL_Quit();

        return false;
    }


    // ==================================================
    // WINDOW
    // ==================================================

    if (!window.Create(
        "Crossing Road SDL",
        Config::WINDOW_WIDTH,
        Config::WINDOW_HEIGHT))
    {
        IMG_Quit();
        TTF_Quit();
        SDL_Quit();

        return false;
    }


    // ==================================================
    // FONT
    // ==================================================

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


    // ==================================================
    // GAME BACKGROUND
    // ==================================================

    if (!textureManager.LoadTexture(
        window.GetRenderer(),
        "background",
        "Assets/Images/Background/chapter1_background.png"))
    {
        std::cout
            << "Cannot load background texture!"
            << std::endl;

        return false;
    }


    // ==================================================
    // MENU
    // ==================================================

    if (!textureManager.LoadTexture(
        window.GetRenderer(),
        "menu_background",
        "Assets/Images/UI/Menu/menu_background.jpg"))
    {
        return false;
    }


    if (!textureManager.LoadTexture(
        window.GetRenderer(),
        "menu_logo",
        "Assets/Images/UI/Menu/logo.png"))
    {
        return false;
    }


    if (!textureManager.LoadTexture(
        window.GetRenderer(),
        "startgame_normal",
        "Assets/Images/UI/Menu/Startgame_normal.png"))
    {
        return false;
    }


    if (!textureManager.LoadTexture(
        window.GetRenderer(),
        "startgame_choose",
        "Assets/Images/UI/Menu/Startgame_choose.png"))
    {
        return false;
    }


    if (!textureManager.LoadTexture(
        window.GetRenderer(),
        "loadgame_normal",
        "Assets/Images/UI/Menu/loadgame_normal.png"))
    {
        return false;
    }


    if (!textureManager.LoadTexture(
        window.GetRenderer(),
        "loadgame_choose",
        "Assets/Images/UI/Menu/loadgame_choose.png"))
    {
        return false;
    }


    if (!textureManager.LoadTexture(
        window.GetRenderer(),
        "leaderboard_normal",
        "Assets/Images/UI/Menu/leaderboard_normal.png"))
    {
        return false;
    }


    if (!textureManager.LoadTexture(
        window.GetRenderer(),
        "leaderboard_choose",
        "Assets/Images/UI/Menu/leaderboard_choose.png"))
    {
        return false;
    }


    // ==================================================
    // WAGON 1
    // ==================================================

    if (!textureManager.LoadTexture(
        window.GetRenderer(),
        "wagon1_01",
        "Assets/Images/Vehicles/wagon1_01.png"))
    {
        return false;
    }

    if (!textureManager.LoadTexture(
        window.GetRenderer(),
        "wagon1_02",
        "Assets/Images/Vehicles/wagon1_02.png"))
    {
        return false;
    }

    if (!textureManager.LoadTexture(
        window.GetRenderer(),
        "wagon1_03",
        "Assets/Images/Vehicles/wagon1_03.png"))
    {
        return false;
    }


    // ==================================================
    // WAGON 2
    // ==================================================

    if (!textureManager.LoadTexture(
        window.GetRenderer(),
        "wagon2_01",
        "Assets/Images/Vehicles/wagon2_01.png"))
    {
        return false;
    }

    if (!textureManager.LoadTexture(
        window.GetRenderer(),
        "wagon2_02",
        "Assets/Images/Vehicles/wagon2_02.png"))
    {
        return false;
    }

    if (!textureManager.LoadTexture(
        window.GetRenderer(),
        "wagon2_03",
        "Assets/Images/Vehicles/wagon2_03.png"))
    {
        return false;
    }


    // ==================================================
    // WAGON 3
    // ==================================================

    if (!textureManager.LoadTexture(
        window.GetRenderer(),
        "wagon3_01",
        "Assets/Images/Vehicles/wagon3_01.png"))
    {
        return false;
    }

    if (!textureManager.LoadTexture(
        window.GetRenderer(),
        "wagon3_02",
        "Assets/Images/Vehicles/wagon3_02.png"))
    {
        return false;
    }

    if (!textureManager.LoadTexture(
        window.GetRenderer(),
        "wagon3_03",
        "Assets/Images/Vehicles/wagon3_03.png"))
    {
        return false;
    }


    // ==================================================
    // WAGON 4
    // ==================================================

    if (!textureManager.LoadTexture(
        window.GetRenderer(),
        "wagon4_01",
        "Assets/Images/Vehicles/wagon4_01.png"))
    {
        return false;
    }

    if (!textureManager.LoadTexture(
        window.GetRenderer(),
        "wagon4_02",
        "Assets/Images/Vehicles/wagon4_02.png"))
    {
        return false;
    }

    if (!textureManager.LoadTexture(
        window.GetRenderer(),
        "wagon4_03",
        "Assets/Images/Vehicles/wagon4_03.png"))
    {
        return false;
    }


    // ==================================================
    // DEER
    // ==================================================

    if (!textureManager.LoadTexture(
        window.GetRenderer(),
        "deer_01",
        "Assets/Images/Animals/deer_01.png"))
    {
        return false;
    }

    if (!textureManager.LoadTexture(
        window.GetRenderer(),
        "deer_02",
        "Assets/Images/Animals/deer_02.png"))
    {
        return false;
    }

    if (!textureManager.LoadTexture(
        window.GetRenderer(),
        "deer_03",
        "Assets/Images/Animals/deer_03.png"))
    {
        return false;
    }

    if (!textureManager.LoadTexture(
        window.GetRenderer(),
        "deer_04",
        "Assets/Images/Animals/deer_04.png"))
    {
        return false;
    }

    if (!textureManager.LoadTexture(
        window.GetRenderer(),
        "deer_05",
        "Assets/Images/Animals/deer_05.png"))
    {
        return false;
    }


    // ==================================================
    // SQUIRREL
    // ==================================================

    if (!textureManager.LoadTexture(
        window.GetRenderer(),
        "squirrel_01",
        "Assets/Images/Animals/squirrel_01.png"))
    {
        return false;
    }

    if (!textureManager.LoadTexture(
        window.GetRenderer(),
        "squirrel_02",
        "Assets/Images/Animals/squirrel_02.png"))
    {
        return false;
    }

    if (!textureManager.LoadTexture(
        window.GetRenderer(),
        "squirrel_03",
        "Assets/Images/Animals/squirrel_03.png"))
    {
        return false;
    }

    if (!textureManager.LoadTexture(
        window.GetRenderer(),
        "squirrel_04",
        "Assets/Images/Animals/squirrel_04.png"))
    {
        return false;
    }


    // ==================================================
    // RABBIT
    // ==================================================

    if (!textureManager.LoadTexture(
        window.GetRenderer(),
        "rabbit_01",
        "Assets/Images/Animals/rabbit_01.png"))
    {
        return false;
    }

    if (!textureManager.LoadTexture(
        window.GetRenderer(),
        "rabbit_02",
        "Assets/Images/Animals/rabbit_02.png"))
    {
        return false;
    }

    if (!textureManager.LoadTexture(
        window.GetRenderer(),
        "rabbit_03",
        "Assets/Images/Animals/rabbit_03.png"))
    {
        return false;
    }


    // ==================================================
    // LANTERN
    // ==================================================

    if (!textureManager.LoadTexture(
        window.GetRenderer(),
        "lantern_green",
        "Assets/Images/Lantern/lantern_green.png"))
    {
        return false;
    }

    if (!textureManager.LoadTexture(
        window.GetRenderer(),
        "lantern_red",
        "Assets/Images/Lantern/lantern_red.png"))
    {
        return false;
    }

    if (!textureManager.LoadTexture(
        window.GetRenderer(),
        "lantern_yellow",
        "Assets/Images/Lantern/latern_yellow.png"))
    {
        return false;
    }


    // ==================================================
    // PLAYER
    // ==================================================

    if (!textureManager.LoadTexture(
        window.GetRenderer(),
        "player_idle",
        "Assets/Images/Player/player_idle.png"))
    {
        return false;
    }


    if (!textureManager.LoadTexture(
        window.GetRenderer(),
        "player_walkright1",
        "Assets/Images/Player/player_walkright1.png"))
    {
        return false;
    }


    if (!textureManager.LoadTexture(
        window.GetRenderer(),
        "player_walkright2",
        "Assets/Images/Player/player_walkright2.png"))
    {
        return false;
    }


    if (!textureManager.LoadTexture(
        window.GetRenderer(),
        "player_walkupdown1",
        "Assets/Images/Player/player_walkupdown1.png"))
    {
        return false;
    }


    if (!textureManager.LoadTexture(
        window.GetRenderer(),
        "player_walkupdown2",
        "Assets/Images/Player/player_walkupdown2.png"))
    {
        return false;
    }


    std::cout
        << "=================================="
        << std::endl;

    std::cout
        << "All textures loaded successfully!"
        << std::endl;

    std::cout
        << "=================================="
        << std::endl;


    return true;
}


// ==================================================
// RUN
// ==================================================

void Application::Run()
{
    bool running =
        true;


    VideoPlayer videoPlayer;


    while (running)
    {
        timer.Tick();


        // ==========================================
        // INPUT
        // ==========================================

        InputManager::Update();


        if (
            InputManager::QuitRequested())
        {
            running =
                false;

            continue;
        }


        // ==========================================
        // GAME UPDATE
        // ==========================================

        game.Update();


        // ==========================================
        // INTRO VIDEO
        // ==========================================

        if (
            game.ConsumeIntroRequest())
        {
            bool quitRequested =
                false;


            bool videoPlayed =
                videoPlayer.Play(
                    window.GetRenderer(),
                    "Assets/Videos/chapter1_intro.mp4",
                    quitRequested);


            if (quitRequested)
            {
                running =
                    false;

                continue;
            }


            if (!videoPlayed)
            {
                std::cout
                    << "Intro could not be played."
                    << std::endl;

                std::cout
                    << "Starting Chapter 1 anyway."
                    << std::endl;
            }


            game.SetState(
                GameState::Playing);
        }


        // ==========================================
        // CLEAR
        // ==========================================

        window.Clear();


        // ==========================================
        // BACKGROUND
        // ==========================================

        Texture* background =
            textureManager.GetTexture(
                "background");


        if (
            background != nullptr &&
            background->GetTexture() != nullptr)
        {
            SDL_Rect destination =
            {
                0,
                0,
                Config::WINDOW_WIDTH,
                Config::WINDOW_HEIGHT
            };


            SDL_RenderCopy(
                window.GetRenderer(),
                background->GetTexture(),
                nullptr,
                &destination);
        }


        // ==========================================
        // GAME
        // ==========================================

        game.Render(
            window.GetRenderer(),
            textRenderer,
            fontManager,
            textureManager);


        // ==========================================
        // PRESENT
        // ==========================================

        window.Present();
    }
}


// ==================================================
// SHUTDOWN
// ==================================================

void Application::Shutdown()
{
    textureManager.Destroy();

    fontManager.Destroy();

    window.Destroy();

    IMG_Quit();

    TTF_Quit();

    SDL_Quit();
}