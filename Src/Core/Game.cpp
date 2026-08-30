#include "Core/Game.h"

#include "Managers/CollisionManager.h"
#include "Managers/InputManager.h"

#include "Objects/Obstacle.h"


// ==================================================
// CONSTRUCTOR
// ==================================================

Game::Game()
{
    // Game bắt đầu ở Menu
    state =
        GameState::Menu;
}


// ==================================================
// UPDATE
// ==================================================

void Game::Update()
{
    // ==========================================
    // MENU
    // ==========================================

    if (state ==
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
                // NEW GAME
                // ==================================

            case 0:

                player.Reset();

                state =
                    GameState::Playing;

                break;


                // ==================================
                // LOAD GAME
                // ==================================

            case 1:

                // Sẽ cài đặt ở bước Save / Load

                break;


                // ==================================
                // LEADERBOARD
                // ==================================

            case 2:

                // Sẽ cài đặt ở bước Leaderboard

                break;


                // ==================================
                // SETTINGS
                // ==================================

            case 3:

                // Sẽ cài đặt ở bước Settings

                break;


                // ==================================
                // EXIT
                // ==================================

            case 4:

                state =
                    GameState::Exit;

                break;


            default:

                break;
            }
        }


        return;
    }


    // ==========================================
    // NON-PLAYING STATES
    // ==========================================

    if (state !=
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

    if (state ==
        GameState::Menu)
    {
        menu.Draw(
            renderer,
            textRenderer,
            fontManager);

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
    // GAME STATE UI
    // ==========================================

    switch (state)
    {
    case GameState::Playing:

        break;


    case GameState::GameOver:

        textRenderer.Draw(
            renderer,
            fontManager.GetFont(
                "default"),
            "GAME OVER",
            280,
            220);


        textRenderer.Draw(
            renderer,
            fontManager.GetFont(
                "default"),
            "Press ENTER to restart",
            220,
            270);

        break;


    case GameState::LevelComplete:

        textRenderer.Draw(
            renderer,
            fontManager.GetFont(
                "default"),
            "YOU WIN!",
            300,
            220);


        textRenderer.Draw(
            renderer,
            fontManager.GetFont(
                "default"),
            "Press ENTER to continue",
            210,
            270);

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