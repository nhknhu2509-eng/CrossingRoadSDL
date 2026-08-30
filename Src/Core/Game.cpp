#include "Core/Game.h"

#include "Managers/CollisionManager.h"
#include "Objects/Obstacle.h"


Game::Game()
{
    state =
        GameState::Playing;
}


// ==================================================
// UPDATE
// ==================================================

void Game::Update()
{
    if (state !=
        GameState::Playing)
    {
        return;
    }


    player.Update();


    // ==========================================
    // GOAL CHECK
    // ==========================================

    // Giữ nguyên:
    // Goal vẫn dùng Player rect,
    // KHÔNG đổi sang hitbox.
    if (goal.Reached(
        player.GetRect()))
    {
        state =
            GameState::LevelComplete;

        return;
    }


    vehicleManager.Update();


    // ==========================================
    // POLYMORPHIC COLLISION
    // ==========================================

    for (
        const Obstacle* obstacle :
        vehicleManager.GetObstacles())
    {
        if (
            CollisionManager::CheckCollision(
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
    goal.Draw(
        renderer);


    vehicleManager.Draw(
        renderer,
        textureManager);


    player.Draw(
        renderer,
        textureManager);


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
// STATE
// ==================================================

void Game::SetState(
    GameState newState)
{
    state =
        newState;
}


GameState
Game::GetState() const
{
    return state;
}