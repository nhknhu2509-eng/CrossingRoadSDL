#include "Core/Game.h"

#include "Managers/CollisionManager.h"
#include "Managers/InputManager.h"

#include "Objects/Obstacle.h"


// ==================================================
// HELPER - DRAW STATE OVERLAY
// ==================================================

namespace
{
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
        // GET FRAME TEXTURE
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


        // ==========================================
        // FRAME POSITION
        // ==========================================

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


        // ==========================================
        // DRAW FRAME
        // ==========================================

        SDL_RenderCopy(
            renderer,
            frame->GetTexture(),
            nullptr,
            &frameRect);
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
    // Không update Player / Map / Collision.
    // Toàn bộ gameplay đứng yên.
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
    // PLAYER
    // ==========================================

    player.Draw(
        renderer,
        textureManager);


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

        break;


        // ======================================
        // WIN
        // ======================================

    case GameState::LevelComplete:

        DrawStateOverlay(
            renderer,
            textureManager,
            "win_frame");

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