#include "Core/Game.h"


#include "Managers/CollisionManager.h"
#include "Managers/InputManager.h"


#include "Objects/Obstacle.h"


#include <string>


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
        //
        // Phần tròn bên trái chứa icon quả thông.
        // Vì vậy chỉ center text trong phần bảng gỗ.
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


        // Điểm nằm ở phần trên của frame kết thúc.
        //
        // Nếu asset của bạn cần thấp/cao hơn,
        // chỉ cần thay số 90 này.

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

                // Save / Load sẽ làm sau.

                break;


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
    //
    // Player / Map / Collision / PineCone
    // đều không update.
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
    //
    // Hiện:
    // - Score frame
    // - Pause hint
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


        // Điểm được vẽ SAU frame,
        // vì vậy sẽ nằm trên frame.

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


        // Điểm được vẽ SAU frame,
        // vì vậy sẽ nằm trên frame.

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