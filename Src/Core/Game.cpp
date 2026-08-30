#include "Core/Game.h"

#include "Managers/CollisionManager.h"

#include "Objects/Obstacle.h"


// ==================================================
// CONSTRUCTOR
// ==================================================

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
    // ==========================================
    // STATE CHECK
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
    //
    // Giữ nguyên logic hiện tại:
    //
    // Goal vẫn kiểm tra PLAYER RECT,
    // KHÔNG dùng player hitbox.
    //
    // Quan trọng:
    // Goal vẫn được kiểm tra TRƯỚC khi
    // obstacle được Update.
    //
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
    //
    // Tương đương chính xác với:
    //
    // vehicleManager.Update();
    //
    // vì Map::Update() gọi:
    //
    // vehicleManager.Update();
    //
    // ==========================================

    map.Update();


    // ==========================================
    // 4. COLLISION CHECK
    // ==========================================

    for (
        const Obstacle* obstacle :
        map.GetVehicleManager().
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
    // MAP
    // ==========================================
    //
    // Map::Draw() giữ nguyên thứ tự:
    //
    // Goal
    // Vehicle / Animal / TrafficLight
    //
    // ==========================================

    map.Draw(
        renderer,
        textureManager);


    // ==========================================
    // PLAYER
    // ==========================================
    //
    // Player vẫn được vẽ SAU map.
    //
    // Vì vậy render order không thay đổi.
    //
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