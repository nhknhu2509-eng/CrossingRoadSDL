#include "Core/Game.h"

#include "Managers/CollisionManager.h"
#include "Objects/Vehicle.h"
#include "Objects/Animal.h"

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

    // =====================================
    // GOAL CHECK
    // =====================================
    // Vẫn dùng rect của Player cho Goal.
    if (goal.Reached(
        player.GetRect()))
    {
        state = GameState::LevelComplete;
        return;
    }

    vehicleManager.Update();

    // =====================================
    // WAGON COLLISION
    // =====================================
    // Chỉ thay Player rect -> Player hitbox.
    // Hitbox của Wagon KHÔNG thay đổi.
    for (const Vehicle& vehicle :
        vehicleManager.GetVehicles())
    {
        if (CollisionManager::CheckCollision(
            player.GetHitbox(),
            vehicle.GetHitbox()))
        {
            player.Reset();
            state = GameState::GameOver;
            return;
        }
    }

    // =====================================
    // DEER COLLISION
    // =====================================
    // Chỉ thay Player rect -> Player hitbox.
    // Hitbox của Deer KHÔNG thay đổi.
    for (const Animal& animal :
        vehicleManager.GetAnimals())
    {
        if (CollisionManager::CheckCollision(
            player.GetHitbox(),
            animal.GetHitbox()))
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
    FontManager& fontManager,
    TextureManager& textureManager)
{
    goal.Draw(renderer);

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

void Game::SetState(
    GameState newState)
{
    state = newState;
}

GameState Game::GetState() const
{
    return state;
}