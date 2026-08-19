#pragma once

#include <SDL.h>

#include "World/Goal.h"
#include "Managers/VehicleManager.h"

class TextureManager;

class Map
{
public:
    Map();

    void Update();

    void Draw(
        SDL_Renderer* renderer,
        TextureManager& textureManager);

    Goal& GetGoal();

    VehicleManager& GetVehicleManager();

private:
    Goal goal;
    VehicleManager vehicleManager;
};