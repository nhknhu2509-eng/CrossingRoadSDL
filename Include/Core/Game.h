#pragma once

#include <SDL.h>


#include "Core/GameState.h"


#include "Graphics/FontManager.h"
#include "Graphics/TextRenderer.h"
#include "Graphics/TextureManager.h"


#include "Objects/Player.h"


#include "World/Map.h"


#include "UI/Menu.h"


#include "Managers/PineConeManager.h"
#include "Managers/SaveManager.h"


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


    // ==========================================
    // INTRO
    // ==========================================

    bool ConsumeIntroRequest();


private:

    // ==========================================
    // SAVE / LOAD
    // ==========================================

    bool SaveGame();

    bool LoadGame();


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
    // PINE CONES
    // ==========================================

    PineConeManager
        pineConeManager;


    // ==========================================
    // SCORE
    // ==========================================

    int score;


    // ==========================================
    // STATE
    // ==========================================

    GameState state;


    // ==========================================
    // STATE ACTION MENU
    //
    // 0 = PLAY AGAIN
    // 1 = SAVE
    // 2 = MENU
    // ==========================================

    int stateMenuSelectedIndex;


    // ==========================================
    // INTRO REQUEST
    // ==========================================

    bool introRequested;
};
