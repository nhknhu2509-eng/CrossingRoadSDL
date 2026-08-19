#pragma once

#include <vector>
#include <SDL.h>

#include "Objects/Vehicle.h"
#include "World/TrafficLight.h"

class Lane
{
public:
    Lane(
        int y,
        int direction,
        int speed,
        int vehicleCount);

    void Update();

    void Draw(SDL_Renderer* renderer);

    const std::vector<Vehicle>& GetVehicles() const;

private:
    std::vector<Vehicle> vehicles;

    TrafficLight trafficLight;
};