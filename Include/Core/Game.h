#pragma once

#include <SDL.h>

#include "Core/GameState.h"
#include "Graphics/FontManager.h"
#include "Graphics/TextRenderer.h"
#include "Managers/VehicleManager.h"
#include "Objects/Player.h"
#include "World/Goal.h"

class Game
{
public:
    Game();

    void Update();

    void Render(
        SDL_Renderer* renderer,
        TextRenderer& textRenderer,
        FontManager& fontManager);

    void SetState(GameState newState);
    GameState GetState() const;

private:
    Player player;
    VehicleManager vehicleManager;
    Goal goal;
    GameState state;
};