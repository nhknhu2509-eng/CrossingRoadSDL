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
// - VehicleManager
//
// Game sẽ không trực tiếp sở hữu các thành phần này nữa.
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
    // OBSTACLES
    // ==========================================

    VehicleManager& GetVehicleManager();

    const VehicleManager&
        GetVehicleManager() const;


private:

    Goal goal;

    VehicleManager vehicleManager;
};