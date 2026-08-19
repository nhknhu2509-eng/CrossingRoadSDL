#pragma once

#include <vector>
#include <SDL.h>

#include "Objects/Vehicle.h"
#include "World/TrafficLight.h"

class TextureManager;

class Lane
{
public:
    Lane(
        int y,
        int height,
        int direction,
        int speed,
        int vehicleCount);

    void Update();

    void Draw(
        SDL_Renderer* renderer,
        TextureManager& textureManager);

    const std::vector<Vehicle>& GetVehicles() const;

private:
    std::vector<Vehicle> vehicles;

    TrafficLight trafficLight;
};