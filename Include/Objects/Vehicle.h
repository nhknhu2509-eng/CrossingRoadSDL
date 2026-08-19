#pragma once

#include <vector>
#include <SDL.h>

#include "World/Lane.h"
#include "Objects/Animal.h"

class TextureManager;

class VehicleManager
{
public:
    VehicleManager();

    void Update();

    void Draw(
        SDL_Renderer* renderer,
        TextureManager& textureManager);

    std::vector<Vehicle>
        GetVehicles() const;

    std::vector<Animal>
        GetAnimals() const;

private:
    std::vector<Lane> lanes;
};