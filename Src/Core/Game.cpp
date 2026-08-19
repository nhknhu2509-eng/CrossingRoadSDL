#include "Core/Game.h"

#include "Managers/CollisionManager.h"
#include "Objects/Vehicle.h"

Game::Game()
{
    state = GameState::Playing;
}

void Game::Update()
{
    if (state != GameState::Playing)
    {
        return;
    }

    player.Update();

    // Người chơi tới đích
    if (goal.Reached(player.GetRect()))
    {
        state = GameState::LevelComplete;
        return;
    }

    vehicleManager.Update();

    // Kiểm tra va chạm
    for (const Vehicle& vehicle : vehicleManager.GetVehicles())
    {
        if (CollisionManager::CheckCollision(
            player.GetRect(),
            vehicle.GetRect()))
        {
            player.Reset();

            state = GameState::GameOver;

            return;
        }
    }
}

void Game::Render(
    SDL_Renderer* renderer,
    TextRenderer& textRenderer,
    FontManager& fontManager)
{
    goal.Draw(renderer);

    vehicleManager.Draw(renderer);

    player.Draw(renderer);

    switch (state)
    {
    case GameState::Playing:

        textRenderer.Draw(
            renderer,
            fontManager.GetFont("default"),
            "Crossing Road SDL",
            20,
            20);

        break;

    case GameState::GameOver:

        textRenderer.Draw(
            renderer,
            fontManager.GetFont("default"),
            "GAME OVER",
            280,
            220);

        textRenderer.Draw(
            renderer,
            fontManager.GetFont("default"),
            "Press ENTER to restart",
            220,
            270);

        break;

    case GameState::LevelComplete:

        textRenderer.Draw(
            renderer,
            fontManager.GetFont("default"),
            "YOU WIN!",
            300,
            220);

        textRenderer.Draw(
            renderer,
            fontManager.GetFont("default"),
            "Press ENTER to continue",
            210,
            270);

        break;

    default:
        break;
    }
}

void Game::SetState(GameState newState)
{
    state = newState;
}

GameState Game::GetState() const
{
    return state;
}