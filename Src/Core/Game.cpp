#include "Core/Game.h"


#include "Managers/CollisionManager.h"
#include "Managers/InputManager.h"


#include "Objects/Obstacle.h"


#include "Graphics/Texture.h"


#include <string>
#include <iostream>


// ==================================================
// HELPER FUNCTIONS
// ==================================================

namespace
{

    const int STATE_MENU_ITEM_COUNT =
        3;


    // ==================================================
    // STATE FRAME RECT
    //
    // Frame được đưa lên phía trên một chút để chừa
    // đủ chỗ cho 3 button nằm hoàn toàn bên dưới.
    // ==================================================

    SDL_Rect GetStateFrameRect()
    {
        const int frameWidth =
            760;


        const int frameHeight =
            430;


        const int frameY =
            45;


        SDL_Rect frameRect =
        {
            (1280 - frameWidth) / 2,
            frameY,

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
    // LOAD STATE BUTTON TEXTURES
    // ==================================================

    void EnsureStateButtonTexturesLoaded(
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
        // PLAY AGAIN
        // ==========================================

        if (
            textureManager.GetTexture(
                "state_play_again") == nullptr)
        {
            textureManager.LoadTexture(
                renderer,
                "state_play_again",
                "Assets/Images/UI/State/Playagian_button.png");
        }


        if (
            textureManager.GetTexture(
                "state_play_again_choose") == nullptr)
        {
            textureManager.LoadTexture(
                renderer,
                "state_play_again_choose",
                "Assets/Images/UI/State/Playagian_button_choose.png");
        }


        // ==========================================
        // SAVE
        // ==========================================

        if (
            textureManager.GetTexture(
                "state_save") == nullptr)
        {
            textureManager.LoadTexture(
                renderer,
                "state_save",
                "Assets/Images/UI/State/Save_button.png");
        }


        if (
            textureManager.GetTexture(
                "state_save_choose") == nullptr)
        {
            textureManager.LoadTexture(
                renderer,
                "state_save_choose",
                "Assets/Images/UI/State/Save_button_choose.png");
        }


        // ==========================================
        // MENU
        // ==========================================

        if (
            textureManager.GetTexture(
                "state_menu") == nullptr)
        {
            textureManager.LoadTexture(
                renderer,
                "state_menu",
                "Assets/Images/UI/State/Menu_button.png");
        }


        if (
            textureManager.GetTexture(
                "state_menu_choose") == nullptr)
        {
            textureManager.LoadTexture(
                renderer,
                "state_menu_choose",
                "Assets/Images/UI/State/Menu_button_choose.png");
        }
    }


    // ==================================================
    // DRAW ONE STATE BUTTON
    // ==================================================

    void DrawStateButton(
        SDL_Renderer* renderer,
        TextureManager& textureManager,
        const char* textureId,
        int x,
        int y,
        int width,
        int height)
    {
        Texture* texture =
            textureManager.GetTexture(
                textureId);


        if (
            texture == nullptr ||
            texture->GetTexture() == nullptr)
        {
            return;
        }


        SDL_Rect destination =
        {
            x,
            y,
            width,
            height
        };


        SDL_RenderCopy(
            renderer,
            texture->GetTexture(),
            nullptr,
            &destination);
    }


    // ==================================================
    // DRAW STATE BUTTONS
    // ==================================================

    void DrawStateButtons(
        SDL_Renderer* renderer,
        TextureManager& textureManager,
        int selectedIndex)
    {
        EnsureStateButtonTexturesLoaded(
            renderer,
            textureManager);


        // Asset gốc của bạn là 300 x 53.
        const int buttonWidth =
            300;


        const int buttonHeight =
            53;


        const int buttonX =
            (1280 - buttonWidth) / 2;


        SDL_Rect frameRect =
            GetStateFrameRect();


        const int gapFromFrame =
            15;


        const int buttonSpacing =
            10;


        const int firstButtonY =
            frameRect.y +
            frameRect.h +
            gapFromFrame;


        // ==========================================
        // PLAY AGAIN
        // ==========================================

        DrawStateButton(
            renderer,
            textureManager,
            selectedIndex == 0
            ? "state_play_again_choose"
            : "state_play_again",
            buttonX,
            firstButtonY,
            buttonWidth,
            buttonHeight);


        // ==========================================
        // SAVE
        // ==========================================

        DrawStateButton(
            renderer,
            textureManager,
            selectedIndex == 1
            ? "state_save_choose"
            : "state_save",
            buttonX,
            firstButtonY +
            buttonHeight +
            buttonSpacing,
            buttonWidth,
            buttonHeight);


        // ==========================================
        // MENU
        // ==========================================

        DrawStateButton(
            renderer,
            textureManager,
            selectedIndex == 2
            ? "state_menu_choose"
            : "state_menu",
            buttonX,
            firstButtonY +
            (buttonHeight + buttonSpacing) * 2,
            buttonWidth,
            buttonHeight);
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


    stateMenuSelectedIndex =
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
                player.Reset();


                score =
                    0;


                pineConeManager.Reset();


                // Tạo lại Map để obstacle và traffic light
                // bắt đầu lại từ trạng thái ban đầu.
                map =
                    Map();


                stateMenuSelectedIndex =
                    0;


                // Chỉ START GAME từ menu mới chạy intro.
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


        stateMenuSelectedIndex =
            0;


        return;
    }


    // ==========================================
    // RESUME
    //
    // Không cần Continue button.
    // Nhấn P lần nữa để tiếp tục.
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
    // PAUSE / LOSE / WIN ACTION MENU
    // ==========================================

    if (
        state == GameState::Paused ||
        state == GameState::GameOver ||
        state == GameState::LevelComplete)
    {
        // ======================================
        // MOVE UP
        // ======================================

        if (
            InputManager::IsKeyPressed(
                SDL_SCANCODE_UP) ||
            InputManager::IsKeyPressed(
                SDL_SCANCODE_W))
        {
            stateMenuSelectedIndex--;


            if (
                stateMenuSelectedIndex < 0)
            {
                stateMenuSelectedIndex =
                    STATE_MENU_ITEM_COUNT - 1;
            }
        }


        // ======================================
        // MOVE DOWN
        // ======================================

        if (
            InputManager::IsKeyPressed(
                SDL_SCANCODE_DOWN) ||
            InputManager::IsKeyPressed(
                SDL_SCANCODE_S))
        {
            stateMenuSelectedIndex++;


            if (
                stateMenuSelectedIndex >=
                STATE_MENU_ITEM_COUNT)
            {
                stateMenuSelectedIndex =
                    0;
            }
        }


        // ======================================
        // SELECT
        // ======================================

        if (
            InputManager::IsKeyPressed(
                SDL_SCANCODE_RETURN))
        {
            switch (
                stateMenuSelectedIndex)
            {
                // ==================================
                // PLAY AGAIN
                // ==================================

            case 0:
            {
                player.Reset();


                score =
                    0;


                pineConeManager.Reset();


                map =
                    Map();


                // Rất quan trọng:
                // Play Again đi thẳng vào gameplay,
                // KHÔNG phát intro lần nữa.
                introRequested =
                    false;


                state =
                    GameState::Playing;


                stateMenuSelectedIndex =
                    0;


                break;
            }


            // ==================================
            // SAVE
            // ==================================

            case 1:
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


                break;
            }


            // ==================================
            // MENU
            // ==================================

            case 2:
            {
                introRequested =
                    false;


                state =
                    GameState::Menu;


                stateMenuSelectedIndex =
                    0;


                break;
            }


            default:

                break;
            }
        }


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


        stateMenuSelectedIndex =
            0;


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


            stateMenuSelectedIndex =
                0;


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


        DrawStateButtons(
            renderer,
            textureManager,
            stateMenuSelectedIndex);


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


        DrawStateButtons(
            renderer,
            textureManager,
            stateMenuSelectedIndex);


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


        DrawStateButtons(
            renderer,
            textureManager,
            stateMenuSelectedIndex);


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


    if (
        state == GameState::Paused ||
        state == GameState::GameOver ||
        state == GameState::LevelComplete)
    {
        stateMenuSelectedIndex =
            0;
    }
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


    stateMenuSelectedIndex =
        0;


    // ==========================================
    // KHÔNG PHÁT INTRO KHI LOAD
    // ==========================================

    introRequested =
        false;


    return true;
}
