#pragma once

#include <SDL.h>

#include "Core/GameState.h"

#include "Graphics/FontManager.h"
#include "Graphics/TextRenderer.h"
#include "Graphics/TextureManager.h"

#include "Objects/Player.h"

#include "World/Map.h"

#include "UI/Menu.h"


class Game
{
public:

    Game();


    // ==========================================
    // GAME LOOP
    // ==========================================

    void Update();


    void Render(
        SDL_Renderer* renderer,
        TextRenderer& textRenderer,
        FontManager& fontManager,
        TextureManager& textureManager);


    // ==========================================
    // GAME STATE
    // ==========================================

    void SetState(
        GameState newState);


    GameState GetState() const;


private:

    // ==========================================
    // MENU
    // ==========================================

    Menu menu;


    // ==========================================
    // PLAYER
    // ==========================================

    Player player;


    // ==========================================
    // WORLD
    // ==========================================

    Map map;


    // ==========================================
    // STATE
    // ==========================================

    GameState state;
};