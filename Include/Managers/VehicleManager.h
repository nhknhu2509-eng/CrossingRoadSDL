#pragma once

#include <vector>
#include <SDL.h>

#include "World/Lane.h"

class VehicleManager
{
public:
    VehicleManager();

    void Update();
    void Draw(SDL_Renderer* renderer);

    std::vector<Vehicle> GetVehicles() const;

private:
    std::vector<Lane> lanes;
};