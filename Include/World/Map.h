#pragma once
#pragma once

#include <SDL.h>

#include "World/Goal.h"
#include "Managers/VehicleManager.h"

class Map
{
public:
    Map();

    void Update();

    void Draw(SDL_Renderer* renderer);

    Goal& GetGoal();

    VehicleManager& GetVehicleManager();

private:
    Goal goal;

    VehicleManager vehicleManager;
};