#include "Core/Game.h"


#include "Managers/CollisionManager.h"
#include "Managers/InputManager.h"


#include "Objects/Obstacle.h"


#include <string>
#include <iostream>


// ==================================================
// HELPER FUNCTIONS
// ==================================================

namespace
{

    // ==================================================
    // STATE FRAME RECT
    // ==================================================

    SDL_Rect GetStateFrameRect()
    {
        const int frameWidth =
            760;


        const int frameHeight =
            430;


        SDL_Rect frameRect =
        {
            (1280 - frameWidth) / 2,
            (720 - frameHeight) / 2,

            frameWidth,
            frameHeight
        };


        return frameRect;
    }


    // ==================================================
    // DRAW STATE OVERLAY
    // ==================================================

    void DrawStateOverlay(
        SDL_Renderer* renderer,
        TextureManager& textureManager,
        const std::string& textureName)
    {
        // ==========================================
        // DARKEN GAME SCREEN
        // ==========================================

        SDL_SetRenderDrawBlendMode(
            renderer,
            SDL_BLENDMODE_BLEND);


        SDL_SetRenderDrawColor(
            renderer,
            0,
            0,
            0,
            140);


        SDL_Rect darkOverlay =
        {
            0,
            0,
            1280,
            720
        };


        SDL_RenderFillRect(
            renderer,
            &darkOverlay);


        // ==========================================
        // GET FRAME
        // ==========================================

        Texture* frame =
            textureManager.GetTexture(
                textureName);


        if (
            frame == nullptr ||
            frame->GetTexture() == nullptr)
        {
            return;
        }


        SDL_Rect frameRect =
            GetStateFrameRect();


        // ==========================================
        // DRAW FRAME
        // ==========================================

        SDL_RenderCopy(
            renderer,
            frame->GetTexture(),
            nullptr,
            &frameRect);
    }


    // ==================================================
    // LOAD HUD TEXTURES
    // ==================================================

    void EnsureHudTexturesLoaded(
        SDL_Renderer* renderer,
        TextureManager& textureManager)
    {
        static bool loadAttempted =
            false;


        if (loadAttempted)
        {
            return;
        }


        loadAttempted =
            true;


        // ==========================================
        // SCORE FRAME
        // ==========================================

        if (
            textureManager.GetTexture(
                "score_frame") == nullptr)
        {
            textureManager.LoadTexture(
                renderer,
                "score_frame",
                "Assets/Images/UI/HUD/score_frame.png");
        }


        // ==========================================
        // PAUSE HINT
        // ==========================================

        if (
            textureManager.GetTexture(
                "pause_hint") == nullptr)
        {
            textureManager.LoadTexture(
                renderer,
                "pause_hint",
                "Assets/Images/UI/HUD/pause_hint.png");
        }
    }


    // ==================================================
    // DRAW GAMEPLAY HUD
    // ==================================================

    void DrawGameplayHud(
        SDL_Renderer* renderer,
        TextRenderer& textRenderer,
        FontManager& fontManager,
        TextureManager& textureManager,
        int score)
    {
        EnsureHudTexturesLoaded(
            renderer,
            textureManager);


        // ==========================================
        // HUD SIZE
        // ==========================================

        const int hudWidth =
            300;


        const int hudHeight =
            53;


        // ==========================================
        // SCORE FRAME
        // ==========================================

        SDL_Rect scoreRect =
        {
            20,
            18,

            hudWidth,
            hudHeight
        };


        Texture* scoreFrame =
            textureManager.GetTexture(
                "score_frame");


        if (
            scoreFrame != nullptr &&
            scoreFrame->GetTexture() != nullptr)
        {
            SDL_RenderCopy(
                renderer,
                scoreFrame->GetTexture(),
                nullptr,
                &scoreRect);
        }


        // ==========================================
        // PAUSE HINT
        // ==========================================

        SDL_Rect pauseRect =
        {
            1280 - hudWidth - 20,
            18,

            hudWidth,
            hudHeight
        };


        Texture* pauseHint =
            textureManager.GetTexture(
                "pause_hint");


        if (
            pauseHint != nullptr &&
            pauseHint->GetTexture() != nullptr)
        {
            SDL_RenderCopy(
                renderer,
                pauseHint->GetTexture(),
                nullptr,
                &pauseRect);
        }


        // ==========================================
        // SCORE TEXT
        // ==========================================

        TTF_Font* font =
            fontManager.GetFont(
                "default");


        if (font == nullptr)
        {
            return;
        }


        std::string scoreText =
            std::to_string(
                score);


        int textWidth =
            0;


        int textHeight =
            0;


        TTF_SizeUTF8(
            font,
            scoreText.c_str(),
            &textWidth,
            &textHeight);


        // ==========================================
        // SCORE TEXT POSITION
        // ==========================================

        const int iconAreaWidth =
            75;


        const int woodAreaWidth =
            hudWidth -
            iconAreaWidth;


        int scoreX =
            scoreRect.x +
            iconAreaWidth +
            (woodAreaWidth - textWidth) / 2;


        int scoreY =
            scoreRect.y +
            (hudHeight - textHeight) / 2;


        // ==========================================
        // DRAW SCORE
        // ==========================================

        textRenderer.Draw(
            renderer,
            font,
            scoreText,
            scoreX,
            scoreY,
            { 245, 225, 190, 255 });
    }


    // ==================================================
    // DRAW FINAL SCORE
    // ==================================================

    void DrawFinalScore(
        SDL_Renderer* renderer,
        TextRenderer& textRenderer,
        FontManager& fontManager,
        int score)
    {
        TTF_Font* font =
            fontManager.GetFont(
                "default");


        if (font == nullptr)
        {
            return;
        }


        std::string finalScoreText =
            "SCORE: " +
            std::to_string(
                score);


        int textWidth =
            0;


        int textHeight =
            0;


        TTF_SizeUTF8(
            font,
            finalScoreText.c_str(),
            &textWidth,
            &textHeight);


        SDL_Rect frameRect =
            GetStateFrameRect();


        // ==========================================
        // CENTER SCORE ON STATE FRAME
        // ==========================================

        int scoreX =
            frameRect.x +
            (frameRect.w - textWidth) / 2;


        int scoreY =
            frameRect.y + 250;


        textRenderer.Draw(
            renderer,
            font,
            finalScoreText,
            scoreX,
            scoreY,
            { 245, 225, 190, 255 });
    }
}


// ==================================================
// CONSTRUCTOR
// ==================================================

Game::Game()
{
    state =
        GameState::Menu;


    introRequested =
        false;


    score =
        0;
}


// ==================================================
// UPDATE
// ==================================================

void Game::Update()
{
    // ==========================================
    // MENU
    // ==========================================

    if (
        state ==
        GameState::Menu)
    {
        menu.Update();


        if (
            InputManager::IsKeyPressed(
                SDL_SCANCODE_RETURN))
        {
            switch (
                menu.GetSelectedIndex())
            {
                // ==================================
                // START GAME
                // ==================================

            case 0:
            {
                // ==================================
                // RESET PLAYER
                // ==================================

                player.Reset();


                // ==================================
                // RESET SCORE
                // ==================================

                score =
                    0;


                // ==================================
                // RESET PINE CONES
                // ==================================

                pineConeManager.Reset();


                // ==================================
                // INTRO
                // ==================================

                introRequested =
                    true;


                break;
            }


            // ==================================
            // LOAD GAME
            // ==================================

            case 1:
            {
                if (LoadGame())
                {
                    std::cout
                        << "Load game successful!"
                        << std::endl;
                }
                else
                {
                    std::cout
                        << "No valid save game found."
                        << std::endl;
                }


                break;
            }


            // ==================================
            // LEADERBOARD
            // ==================================

            case 2:

                // Leaderboard sẽ làm sau.

                break;


            default:

                break;
            }
        }


        return;
    }


    // ==========================================
    // SAVE GAME
    //
    // Chỉ được Save khi:
    //
    // Paused
    // GameOver
    // LevelComplete
    // ==========================================

    if (
        InputManager::IsKeyPressed(
            SDL_SCANCODE_S))
    {
        if (
            state == GameState::Paused ||
            state == GameState::GameOver ||
            state == GameState::LevelComplete)
        {
            if (SaveGame())
            {
                std::cout
                    << "Game saved successfully!"
                    << std::endl;
            }
            else
            {
                std::cout
                    << "Cannot save game!"
                    << std::endl;
            }
        }
    }


    // ==========================================
    // PAUSE
    // ==========================================

    if (
        state ==
        GameState::Playing &&
        InputManager::IsKeyPressed(
            SDL_SCANCODE_P))
    {
        state =
            GameState::Paused;


        return;
    }


    // ==========================================
    // RESUME
    // ==========================================

    if (
        state ==
        GameState::Paused &&
        InputManager::IsKeyPressed(
            SDL_SCANCODE_P))
    {
        state =
            GameState::Playing;


        return;
    }


    // ==========================================
    // PAUSED
    // ==========================================

    if (
        state ==
        GameState::Paused)
    {
        return;
    }


    // ==========================================
    // NON-PLAYING STATES
    // ==========================================

    if (
        state !=
        GameState::Playing)
    {
        return;
    }


    // ==========================================
    // 1. PLAYER UPDATE
    // ==========================================

    player.Update();


    // ==========================================
    // 1.5 PINE CONE COLLECTION
    // ==========================================

    int collectedScore =
        pineConeManager.CheckCollection(
            player.GetHitbox());


    if (
        collectedScore > 0)
    {
        score +=
            collectedScore;
    }


    // ==========================================
    // 2. GOAL CHECK
    // ==========================================

    if (
        map.GetGoal().Reached(
            player.GetRect()))
    {
        state =
            GameState::LevelComplete;


        return;
    }


    // ==========================================
    // 3. MAP UPDATE
    // ==========================================

    map.Update();


    // ==========================================
    // 4. COLLISION CHECK
    // ==========================================

    for (
        const Obstacle* obstacle :
        map.GetLaneManager().
        GetObstacles())
    {
        if (
            CollisionManager::
            CheckCollision(
                player.GetHitbox(),
                obstacle->GetHitbox()))
        {
            player.Reset();


            state =
                GameState::GameOver;


            return;
        }
    }
}


// ==================================================
// RENDER
// ==================================================

void Game::Render(
    SDL_Renderer* renderer,
    TextRenderer& textRenderer,
    FontManager& fontManager,
    TextureManager& textureManager)
{
    // ==========================================
    // MENU
    // ==========================================

    if (
        state ==
        GameState::Menu)
    {
        menu.Draw(
            renderer,
            textureManager);


        return;
    }


    // ==========================================
    // WORLD
    // ==========================================

    map.Draw(
        renderer,
        textureManager);


    // ==========================================
    // PINE CONES
    // ==========================================

    pineConeManager.Draw(
        renderer,
        textureManager);


    // ==========================================
    // PLAYER
    // ==========================================

    player.Draw(
        renderer,
        textureManager);


    // ==========================================
    // GAMEPLAY HUD
    // ==========================================

    DrawGameplayHud(
        renderer,
        textRenderer,
        fontManager,
        textureManager,
        score);


    // ==========================================
    // STATE UI
    // ==========================================

    switch (state)
    {
        // ======================================
        // NORMAL GAME
        // ======================================

    case GameState::Playing:

        break;


        // ======================================
        // PAUSE
        // ======================================

    case GameState::Paused:

        DrawStateOverlay(
            renderer,
            textureManager,
            "pause_frame");


        break;


        // ======================================
        // LOSE
        // ======================================

    case GameState::GameOver:

        DrawStateOverlay(
            renderer,
            textureManager,
            "lose_frame");


        DrawFinalScore(
            renderer,
            textRenderer,
            fontManager,
            score);


        break;


        // ======================================
        // WIN
        // ======================================

    case GameState::LevelComplete:

        DrawStateOverlay(
            renderer,
            textureManager,
            "win_frame");


        DrawFinalScore(
            renderer,
            textRenderer,
            fontManager,
            score);


        break;


    default:

        break;
    }
}


// ==================================================
// SET STATE
// ==================================================

void Game::SetState(
    GameState newState)
{
    state =
        newState;
}


// ==================================================
// GET STATE
// ==================================================

GameState
Game::GetState() const
{
    return state;
}


// ==================================================
// CONSUME INTRO REQUEST
// ==================================================

bool Game::ConsumeIntroRequest()
{
    if (!introRequested)
    {
        return false;
    }


    introRequested =
        false;


    return true;
}


// ==================================================
// SAVE GAME
// ==================================================

bool Game::SaveGame()
{
    // ==========================================
    // ONLY VALID SAVE STATES
    // ==========================================

    if (
        state != GameState::Paused &&
        state != GameState::GameOver &&
        state != GameState::LevelComplete)
    {
        return false;
    }


    SaveData data;


    // ==========================================
    // GAME STATE
    // ==========================================

    data.state =
        state;


    // ==========================================
    // SCORE
    // ==========================================

    data.score =
        score;


    // ==========================================
    // PLAYER POSITION
    // ==========================================

    const SDL_Rect& playerRect =
        player.GetRect();


    data.playerX =
        playerRect.x;


    data.playerY =
        playerRect.y;


    // ==========================================
    // PINE CONES
    // ==========================================

    data.pineConeCollected =
        pineConeManager.
        GetCollectedStates();


    // ==========================================
    // OBSTACLES
    // ==========================================

    std::vector<const Obstacle*>
        obstacles =
        map.GetLaneManager().
        GetObstacles();


    data.obstaclePositions.clear();


    for (
        const Obstacle* obstacle :
        obstacles)
    {
        const SDL_Rect& obstacleRect =
            obstacle->GetRect();


        SDL_Point position =
        {
            obstacleRect.x,
            obstacleRect.y
        };


        data.obstaclePositions.push_back(
            position);
    }


    // ==========================================
    // TRAFFIC LIGHTS
    // ==========================================

    data.trafficLights =
        map.GetLaneManager().
        GetTrafficLightSaveStates();


    // ==========================================
    // WRITE SAVE FILE
    // ==========================================

    return SaveManager::Save(
        data);
}


// ==================================================
// LOAD GAME
// ==================================================

bool Game::LoadGame()
{
    SaveData data;


    // ==========================================
    // READ SAVE FILE
    // ==========================================

    if (
        !SaveManager::Load(
            data))
    {
        return false;
    }


    // ==========================================
    // PLAYER
    // ==========================================

    player.SetPosition(
        data.playerX,
        data.playerY);


    // ==========================================
    // SCORE
    // ==========================================

    score =
        data.score;


    // ==========================================
    // PINE CONES
    // ==========================================

    pineConeManager.
        SetCollectedStates(
            data.pineConeCollected);


    // ==========================================
    // OBSTACLES
    // ==========================================

    std::vector<Obstacle*>
        obstacles =
        map.GetLaneManager().
        GetMutableObstacles();


    size_t obstacleCount =
        data.obstaclePositions.size();


    if (
        obstacleCount >
        obstacles.size())
    {
        obstacleCount =
            obstacles.size();
    }


    for (
        size_t i = 0;
        i < obstacleCount;
        i++)
    {
        obstacles[i]->SetPosition(
            data.obstaclePositions[i].x,
            data.obstaclePositions[i].y);
    }


    // ==========================================
    // TRAFFIC LIGHTS
    // ==========================================

    map.GetLaneManager().
        RestoreTrafficLightSaveStates(
            data.trafficLights);


    // ==========================================
    // RESTORE GAME STATE
    // ==========================================

    state =
        data.state;


    // ==========================================
    // KHÔNG PHÁT INTRO KHI LOAD
    // ==========================================

    introRequested =
        false;


    return true;
}