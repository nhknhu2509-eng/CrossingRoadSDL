#pragma once

#include <SDL.h>

#include "World/Goal.h"
#include "Managers/LaneManager.h"


class TextureManager;


// ==================================================
// MAP
// ==================================================
//
// Map quản lý các thành phần thuộc thế giới game:
//
// - Goal
// - LaneManager
//
// Game không trực tiếp sở hữu các thành phần này.
//
// ==================================================

class Map
{
public:

    Map();


    // ==========================================
    // UPDATE
    // ==========================================

    void Update();


    // ==========================================
    // DRAW
    // ==========================================

    void Draw(
        SDL_Renderer* renderer,
        TextureManager& textureManager);


    // ==========================================
    // GOAL
    // ==========================================

    Goal& GetGoal();

    const Goal& GetGoal() const;


    // ==========================================
    // LANE MANAGER
    // ==========================================

    LaneManager& GetLaneManager();

    const LaneManager&
        GetLaneManager() const;


private:

    Goal goal;

    LaneManager laneManager;
};